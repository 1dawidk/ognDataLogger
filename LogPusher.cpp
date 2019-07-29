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
}

void LogPusher::onRun() {
    uint8_t now= (uint8_t)Clock::getHour();

    if(lastHour!=now /*&& (now==10 || now==22)*/ ) {

        vector<string> fileLines;
        ognLogger->readDataFile(fileLines);

        ofstream sendFile;
        sendFile.open(dataDir+"sendLog.data");
        for(int i=0; i<fileLines.size(); i++){
            sendFile << fileLines[i];
            if(i!=fileLines.size()-1)
                sendFile << endl;
        }
        sendFile.close();


        CURL *curlHandle = curl_easy_init();
        curl_httppost *post=NULL;
        curl_httppost *lastpost=NULL;

        // Create http header
        struct curl_slist *headerlist = NULL;
        headerlist = curl_slist_append(headerlist, "Content-Type: multipart/form-data");
        headerlist = curl_slist_append(headerlist, "Connection: close");

        // Prepare post form
        curl_formadd(&post, &lastpost,
                     CURLFORM_COPYNAME, "file",
                     CURLFORM_FILE, string(dataDir+"sendLog.data").c_str(),
                     CURLFORM_END);


        // Set curl opts
        curl_easy_setopt(curlHandle, CURLOPT_URL, serverAdr);
        curl_easy_setopt(curlHandle, CURLOPT_HTTPHEADER, headerlist);
        curl_easy_setopt(curlHandle, CURLOPT_HTTPPOST, post);
#ifdef BACKGROUND_MODE
        curl_easy_setopt(curlHandle, CURLOPT_VERBOSE, 0);
#else
        curl_easy_setopt(curlHandle, CURLOPT_VERBOSE, 1);
#endif

        // Perform curl
        curl_easy_perform(curlHandle);
        //curl_easy_cleanup(curlHandle);

        debugLog->write("LogPusher", "Sent to server");

        lastHour= now;
    }


    Thread::pause(120000);
}

void LogPusher::onStop() {

}
