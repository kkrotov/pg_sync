#include "../common.h"
#include "Thread.h"

Thread::Thread() :
    exitAfterRunsCount{0},
    runsCount{0},
    threadSleepSeconds{1},
    threadPrepareSleepSeconds{1},
    status{ThreadStatus::THREAD_CREATED},
    status_ready{false},
    status_prepared{false} {

    id = string_fmt("%d", rand());
    name = id;

    setRealStatus(ThreadStatus::THREAD_CREATED);
}

Thread::~Thread() {
}

void Thread::start(int exitAfterRunsCount, bool skipPrepare) {
    this->exitAfterRunsCount = exitAfterRunsCount;
    this->status_prepared = skipPrepare;
    std::thread t(&Thread::operator(), this);
    std::swap(task_thread, t);
}

void Thread::ssleep(unsigned int seconds) {
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
}

void Thread::usleep(unsigned int milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

void Thread::threadTotalsHeader(stringstream &html) {
    html << "<tr>\n";
    html << "<td style='text-align: left' nowrap>Thread</td>\n";
    html << "<td style='text-align: left' nowrap>Status</td>\n";
    html << "<td style='text-align: left' nowrap>Count</td>\n";
    html << "<td style='text-align: left' nowrap>Time last</td>\n";
    html << "<td style='text-align: left' nowrap>Time total</td>\n";
    html << "<td style='text-align: left' nowrap>Errors</td>\n";
    html << "<td style='text-align: left' nowrap>Last error</td>\n";
    html << "</tr>";
}

void Thread::threadTotalsData(stringstream &html) {
    ThreadStatus status = getStatus();
    ThreadStatus real_status = getRealStatus();

    html << "<tr>\n";
    if (hasFullHtml()) {
        html << "<td style='text-align: left' nowrap><a href='/task?id=" << this->id << "'>" << this->name << "</a></td>\n";
    } else {
        html << "<td style='text-align: left' nowrap>" << this->name << "</td>\n";
    }
    html << "<td style='text-align: left' nowrap>" << ThreadStatusNames[real_status] << (status != real_status ? " (" + string(ThreadStatusNames[status]) + ")" : "") << "</td>\n";
    html << "<td style='text-align: left' nowrap>" << timer.count << "</td>\n";
    html << "<td style='text-align: left' nowrap>" << timer.sloop() << " " << timer.sloop_cpu_usage() << "%</td>\n";
    html << "<td style='text-align: left' nowrap>" << timer.sfull() << " " << timer.sfull_cpu_usage() << "%</td>\n";
    html << "<td style='text-align: left' nowrap>" << errorsCount << "</td>\n";
    html << "<td style='text-align: left' nowrap>" << lastError << "</td>\n";
    html << "</tr>\n";
}

void Thread::html(stringstream &html) {
    html << "<table width=100%>\n";
    this->threadTotalsHeader(html);
    this->threadTotalsData(html);
    html << "</table>\n";
    html << "<hr/><br/>\n";
}

void Thread::htmlfull(stringstream &html) {
    this->html(html);
}

bool Thread::hasFullHtml() {
    return false;
}

bool Thread::ready() {
    return true;
}

bool Thread::prepare() {
    return true;
}

void Thread::operator()() {

    onStarted(this);

    while (true) {

        try {
            if (getStatus() == ThreadStatus::THREAD_STOPPED) {
                setRealStatus(ThreadStatus::THREAD_STOPPED);
                onFinished(this);
                return;
            } else if (getStatus() == ThreadStatus::THREAD_PAUSED) {
                setRealStatus(ThreadStatus::THREAD_PAUSED);
            } else if (getStatus() == ThreadStatus::THREAD_RUNNING || getStatus() == ThreadStatus::THREAD_PREPARING) {
                if (!status_ready) {
                    setRealStatus(ThreadStatus::THREAD_WAITING);
                    if (this->ready()) {
                        status_ready = true;
                        continue;
                    }
                } else if (!status_prepared) {
                    setRealStatus(ThreadStatus::THREAD_PREPARING);
                    if (this->prepare()) {
                        status_prepared = true;
                        continue;
                    }
                } else {
                    if (getStatus() == ThreadStatus::THREAD_RUNNING) {
                        setRealStatus(ThreadStatus::THREAD_RUNNING);

                        {
                            TimerScope ts(timer);
                            this->run();
                        }

                        lastError = "";

                        if (exitAfterRunsCount)
                        {
                            ++runsCount;
                            if (runsCount == exitAfterRunsCount) {
                                setStatus(ThreadStatus::THREAD_STOPPED);
                                continue;
                            }
                        }

                        ssleep(threadSleepSeconds);
                        continue;

                    } else {
                        setRealStatus(ThreadStatus::THREAD_PAUSED);
                    }
                }
            }


            ssleep(threadPrepareSleepSeconds);
            continue;

        } catch (ThreadInterrupted const& e) {
            continue;
        } catch (Exception &e) {
            e.addTrace("Thread(" + name + "): ");
            Log::exception(e);
            lastError = e.getFullMessage();
            errorsCount++;
        } catch (std::exception &e) {
            Log::error("Thread(" + name + "): " + string(e.what()));
            lastError = string(e.what());
            errorsCount++;
        } catch (...) {
            Log::error("Thread(" + name + "): ERROR");
            lastError = "Unknown error";
            errorsCount++;
        }
        
        if (exitAfterRunsCount)
        {
            setStatus(ThreadStatus::THREAD_STOPPED);
            continue;
        }

        try {
            ssleep(threadSleepSeconds);
        } catch (ThreadInterrupted const& e) {
            continue;
        }
    }

    onFinished(this);
}

void Thread::setStatus(ThreadStatus status) {
    this->status = status;
    if (status == THREAD_STOPPED) {
        onShutdown();
    }
}

void Thread::setRealStatus(ThreadStatus real_status) {
    this->real_status = real_status;
    onRealStatusChanged(this);
}

ThreadStatus Thread::getStatus() {
    return status;
}

ThreadStatus Thread::getRealStatus() {
    return real_status;
}
       