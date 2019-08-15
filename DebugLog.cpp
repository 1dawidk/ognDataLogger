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
    char ts[16];
    sprintf(ts, "%02d-%02d-%04d %02d:%02d", Clock::getDay(), Clock::getMonth(),
            Clock::getYear(), Clock::getHour(), Clock::getMinutes());
    debugStream << ts << " | " << tag << ": " << msg << endl;
    pthread_mutex_unlock(&streamMutex);
}

void DebugLog::finish() {
    pthread_mutex_lock(&streamMutex);
    debugStream.close();
    pthread_mutex_unlock(&streamMutex);
}
