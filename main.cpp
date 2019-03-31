#include "OgnLogger.h"

#include "OgnLogger.h"
#include "LogPusher.h"


using namespace cpl::util::network ;


int main( int , char const* const* const argv ) {
    pid_t pid, sid;

    /* Fork off the parent process */
    pid = fork();
    if (pid < 0) {
        /* Log failure (use syslog if possible) */
        exit(EXIT_FAILURE);
    }
    /* If we got a good PID, then
       we can exit the parent process. */
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    /* Change the file mode mask */
    umask(0);

    /* Open any logs here */
    ofstream logFile;
    logFile.open("/home/dkulpa/ognLogger.log");
    if(!logFile.is_open()){
        exit(EXIT_FAILURE);
    }

    /* Create a new SID for the child process */
    sid = setsid();
    if (sid < 0) {
        /* Log any failure */
        logFile << "Error creating child process SID" << endl;
        exit(EXIT_FAILURE);
    }

    /* Change the current working directory */
    if ((chdir("/")) < 0) {
        /* Log any failure here */
        exit(EXIT_FAILURE);
    }

    /* Close out the standard file descriptors */
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    OgnLogger ognLogger(&logFile);
    LogPusher logPusher(ognLogger.getLogQueue(), ognLogger.getQueueMutex());

    ognLogger.start();

    while(ognLogger.isRunning()){

    }
}
