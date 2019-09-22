#ifndef OGNDATALOGGER_OGNLOGGER_H
#define OGNDATALOGGER_OGNLOGGER_H

#include <algorithm>
#include <exception>
#include <iostream>
#include <iterator>
#include <memory>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <stdio.h>

#include <cassert>

#include "cpp-lib/gnss.h"
#include "cpp-lib/ogn.h"
#include "cpp-lib/util.h"
#include "DebugLog.h"
#include "dkulpaclibs/misc/Thread.h"

using namespace cpl::util::network;

class OgnLogger{
public:
    OgnLogger(DebugLog *debugLog, const char *dataDir, const char *filter);
    std::string getDataDir();
    void readDataFile(std::vector<std::string> &fileLines);
    void resetDataFileStream();
    void init();
    void exec();
    void finish();

private:
    cpl::ogn::aprs_parser *parser;
    std::istream *is;
    DebugLog *debugLog;
    std::ofstream dataStream;
    double const utc=-1;
    std::ostream* keepalive;
    cpl::ogn::aircraft_db acdb;
    double utc_parsed;
    std::string filter;
    std::string dataDir;

    pthread_mutex_t dataMutex;

    const std::string DEFAULT_HOST    = "aprs.glidernet.org";
    const std::string DEFAULT_SERVICE = "14580"             ;

// Keepalive message for OGN server, sent each time we receive one
    const std::string KEEPALIVE_MESSAGE = "We are alive";
};


#endif //OGNDATALOGGER_OGNLOGGER_H
