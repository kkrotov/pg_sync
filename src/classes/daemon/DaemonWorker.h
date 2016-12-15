#pragma once

#include <execinfo.h>
#include <signal.h>
#include <sys/resource.h>
#include "../AppBill.h"

#define CHILD_NEED_WORK         0
#define CHILD_NEED_TERMINATE    1000
#define FD_LIMIT                1024*10

class DaemonWorker {

public:
    int run() {

        struct sigaction sigact;
        sigact.sa_flags = SA_SIGINFO;       // сигналы об ошибках в программе будут обрататывать более тщательно. указываем что хотим получать расширенную информацию об ошибках
        sigact.sa_sigaction = DaemonWorker::signal_error; // задаем функцию обработчик сигналов

        sigemptyset(&sigact.sa_mask);   // установим наш обработчик на сигналы
        sigaction(SIGFPE, &sigact, 0);  // ошибка FPU
        sigaction(SIGILL, &sigact, 0);  // ошибочная инструкция
        sigaction(SIGSEGV, &sigact, 0); // ошибка доступа к памяти
        sigaction(SIGBUS, &sigact, 0);  // ошибка шины, при обращении к физической памяти

        sigset_t         sigset;
        sigemptyset(&sigset);           // блокируем сигналы которые будем ожидать
        sigaddset(&sigset, SIGQUIT);    // сигнал остановки процесса пользователем
        sigaddset(&sigset, SIGINT);     // сигнал для остановки процесса пользователем с терминала
        sigaddset(&sigset, SIGTERM);    // сигнал запроса завершения процесса
        sigaddset(&sigset, SIGUSR1);    // пользовательский сигнал который мы будем использовать для обновления конфига
        sigprocmask(SIG_BLOCK, &sigset, NULL);


        setFdLimit(FD_LIMIT);           // Установим максимальное кол-во дискрипторов которое можно открыть

        logInfo("Started");

        bool status = app().start();    // запускаем все рабочие потоки
        if (!status) {

            for (;;) {  // цикл ожижания сообщений
                int signo;
                sigwait(&sigset, &signo);   // ждем указанных сообщений
                string message = string_fmt("Got signal %d", signo);
        	logInfo(message);

                if (signo == SIGUSR1) {     // если то сообщение обновления конфига, обновим конфиг
                    status = false;     // ReloadConfig();
                    if (!status) {
                        logError("Reload config failed");
                    } else {
                        logInfo("Reload config OK");
                    }
                } else {  // если какой-либо другой сигнал, то выйдим из цикла
                    break;
                }
            }

            app().stop();   // остановим все рабочеи потоки и корректно закроем всё что надо
        } else {
            logError("Create work thread failed");
        }

        logInfo("Stopped");

        return CHILD_NEED_TERMINATE;    // вернем код не требующим перезапуска
    }

protected:

    int setFdLimit(int MaxFd) // функция установки максимального кол-во дескрипторов которое может быть открыто
    {
        struct rlimit lim;
        lim.rlim_cur = MaxFd;   // зададим текущий лимит на кол-во открытых дискриптеров
        lim.rlim_max = MaxFd;   // зададим максимальный лимит на кол-во открытых дискриптеров


        int status = setrlimit(RLIMIT_NOFILE, &lim);    // установим указанное кол-во
        return status;
    }

    void logInfo(string message) {
        Log::info("[WORKER] " + message);
        app().logger.processLogQueue();
    }

    void logNotice(string message) {
        Log::notice("[WORKER] " + message);
        app().logger.processLogQueue();
    }

    void logError(string message) {
        Log::error("[WORKER] " + message);
        app().logger.processLogQueue();
    }

    static void signal_error(int sig, siginfo_t *si, void *ptr) // функция обработки сигналов
    {
        void*  ErrorAddr;
        void*  Trace[16];
        int    x;
        int    TraceSize;

        string message = string_fmt("[DAEMON] Signal: %s, Addr: 0x%0.16X\n", strsignal(sig), si->si_addr);  // запишем в лог что за сигнал пришел

#if __WORDSIZE == 64 // если дело имеем с 64 битной ОС
        ErrorAddr = (void*)((ucontext_t*)ptr)->uc_mcontext.gregs[REG_RIP];  // получим адрес инструкции которая вызвала ошибку
#else
        ErrorAddr = (void*)((ucontext_t*)ptr)->uc_mcontext.gregs[REG_EIP];  // получим адрес инструкции которая вызвала ошибку
    #endif

        TraceSize = backtrace(Trace, 16);   // произведем backtrace чтобы получить весь стек вызовов
        Trace[1] = ErrorAddr;

        char** Messages = backtrace_symbols(Trace, TraceSize);  // получим расшифровку трасировки
        if (Messages) {
            message.append("== Backtrace ==\n");

            for (x = 1; x < TraceSize; x++) {   // запишем в лог
                message.append(string_fmt("%s\n", Messages[x]));
            }

            message.append("== End Backtrace ==\n");
            free(Messages);
        }

        Log::error("[WORKER] " + message);
        Log::info("[WORKER] Stopped");
        app().logger.processLogQueue();

        app().stop();    // остановим все рабочие потоки и корректно закроем всё что надо

        exit(CHILD_NEED_WORK);  // завершим процесс с кодом требующим перезапуска
    }

};

