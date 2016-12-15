#pragma once

#include "../LogWriterScreen.h"
#include "../LogWriterFile.h"
#include "../LogWriterSyslog.h"

#include "DaemonWorker.h"
#include <sys/wait.h>

class DaemonMonitor {

public:
    int run() {

        app().logger.setLogGroupingInterval(0);

        if (!app().conf.log_file_filename.empty())
            app().logger.addLogWriter(pLogWriter(new LogWriterFile(app().conf.log_file_filename, app().conf.log_file_min_level, app().conf.log_file_max_level)));

        if (!app().conf.log_syslog_ident.empty())
            app().logger.addLogWriter(pLogWriter(new LogWriterSyslog(app().conf.log_syslog_ident, app().conf.log_syslog_min_level, app().conf.log_syslog_max_level)));


        sigset_t sigset;
        sigemptyset(&sigset);        // настраиваем сигналы которые будем обрабатывать
        sigaddset(&sigset, SIGQUIT); // сигнал остановки процесса пользователем
        sigaddset(&sigset, SIGINT);  // сигнал для остановки процесса пользователем с терминала
        sigaddset(&sigset, SIGTERM); // сигнал запроса завершения процесса
        sigaddset(&sigset, SIGCHLD); // сигнал посылаемый при изменении статуса дочернего процесса
        sigaddset(&sigset, SIGUSR1); // пользовательский сигнал который мы будем использовать для обновления конфига
        sigprocmask(SIG_BLOCK, &sigset, NULL);

        int      status = 0;
        bool     need_start = true;

        logInfo("Started");

        for (;;) { // бесконечный цикл работы
            pid_t pid;

            if (need_start) {   // если необходимо создать потомка
                pid = fork();   // создаём потомка
            }

            need_start = true;

            if (pid == -1) {    // если произошла ошибка

                logError("Fork failed (" + string(strerror(errno)) + ")");

            } else if (!pid) {  // если мы потомок

                app().logger.setLogGroupingInterval(app().conf.log_grouping_interval);

                DaemonWorker worker;
                status = worker.run();

                exit(status);   // завершим процесс

            } else {            // если мы родитель

                siginfo_t siginfo;
                sigwaitinfo(&sigset, &siginfo);     // ожидаем поступление сигнала
                if (siginfo.si_signo == SIGCHLD) {  // если пришел сигнал от потомка
                    wait(&status);                  // получаем статус завершения

                    status = WEXITSTATUS(status);   // преобразуем статус в нормальный вид

                    if (status == CHILD_NEED_TERMINATE) {   // если потомок завершил работу с кодом говорящем о том, что нет нужды дальше работать
                        logInfo("Child stopped");
                        break;  // прервем цикл
                    }
                    else if (status == CHILD_NEED_WORK) {   // если требуется перезапустить потомка
                        logInfo("Child restart");
                    }

                } else if (siginfo.si_signo == SIGUSR1) {   // если пришел сигнал что необходимо перезагрузить конфиг

                    kill(pid, SIGUSR1); // перешлем его потомку
                    need_start = false; // установим флаг что нам не надо запускать потомка заново

                } else {    // если пришел какой-либо другой ожидаемый сигнал

                    logNotice("Signal " + string(strsignal(siginfo.si_signo)));

                    kill(pid, SIGTERM); // убьем потомка
                    status = 0;
                    break;
                }
            }
        }

        logInfo("Stopped");

        return status;
    }

protected:

    void logInfo(string message) {
        Log::info("[MONITOR] " + message);
        app().logger.processLogQueue();
    }

    void logNotice(string message) {
        Log::notice("[MONITOR] " + message);
        app().logger.processLogQueue();
    }

    void logError(string message) {
        Log::error("[MONITOR] " + message);
        app().logger.processLogQueue();
    }

};