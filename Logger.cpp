#include "Logger.h"

Logger::Logger(string debugFileName) {
    streamMutex= PTHREAD_MUTEX_INITIALIZER;

    // Open any logs here
    debugStream.open(string(debugFileName));
    if(!debugStream.is_open()){
        exit(EXIT_FAILURE);
    }

}

void Logger::write(const char *tag, const char *msg) {
    pthread_mutex_lock(&streamMutex);
    char ts[16];
    sprintf(ts, "%02d-%02d-%04d %02d:%02d", Clock::nowDay(), Clock::nowMonth(),
            Clock::nowYear(), Clock::nowHour(), Clock::nowMinutes());
    debugStream << ts << ", " << tag << ", " << msg << endl;
    pthread_mutex_unlock(&streamMutex);
}

void Logger::finish() {
    pthread_mutex_lock(&streamMutex);
    debugStream.close();
    pthread_mutex_unlock(&streamMutex);
}
