#ifndef OGNDATALOGGER_LOGGER_H
#define OGNDATALOGGER_LOGGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "dkulpaclibs/misc/Thread.h"
#include "dkulpaclibs/misc/Clock.h"

using namespace std;

class Logger {
public:
    Logger(string debugFileName);

    void write(const char *tag, const char *msg);

    void finish();
private:
    ofstream debugStream;
    pthread_mutex_t streamMutex;
};


#endif //OGNDATALOGGER_LOGGER_H
