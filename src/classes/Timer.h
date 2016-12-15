#pragma once

#include <time.h>
#include <string>
#include <math.h>

using namespace std;

class Timer {
    double time;
    double full_time;
    double cpu_time;
    double cpu_full_time;
    timespec ts_start;
    timespec ts_cpu_start;

    bool started;

public:
    unsigned long long count;

    Timer() {
        this->time = 0.0;
        this->full_time = 0.0;
        this->cpu_time = 0.0;
        this->cpu_full_time = 0.0;
        this->count = 0;
        this->started = false;
    }

    void start() {
        if (started) return;

        clock_gettime(CLOCK_MONOTONIC, &ts_start);
        clock_gettime(CLOCK_THREAD_CPUTIME_ID, &ts_cpu_start);
        started = true;
    }

    void stop() {
        if (!started) return;

        this->time = current();
        this->full_time += this->time;
        this->cpu_time = cpu_current();
        this->cpu_full_time += this->cpu_time;
        this->count++;

        started = false;
    }

    double current() {
        if (started) {
            timespec ts_current;
            clock_gettime(CLOCK_MONOTONIC, &ts_current);
            return ((double)ts_current.tv_sec + (double)ts_current.tv_nsec / 1.0e9) - ((double)ts_start.tv_sec + (double)ts_start.tv_nsec / 1.0e9);
        } else {
            return 0;
        }
    }

    double cpu_current() {
        if (started) {
            timespec ts_cpu_current;
            clock_gettime(CLOCK_THREAD_CPUTIME_ID, &ts_cpu_current);
            return ((double)ts_cpu_current.tv_sec + (double)ts_cpu_current.tv_nsec / 1.0e9) - ((double)ts_cpu_start.tv_sec + (double)ts_cpu_start.tv_nsec / 1.0e9);
        } else {
            return 0;
        }
    }

    double tloop() {
        return this->time + current();
    }

    double tfull() {
        return this->full_time + current();
    }

    double tloop_cpu() {
        return this->cpu_time + cpu_current();
    }

    double tfull_cpu() {
        return this->cpu_full_time + cpu_current();
    }

    string sloop() {
        char buf[50];
        sprintf(buf, "%.6f", tloop());
        return string(buf);
    }

    string sfull() {
        char buf[50];
        sprintf(buf, "%.3f", tfull());
        return string(buf);
    }

    string sloop_cpu() {
        char buf[50];
        sprintf(buf, "%.6f", tloop_cpu());
        return string(buf);
    }

    string sloop_cpu_usage() {
        double time = tloop();
        double timecpu = tloop_cpu();
        double usage = 0.0;
        if (time > 0.00001) {
            usage = timecpu / time * 100.0;
        }
        //char buf[500];
        //sprintf(buf, "%f %f %f %f %d", time, timecpu, usage, round(usage), (int)round(usage));
        char buf[50];
        sprintf(buf, "%d", (int)round(usage));
        return string(buf);
    }

    string sfull_cpu() {
        char buf[50];
        sprintf(buf, "%.3f", tfull_cpu());
        return string(buf);
    }

    string sfull_cpu_usage() {
        double time = tfull();
        double timecpu = tfull_cpu();
        double usage = 0.0;
        if (time > 0.00001) {
            usage = timecpu / time * 100.0;
        }
        char buf[50];
        sprintf(buf, "%d", (int)round(usage));
        return string(buf);
    }
};

class TimerScope {
    Timer *timer;

public:

    TimerScope(Timer &timer) {
        this->timer = &timer;
        start();
    }

    virtual ~TimerScope() {
        stop();
    }

    void start() {
        timer->start();
    }

    void stop() {
        timer->stop();
    }
};