#include "OgnDataPicker.h"

#include "OgnDataPicker.h"
#include "DBFeeder.h"
#include "Logger.h"
#include "Watchdog.h"


using namespace cpl::util::network ;


int main(int argc, char* argv[]) {

    if(argc<4){
        cout << "Too few arguments. Usage: ognDataLogger filters /data/storage/path/ server.address" << endl;
        exit(EXIT_FAILURE);
    }

    // Open any logs here
    Logger debugLog(string(argv[2]) + "ognDataLogger.log");

    // Change the current working directory
    if ((chdir("/")) < 0) {
        debugLog.write("Init", "Cannot change root directory");
        exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    OgnDataPicker ognLogger(&debugLog, argv[2], argv[1]);
    DBFeeder logPusher(&debugLog, &ognLogger, argv[3]);
    Watchdog  watchdog(&debugLog, &ognLogger);

    ognLogger.init();
    logPusher.start();
    watchdog.start();


    while( 1 ){
        ognLogger.exec();
    }

    logPusher.kill();
    ognLogger.finish();
    debugLog.write("main()", "Shutdown. Bye bye :(");
    debugLog.finish();

    return 0;
}
