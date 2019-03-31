#ifndef OGNDATALOGGER_LOGPUSHER_H
#define OGNDATALOGGER_LOGPUSHER_H

#include <vector>
#include <string>
#include "dkulpaclibs/misc/Thread.h"

using namespace std;

class LogPusher : public Thread {
public:
    LogPusher(vector<string> *logQueue, pthread_mutex_t *queueMutex);

protected:
    void onStart() override;
    void onRun() override;
    void onStop() override;

private:
    vector<string> *logQueue;
    pthread_mutex_t *queueMutex;
};


#endif //OGNDATALOGGER_LOGPUSHER_H
