#include <iostream>
#include "LogPusher.h"

LogPusher::LogPusher(DebugLog *debugLog, OgnLogger *ognLogger, const char* server) {
    this->ognLogger= ognLogger;
    this->dataDir= ognLogger->getDataDir();
    this->serverAdr= server;
    this->debugLog= debugLog;
}

void LogPusher::onStart() {
    lastHour= 120;
    debugLog->write("LogPusher", "Start [ DONE ]");
}

void LogPusher::onRun() {
    uint8_t now= (uint8_t)Clock::getHour();

    if( lastHour!=now ) {

        debugLog->write("LogPusher", "Check data file [ ... ]");
        vector<string> fileLines;
        ognLogger->readDataFile(fileLines);
        debugLog->write("LogPusher", "Check data file [ DONE ]");

        if( fileLines.size() ) {

            ofstream sendFile;
            sendFile.open(dataDir + "sendLog.data");
            for (int i = 0; i < fileLines.size(); i++) {
                sendFile << fileLines[i];
                if (i != fileLines.size() - 1)
                    sendFile << endl;
            }
            sendFile.close();


            CURL *curlHandle = curl_easy_init();
            curl_httppost *post = NULL;
            curl_httppost *lastpost = NULL;

            // Create http header
            struct curl_slist *headerlist = NULL;
            headerlist = curl_slist_append(headerlist, "Content-Type: multipart/form-data");
            headerlist = curl_slist_append(headerlist, "Connection: close");

            // Prepare post form
            curl_formadd(&post, &lastpost,
                         CURLFORM_COPYNAME, "file",
                         CURLFORM_FILE, string(dataDir + "sendLog.data").c_str(),
                         CURLFORM_END);

            // Set curl opts
            curl_easy_setopt(curlHandle, CURLOPT_URL, serverAdr);
            curl_easy_setopt(curlHandle, CURLOPT_HTTPHEADER, headerlist);
            curl_easy_setopt(curlHandle, CURLOPT_HTTPPOST, post);
            curl_easy_setopt(curlHandle, CURLOPT_VERBOSE, 0);

            // Perform curl
            curl_easy_perform(curlHandle);
            curl_easy_cleanup(curlHandle);

            debugLog->write("LogPusher", "Sent to server");
        } else {
            debugLog->write("LogPusher", "Nothing to log");
        }

        lastHour= now;
    }


    Thread::pause(120000);
}

void LogPusher::onStop() {
    debugLog->write("LogPusher", "Stop [ DONE ]");
}
