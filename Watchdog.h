//
// Created by dkulpa on 12.10.19.
//

#ifndef OGNDATALOGGER_WATCHDOG_H
#define OGNDATALOGGER_WATCHDOG_H


#include <dkulpaclibs/misc/Thread.h>
#include "OgnDataPicker.h"

class Watchdog : public Thread {

public:
    Watchdog(Logger *debugLog, OgnDataPicker *ognLogger);
protected:
    void onStart() override;
    void onRun() override;
    void onStop() override;

private:
    OgnDataPicker *ognInstance;
    Logger *debugLog;
};


#endif //OGNDATALOGGER_WATCHDOG_H
