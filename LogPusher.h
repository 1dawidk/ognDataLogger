#ifndef OGNDATALOGGER_LOGPUSHER_H
#define OGNDATALOGGER_LOGPUSHER_H

#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include "dkulpaclibs/misc/Thread.h"
#include "dkulpaclibs/misc/Clock.h"
#include <curl/curl.h>
#include "OgnLogger.h"

using namespace std;

class LogPusher : public Thread {
public:
    LogPusher(OgnLogger *ognLogger, const char* server);

protected:
    void onStart() override;
    void onRun() override;
    void onStop() override;

private:
    string dataDir;
    pthread_mutex_t *queueMutex;
    const char* serverAdr;
    OgnLogger *ognLogger;
    uint8_t lastHour;
};


#endif //OGNDATALOGGER_LOGPUSHER_H
