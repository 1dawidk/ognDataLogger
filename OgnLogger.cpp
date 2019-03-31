#include "OgnLogger.h"

OgnLogger::OgnLogger(std::ostream *os) {
    this->os= os;
}

std::vector<std::string> *OgnLogger::getLogQueue() {
    return &logQueue;
}

pthread_mutex_t *OgnLogger::getQueueMutex() {
    return &queueMutex;
}

void OgnLogger::onStart() {
    std::unique_ptr<connection> c;

    double const ddb_query_interval = cpl::ogn::default_ddb_query_interval();

    parser= new cpl::ogn::aprs_parser(std::clog, ddb_query_interval);

    c = cpl::ogn::connect(std::clog, DEFAULT_HOST, DEFAULT_SERVICE);
    is= new instream(*c);
    keepalive = new onstream(*c);

    cpl::ogn::login(
            std::clog, *keepalive, *is, "ogn-test v1.20", "/52.213495/21.016965/100");

    utc_parsed = 0;
}

void OgnLogger::onRun() {
    std::string line;
    std::getline(*is, line);

    if (line.empty()) {
        return;
    }

    if ('#' == line[0]) {
        (*os) << "KEEPALIVE " << line.substr(2, std::string::npos) << std::endl;

        if (keepalive) {
            *keepalive << "# " << KEEPALIVE_MESSAGE << std::endl;
        }
        return;
    }
    if (utc <= -2) {
        if (line.find("TIME ") == 0) {
            utc_parsed = cpl::util::parse_datetime(line.substr(5));
            return;
        }
    }

    cpl::ogn::aircraft_rx_info_and_name acft;
    const double utc_now = utc > 0 ? utc :
                           (utc <= -2 ? utc_parsed : cpl::util::utc());
    if (parser->parse_aprs_aircraft(line, acft, utc_now)) {
        // Previous aircraft with same info
        auto const it = acdb.find(acft.first);
        cpl::ogn::aircraft_rx_info const* const pprev = acdb.end() == it ? NULL : &it->second;

        (*os) << "AIRCRAFT " << acft.first << " | " << acft.second << std::endl;

        std::stringstream ss;
        ss << acft.first << " " << acft.second;

        pthread_mutex_lock(&queueMutex);
        logQueue.push_back(ss.str());
        pthread_mutex_unlock(&queueMutex);

    } else {
        cpl::ogn::station_info_and_name stat;
        if (cpl::ogn::parse_aprs_station(line, stat, utc_now)) {

        } else {
            (*os) << "# WARNING: Couldn't parse: " << line << std::endl;
        }
    }
}

void OgnLogger::onStop() {

}
