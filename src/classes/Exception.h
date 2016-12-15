#pragma once

#include <string>
#include <cstring>
#include <list>

using namespace std;

class Exception : public exception {
public:
    string message;
    list<string> traceList;

    Exception() {
        message = "";
    }

    Exception(const char * _what) {
        message = string(_what);
    }

    Exception(const string &_what) {
        message = _what;
    }

    Exception(const char * _what, const char * _trace) {
        message = string(_what);
        addTrace(string(_trace));
    }

    Exception(const string &_what, const string &_trace) {
        message = _what;
        addTrace(_trace);
    }

    virtual const char* what() const throw () {
        return message.c_str();
    }

    string what() {
        return message;
    }

    void addTrace(const string &trace) {
        traceList.push_front(trace);
    }

    string getFullMessage() {

        string message = "";

        for (auto it = traceList.begin(); it != traceList.end(); ++it) {
            message += *it + " -> ";
        }

        if (message.size() > 0) {
            message += "\n";
        }

        message += what();

        return message;

    }
};
