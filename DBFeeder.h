#ifndef OGNDATALOGGER_DBFEEDER_H
#define OGNDATALOGGER_DBFEEDER_H

#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include "dkulpaclibs/misc/Thread.h"
#include "dkulpaclibs/misc/Clock.h"
#include <curl/curl.h>
#include "OgnDataPicker.h"

using namespace std;

class DBFeeder : public Thread {
public:
    DBFeeder(Logger *debugLog, OgnDataPicker *ognLogger, const char* server);

protected:
    void onStart() override;
    void onRun() override;
    void onStop() override;

private:
    string dataDir;
    Logger *debugLog;
    const char* serverAdr;
    OgnDataPicker *ognLogger;
    uint8_t lastHour;
    int linesToday;
};


#endif //OGNDATALOGGER_DBFEEDER_H
