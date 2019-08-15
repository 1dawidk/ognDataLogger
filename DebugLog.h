#ifndef OGNDATALOGGER_DEBUGLOG_H
#define OGNDATALOGGER_DEBUGLOG_H

#include <iostream>
#include <fstream>
#include "dkulpaclibs/misc/Thread.h"
#include "dkulpaclibs/misc/Clock.h"

using namespace std;

class DebugLog {
public:
    DebugLog(string debugFileName);

    void write(const char *tag, const char *msg);

    void finish();
private:
    ofstream debugStream;
    pthread_mutex_t streamMutex;
};


#endif //OGNDATALOGGER_DEBUGLOG_H
