#include "LogPusher.h"

LogPusher::LogPusher(vector<string> *logQueue, pthread_mutex_t *queueMutex) {
    this->logQueue= logQueue;
    this->queueMutex= queueMutex;
}

void LogPusher::onStart() {

}

void LogPusher::onRun() {
    pthread_mutex_lock(queueMutex);
    pthread_mutex_unlock(queueMutex);

    Thread::pause(10);
}

void LogPusher::onStop() {

}
