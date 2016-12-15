#include "LogWriterFile.h"

LogWriterFile::LogWriterFile(string fileName, LogLevel minLevel, LogLevel maxLevel) : BaseLogWriter(minLevel, maxLevel) {
    this->fileName = fileName;
}

bool LogWriterFile::beforePublish() {

    file = fopen(fileName.c_str(), "a");
    return file != 0;

}

void LogWriterFile::doPublish(pLogMessage message) {

    string msg = stime(message->log_time) + slevel(message->level) + scount(message->count) + message->message;
    fprintf(file, "%s\n", msg.c_str());

}

void LogWriterFile::afterPublish() {

    fclose(file);

}

string LogWriterFile::logWriterName() {

    return string("LogWriterFile");
}