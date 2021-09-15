//
// Created by dkulpa on 12.10.19.
//

#include "Watchdog.h"

Watchdog::Watchdog(Logger *debugLog, OgnDataPicker *ognLogger) {
    ognInstance= ognLogger;
    this->debugLog= debugLog;
}

void Watchdog::onStart() {

}

void Watchdog::onRun() {
    int l= ognInstance->getLastKeepaliveTime()+40;
    long now= Clock::sinceEpochS();
    if(l<now){
        debugLog->write("Watchdog", "OGN connection is DEAD. Trying to reconnect");
        ognInstance->resetConnection();
        Thread::pause(120000); // Wait 5min for connection to establish
    }
    Thread::pause(1000);
}

void Watchdog::onStop() {

}
