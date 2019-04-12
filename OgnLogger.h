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

using namespace cpl::util::network;

class OgnLogger {
public:
    OgnLogger(std::ostream *logStream, const char *dataDir, const char *filter);
    pthread_mutex_t* getDataMutex();
    std::string getDataDir();
    void readDataFile(std::vector<std::string> &fileLines);

    void init();
    void exec();
    void resetDataFileStream();

private:
    cpl::ogn::aprs_parser *parser;
    std::istream *is;
    std::ostream *logStream;
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
