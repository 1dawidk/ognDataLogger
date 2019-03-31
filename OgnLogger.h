#ifndef OGNDATALOGGER_OGNLOGGER_H
#define OGNDATALOGGER_OGNLOGGER_H

#include "dkulpaclibs/misc/Thread.h"
#include <algorithm>
#include <exception>
#include <iostream>
#include <iterator>
#include <memory>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>

#include <cassert>

#include "cpp-lib/sys/network.h"
#include "cpp-lib/command_line.h"
#include "cpp-lib/container-util.h"
#include "cpp-lib/gnss.h"
#include "cpp-lib/ogn.h"
#include "cpp-lib/util.h"

using namespace cpl::util::network;

class OgnLogger : public Thread {
public:
    OgnLogger(std::ostream *os);
    std::vector<std::string>* getLogQueue();
    pthread_mutex_t* getQueueMutex();
protected:
    void onStart() override;
    void onRun() override;
    void onStop() override;

private:
    cpl::ogn::aprs_parser *parser;
    std::istream *is;
    std::ostream *os;
    double const utc=-1;
    std::ostream* keepalive;
    cpl::ogn::aircraft_db acdb;
    double utc_parsed;
    std::vector<std::string> logQueue;

    pthread_mutex_t queueMutex;

    const std::string DEFAULT_HOST    = "aprs.glidernet.org";
    const std::string DEFAULT_SERVICE = "14580"             ;

// Keepalive message for OGN server, sent each time we receive one
    const std::string KEEPALIVE_MESSAGE = "We are alive";
};


#endif //OGNDATALOGGER_OGNLOGGER_H
