#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "../AppBill.h"
#include "DaemonMonitor.h"
#include "DaemonWorker.h"
#include "../LogWriterGraylog.h"

class DaemonApp {

    int argc;
    char** argv;
    std::string config_file;
    std::string pid_file;

    bool action_start = false;
    bool action_stop = false;
    bool action_restart = false;
    bool action_reload = false;
    bool action_run = false;

public:
    DaemonApp(int argc, char* argv[]) {

        srand(time(nullptr));

        this->argc = argc;
        this->argv = argv;

        std::string path(argv[0]);
        std::vector<std::string> path_parts;
        boost::algorithm::split(path_parts, path, boost::algorithm::is_any_of("/\\"));

        std::string app_name = path_parts[path_parts.size() - 1];
        path_parts.erase(path_parts.end() - 1);
        std::string app_directory = boost::algorithm::join(path_parts, "/");

        config_file = app_directory + "/" + app_name + ".conf";
        pid_file = "/var/run/" + app_name + ".pid";
    }

    bool parseCommand() {

        int action_commands_count = 0;

        try {
            po::options_description desc("Allowed options");
            desc.add_options()
                    ("help,h", "produce help message")
                    ("start", "start daemon")
                    ("stop", "stop daemon")
                    ("restart", "restart daemon")
                    ("reload", "graceful restart")
                    ("config-file,c", po::value<string>(), "path to config file")
                    ("pid-file,p", po::value<string>(), "path to pid file")
                    ("test,t", "run tests")
                    ("gccver,v", "gcc compiler version")
                    ;

            po::variables_map vm;
            po::store(po::parse_command_line(argc, argv, desc), vm);
            po::notify(vm);


            if (vm.count("help")) {
                cout << desc << "\n";
                return false;
            }

            if (vm.count("gccver")) {
                cout << "GCC Version: " << __GNUC__ << "." << __GNUC_MINOR__ << "." << __GNUC_PATCHLEVEL__ << "\n";
                return false;
            }

            if (vm.count("config-file")) {
                config_file = vm["config-file"].as<string>();
            }

            if (vm.count("pid-file")) {
                pid_file = vm["pid-file"].as<string>();
            }

            if (vm.count("start")) {
                action_start = true;
                action_commands_count++;
            }

            if (vm.count("stop")) {
                action_stop= true;
                action_commands_count++;
            }

            if (vm.count("restart")) {
                action_reload = true;
                action_commands_count++;
            }

            if (vm.count("reload")) {
                action_reload = true;
                action_commands_count++;
            }

        } catch (exception& e) {
            cout << "ERROR: Parse cmd line: " << e.what() << endl;
            return false;
        }

        if (action_commands_count > 1) {
            cout << "ERROR: Please use --start or --stop or --restart or --reload command " << endl;
        }

        if (action_commands_count == 0) {
            action_run = true;
        }

        return true;
    }

    bool readConfig() {
        return app().conf.readConfig(config_file, pid_file);
    }

    bool process() {
        if (action_run) {
            return run();
        } else if (action_start) {
            return start();
        } else if (action_stop) {
            return start();
        } else if (action_restart) {
            return restart();
        } else if (action_reload) {
            return reload();
        }
    }

protected:
    bool run() {
        if (!canRun()) {
            return false;
        }

        savePidFile();

        app().logger.setLogGroupingInterval(app().conf.log_grouping_interval);

        app().logger.addLogWriter(pLogWriter(new LogWriterScreen()));

        if (!app().conf.log_file_filename.empty())
            app().logger.addLogWriter(pLogWriter(new LogWriterFile(app().conf.log_file_filename, app().conf.log_file_min_level, app().conf.log_file_max_level)));

        if (!app().conf.log_syslog_ident.empty())
            app().logger.addLogWriter(pLogWriter(new LogWriterSyslog(app().conf.log_syslog_ident, app().conf.log_syslog_min_level, app().conf.log_syslog_max_level)));

        if (!app().conf.log_graylog_host.empty())
            app().logger.addLogWriter(pLogWriter(new LogWriterGraylog(app().conf.log_graylog_host, app().conf.log_graylog_port, app().conf.log_graylog_source, app().conf.str_instance_id)));

        bool result = app().run();

        removePidFile();

        return result;
    }

    bool start() {
        if (!canRun()) {
            return false;
        }

        pid_t pid = fork();       // создаем потомка
        if (pid == -1) {    // если не удалось запустить потомка
            cout << "Error: Start Daemon failed (" << strerror(errno) << ")" << endl;
            return -1;

        } else if (!pid) {  // если это потомок

            cout << "Starting daemon..." << endl;

            // закрываем дескрипторы ввода/вывода/ошибок, так как нам они больше не понадобятся
            close(STDIN_FILENO);
            if (open("/dev/null",O_RDONLY) == -1) {

                std::cout << "Failed to reopen stdin while daemonising" << std::endl;
                exit(1);
            }
            std::string log_filename = app().conf.log_file_filename;
            if (!log_filename.empty()) {
            
                // перенаправляем stdout и stderr в лог
                int logfile_fileno = open(log_filename.c_str(),O_RDWR|O_CREAT|O_APPEND,S_IRUSR|S_IWUSR|S_IRGRP);
                if (logfile_fileno>0) {

                    dup2(logfile_fileno,STDOUT_FILENO);
                    dup2(logfile_fileno,STDERR_FILENO);
                    close(logfile_fileno);
                }

            } else {

                close(STDOUT_FILENO);
                close(STDERR_FILENO);
            }
            umask(0);   // разрешаем выставлять все биты прав на создаваемые файлы, иначе у нас могут быть проблемы с правами доступа
            setsid();   // создаём новый сеанс, чтобы не зависеть от родителя
            chdir("/"); // переходим в корень диска, если мы этого не сделаем, то могут быть проблемы. к примеру с размантированием дисков

            DaemonMonitor monitor;

            savePidFile();

            int status = monitor.run(); // Данная функция будет осуществлять слежение за процессом

            removePidFile();

            return status;

        }
        // если это родитель
        std::this_thread::sleep_for(std::chrono::seconds(1));
        cout << "Success: Daemon is started" << endl;
        return true; // завершим процес, т.к. основную свою задачу (запуск демона) мы выполнили
    }

    bool stop() {
        if (!canStop()) {
            return false;
        }

        pid_t pid = readPidFile();
        if (pid != 0) {
            kill(pid, SIGTERM);
        }
    }

    bool restart() {
        stop();
        return start();
    }

    bool reload() {
        cout << "Error: Reload action not implemented yet" << endl;
        return false;
    }

    bool isRunning() {
        pid_t pid = readPidFile();
        return pid != 0 && kill(pid, 0) == 0;
    }

    bool canRun() {
        if (isRunning()) {
            cout << "Error: Daemon already running (" << pid_file << ")" << endl;
            return false;
        }
        return true;
    }

    bool canStop() {
        if (!isRunning()) {
            cout << "Skip: Daemon is not running (" << pid_file << ")" << endl;
            return false;
        }
        return true;
    }

    pid_t readPidFile() {
        pid_t pid = 0;

        FILE* file = fopen(pid_file.c_str(), "r");
        if (file != nullptr) {
            fscanf(file, "%u", &pid);
            fclose(file);
        }

        return pid;
    }

    void savePidFile() {
        FILE* file = fopen(pid_file.c_str(), "w+");
        if (file != nullptr) {
            fprintf(file, "%u", getpid());
            fclose(file);
        } else {
            cout << "Skip: Can not save pid (" << pid_file << ")" << endl;
        }
    }

    void removePidFile() {
        unlink(pid_file.c_str());
    }

};
