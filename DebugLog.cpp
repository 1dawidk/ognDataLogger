#include "DebugLog.h"

DebugLog::DebugLog(string debugFileName) {
    streamMutex= PTHREAD_MUTEX_INITIALIZER;

    // Open any logs here
    debugStream.open(string(debugFileName));
    if(!debugStream.is_open()){
        exit(EXIT_FAILURE);
    }
}

void DebugLog::write(const char *tag, const char *msg) {
    pthread_mutex_lock(&streamMutex);
    debugStream << tag << ": " << msg << endl;
    pthread_mutex_unlock(&streamMutex);
}
