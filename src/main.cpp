#include "classes/daemon/DaemonApp.h"
#include <iostream>

int main(int argc, char* argv[]) {
    DaemonApp daemon(argc, argv);

    if (!daemon.parseCommand()) {
        return 1;
    }

    if (!daemon.readConfig()) {
        return 1;
    }

    if (!daemon.process()) {
        return 1;
    }

    return 0;

}

