#include "OgnLogger.h"

#include "OgnLogger.h"
#include "LogPusher.h"
#include "DebugLog.h"


using namespace cpl::util::network ;


int main(int argc, char* argv[]) {

    if(argc<4){
        cout << "Too few arguments. Usage: ognDataLogger filters /data/storage/path/ server.address" << endl;
        exit(EXIT_FAILURE);
    }

#ifdef BACKGROUND_MODE
    pid_t pid, sid;

    // Fork off the parent process
    pid = fork();
    if (pid < 0) {
        // Log failure (use syslog if possible)
        exit(EXIT_FAILURE);
    }
    // If we got a good PID, then we can exit the parent process.
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    // Change the file mode mask
    umask(0);
#endif


    // Open any logs here
    DebugLog debugLog(string(argv[2])+"ognDataLogger.log");
#ifdef BACKGROUND_MODE
    // Create a new SID for the child process
    sid = setsid();
    if (sid < 0) {
        // Log any failure
        debugLog.write("Init", "Error creating child process SID");
        exit(EXIT_FAILURE);
    }

    debugLog.write("SID", to_string(sid).c_str());
    debugLog.write("PID", to_string(pid).c_str());
#endif

    // Change the current working directory
    if ((chdir("/")) < 0) {
        debugLog.write("Init", "Cannot change root directory");
        exit(EXIT_FAILURE);
    }

//#ifdef BACKGROUND_MODE
    // Close out the standard file descriptors
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
//#endif

    OgnLogger ognLogger(&debugLog, argv[2], argv[1]);
    LogPusher logPusher(&debugLog, &ognLogger, argv[3]);

    ognLogger.start();
    logPusher.start();

    // Exit at 22:00
    while( (uint8_t)Clock::getHour()<22 ){
        usleep(10000);
    }

    logPusher.stop();
    ognLogger.kill();
    debugLog.write("main()", "Shutdown. Bye bye :(");
    debugLog.finish();

    return 0;
}
