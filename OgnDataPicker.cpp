#include "OgnDataPicker.h"

OgnDataPicker::OgnDataPicker(Logger *log, const char *dataDir, const char *filter) {
    this->log= log;
    this->filter= filter;
    this->dataDir= dataDir;

    log->write("OgnDataPicker", "Start logging...");
    dataMutex= PTHREAD_MUTEX_INITIALIZER;
    connectionMutex= PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_unlock(&dataMutex);
    pthread_mutex_unlock(&connectionMutex);
}


void OgnDataPicker::init() {
    pthread_mutex_lock(&connectionMutex);
    parser = new cpl::ogn::aprs_parser(std::clog, cpl::ogn::default_ddb_query_interval());

    c = cpl::ogn::connect(std::clog, DEFAULT_HOST, DEFAULT_SERVICE);
    is.reset(new instream(*c));
    keepalive.reset(new onstream(*c));
    cpl::ogn::login(std::cout, *keepalive, *is, "ognDataLogger v2.0.0", filter);
    lastKALog= Clock::sinceEpochM();

    utc_parsed= 0;

    pthread_mutex_lock(&dataMutex);
    dataStream.open(dataDir+"ognDataLogger.data", std::ofstream::app);
    pthread_mutex_unlock(&dataMutex);

    suspend= false;
    pthread_mutex_unlock(&connectionMutex);
}

void OgnDataPicker::exec() {
    if(!suspend) {
        pthread_mutex_lock(&connectionMutex);
        std::string line;
        std::getline(*is, line);
        pthread_mutex_unlock(&connectionMutex);

        if (line.empty()) {
            return;
        }

        if ('#' == line[0]) {
            if (keepalive) {
                *keepalive << "# " << KEEPALIVE_MESSAGE << std::endl;
                log->write("OgnDataPicker", "Keepalive [ OK ]");
                lastKALog = Clock::sinceEpochM();
            } else {
                log->write("OgnDataPicker", string("Keepalive [ ERROR ]: " + line).c_str());
            }
            return;
        }

        if (utc <= -2) {
            if (line.find("TIME ") == 0) {
                utc_parsed = cpl::util::parse_datetime(line.substr(5));
                log->write("OgnDataPicker", string("UTC Update [ OK ]: " + line).c_str());
                return;
            } else {
                log->write("OgnDataPicker", string("UTC Update [ ERROR ]: " + line).c_str());
                return;
            }
        }

        cpl::ogn::aircraft_rx_info_and_name acft;
        const double utc_now = utc > 0 ? utc : (utc <= -2 ? utc_parsed : cpl::util::utc());

        if (parser->parse_aprs_aircraft(line, acft, utc_now)) {

            if (acft.first.find("ogn") != std::string::npos && acft.second.mot.speed > 4) {
                std::stringstream ss;
                ss << acft.first.substr(4) << " " << static_cast<cpl::gnss::position_time const &>(acft.second.pta)
                   << " "
                   << acft.second.mot.course << " " << acft.second.mot.speed;

                pthread_mutex_lock(&dataMutex);
                dataStream << ss.str() << std::endl;
                pthread_mutex_unlock(&dataMutex);
            }

        } else {
            cpl::ogn::station_info_and_name stat;
            if (cpl::ogn::parse_aprs_station(line, stat, utc_now)) {

            } else {
                log->write("OgnDataPicker", string("# WARNING: Couldn't parse: " + line).c_str());
            }
        }
    }
}

void OgnDataPicker::finish() {
    pthread_mutex_lock(&dataMutex);
    dataStream.close();
    pthread_mutex_unlock(&dataMutex);

    log->write("OgnDataPicker", "Finish [ DONE ]");
}


std::string OgnDataPicker::getDataDir() {
    return dataDir;
}

void OgnDataPicker::readDataFile(std::vector<std::string> &fileLines) {
    pthread_mutex_lock(&dataMutex);

    std::string line;
    std::ifstream inDataFile;
    inDataFile.open(dataDir+"ognDataLogger.data");

    if(inDataFile.is_open()) {
        while (getline(inDataFile, line)) {
            fileLines.push_back(line);
        }

        inDataFile.close();
    }

    resetDataFileStream();
    pthread_mutex_unlock(&dataMutex);
}

void OgnDataPicker::resetDataFileStream() {
    if(dataStream.is_open()){
        dataStream.close();
    }

    dataStream.open(dataDir+"ognDataLogger.data");
}

int OgnDataPicker::getLastKeepaliveTime() {
    return lastKALog;
}

void OgnDataPicker::resetConnection() {
    suspend= true;
    this->init();
}
