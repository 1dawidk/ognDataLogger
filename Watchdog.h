//
// Created by dkulpa on 12.10.19.
//

#ifndef OGNDATALOGGER_WATCHDOG_H
#define OGNDATALOGGER_WATCHDOG_H


#include <dkulpaclibs/misc/Thread.h>
#include "OgnLogger.h"

class Watchdog : public Thread {

public:
    Watchdog(DebugLog *debugLog, OgnLogger *ognLogger);
protected:
    void onStart() override;
    void onRun() override;
    void onStop() override;

private:
    OgnLogger *ognInstance;
    DebugLog *debugLog;
};


#endif //OGNDATALOGGER_WATCHDOG_H
