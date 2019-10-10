#include "OgnLogger.h"

OgnLogger::OgnLogger(DebugLog *debugLog, const char *dataDir, const char *filter) {
    this->debugLog= debugLog;
    this->filter= filter;
    this->dataDir= dataDir;


    debugLog->write("OgnLogger", "Start logging...");
    dataMutex= PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_unlock(&dataMutex);
}


void OgnLogger::init() {
    parser = new cpl::ogn::aprs_parser(std::clog, cpl::ogn::default_ddb_query_interval());

    c = cpl::ogn::connect(std::clog, DEFAULT_HOST, DEFAULT_SERVICE);
    is.reset(new instream(*c));
    keepalive.reset(new onstream(*c));
    cpl::ogn::login(std::clog, *keepalive, *is, "ognDataLogger v2.0.0", filter);
    lastKALog= Clock::GetDayMinutes();


    utc_parsed= 0;

    pthread_mutex_lock(&dataMutex);
    dataStream.open(dataDir+"ognDataLogger.data");
    pthread_mutex_unlock(&dataMutex);
}

void OgnLogger::exec() {
    std::string line;
    std::getline(*is, line);

    if (line.empty()) {
        return;
    }

    if ('#' == line[0]) {
        if (keepalive) {
            *keepalive << "# " << KEEPALIVE_MESSAGE << std::endl;
            if(lastKALog+60<Clock::GetDayMinutes()) {
                debugLog->write("OgnLogger", string("Keepalive [ OK ]: " + line).c_str());
                lastKALog= Clock::GetDayMinutes();
            }
        } else {
            debugLog->write("OgnLogger", string("Keepalive [ ERROR ]: "+line).c_str());
        }
        return;
    }

    if (utc <= -2) {
        if (line.find("TIME ") == 0) {
            utc_parsed = cpl::util::parse_datetime(line.substr(5));
            debugLog->write("OgnLogger", string("UTC Update [ OK ]: "+line).c_str());
            return;
        } else {
            debugLog->write("OgnLogger", string("UTC Update [ ERROR ]: "+line).c_str());
            return;
        }
    }

    cpl::ogn::aircraft_rx_info_and_name acft;
    const double utc_now = utc > 0 ? utc : (utc <= -2 ? utc_parsed : cpl::util::utc());

    if (parser->parse_aprs_aircraft(line, acft, utc_now)) {

        if(acft.first.find("ogn")!=std::string::npos && acft.second.mot.speed>4) {
            std::stringstream ss;
            ss << acft.first.substr(4) << " " << static_cast<cpl::gnss::position_time const&>(acft.second.pta) << " "
               << acft.second.mot.course << " " << acft.second.mot.speed;

            pthread_mutex_lock(&dataMutex);
            dataStream << ss.str() << std::endl;
            pthread_mutex_unlock(&dataMutex);
        }

    } else {
        cpl::ogn::station_info_and_name stat;
        if (cpl::ogn::parse_aprs_station(line, stat, utc_now)) {

        } else {
            debugLog->write("OgnLogger", string("# WARNING: Couldn't parse: " + line).c_str());
        }
    }
}

void OgnLogger::finish() {
    pthread_mutex_lock(&dataMutex);
    dataStream.close();
    pthread_mutex_unlock(&dataMutex);

    debugLog->write("OgnLogger", "Finish [ DONE ]");
}


std::string OgnLogger::getDataDir() {
    return dataDir;
}

void OgnLogger::readDataFile(std::vector<std::string> &fileLines) {
    pthread_mutex_lock(&dataMutex);

    std::string line;
    std::ifstream inDataFile;
    inDataFile.open(dataDir+"ognDataLogger.data");

    while ( getline (inDataFile,line) )
    {
        fileLines.push_back(line);
    }

    inDataFile.close();

    resetDataFileStream();
    pthread_mutex_unlock(&dataMutex);
}

void OgnLogger::resetDataFileStream() {
    if(dataStream.is_open()){
        dataStream.close();
    }

    dataStream.open(dataDir+"ognDataLogger.data");
}