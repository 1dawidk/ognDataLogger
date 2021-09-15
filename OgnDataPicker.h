#ifndef OGNDATALOGGER_OGNDATAPICKER_H
#define OGNDATALOGGER_OGNDATAPICKER_H

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
#include "dkulpaclibs/misc/Logger.h"
#include "dkulpaclibs/misc/Thread.h"

using namespace cpl::util::network;

class OgnDataPicker{
public:
    OgnDataPicker(Logger *log, const char *dataDir, const char *filter);
    std::string getDataDir();
    void readDataFile(std::vector<std::string> &fileLines);
    void resetDataFileStream();
    int getLastKeepaliveTime();
    void resetConnection();
    void init();
    void exec();
    void finish();

private:
    // OGN connection vars
    std::unique_ptr<connection> c;
    std::unique_ptr<std::istream> is;
    std::unique_ptr<std::ostream> keepalive;

    double const utc=-1;
    double utc_parsed;
    volatile long lastKALog;
    cpl::ogn::aprs_parser *parser;

    Logger *log;
    std::ofstream dataStream;

    std::string filter;
    std::string dataDir;

    pthread_mutex_t dataMutex;
    pthread_mutex_t connectionMutex;

    bool suspend;
    bool suspended;

    const std::string DEFAULT_HOST    = "aprs.glidernet.org";
    const std::string DEFAULT_SERVICE = "14580"             ;

// Keepalive message for OGN server, sent each time we receive one
    const std::string KEEPALIVE_MESSAGE = "We are alive";
};


#endif //OGNDATALOGGER_OGNDATAPICKER_H
