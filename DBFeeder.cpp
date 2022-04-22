#include <iostream>
#include "DBFeeder.h"

DBFeeder::DBFeeder(Logger *debugLog, OgnDataPicker *ognLogger, const char* server) {
    this->ognLogger= ognLogger;
    this->dataDir= ognLogger->getDataDir();
    this->serverAdr= server;
    this->debugLog= debugLog;
}

void DBFeeder::onStart() {
    lastHour= 120;
    debugLog->write("DBFeeder", "Start [ DONE ]");
    linesToday=0;
}

void DBFeeder::onRun() {
    uint8_t now= (uint8_t)Clock::nowHour();

    if( lastHour!=now ) {
        vector<string> fileLines;
        ognLogger->readDataFile(fileLines);

        if( fileLines.size() ) {

            ofstream sendFile;
            ofstream backupFile;
            sendFile.open(dataDir + "sendLog.data");
            backupFile.open(dataDir+"ognData.backup", ios::app);
            for (int i = 0; i < fileLines.size(); i++) {
                sendFile << fileLines[i];
                backupFile << fileLines[i] << endl;
                if (i != fileLines.size() - 1)
                    sendFile << endl;
            }
            sendFile.close();
            backupFile.close();

            CURL *curlHandle = curl_easy_init();
            curl_httppost *post = NULL;
            curl_httppost *lastpost = NULL;
            std::string readBuffer;

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

            long response_code;
            curl_easy_getinfo(curlHandle, CURLINFO_RESPONSE_CODE, &response_code);

            curl_easy_cleanup(curlHandle);

            if(now) {
                linesToday+= fileLines.size();
            } else {
                linesToday= fileLines.size();
            }

            char msg[256];
            if(response_code==200){
                sprintf(msg, "Sent to server [ %d lines today ]", linesToday);
                debugLog->write("DBFeeder", msg);
            } else {
                sprintf(msg, "Server error! Responded with %ld", response_code);
                debugLog->write("DBFeeder", msg);
            }
        } else {
            char msg[256];
            sprintf(msg, "Nothing to log [ %d lines today ]", linesToday);
            debugLog->write("DBFeeder", msg);
        }

        lastHour= now;
    }


    Thread::pause(120000);
}

void DBFeeder::onStop() {
    debugLog->write("DBFeeder", "Stop [ DONE ]");
}
