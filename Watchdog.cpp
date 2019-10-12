//
// Created by dkulpa on 12.10.19.
//

#include "Watchdog.h"

Watchdog::Watchdog(DebugLog *debugLog, OgnLogger *ognLogger) {
    ognInstance= ognLogger;
    this->debugLog= debugLog;
}

void Watchdog::onStart() {

}

void Watchdog::onRun() {
    if(ognInstance->getLastKeepaliveTime()+2<Clock::GetDayMinutes()){
        debugLog->write("Watchdog", "OGN connection is DEAD. Trying to reconnect");
        //ognInstance->resetConnection();
        Thread::pause(120000); // Wait 5min for connection to establish
    }
    Thread::pause(1000);
}

void Watchdog::onStop() {

}
