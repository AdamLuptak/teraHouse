//
// Created by adam on 3.12.2016.
//

#ifndef CLION_HTTPPARSER_H
#define CLION_HTTPPARSER_H

#include <WString.h>
#include <USBAPI.h>
#include "ArduinoJson.h"

class HttpParser {
public:
    String  parseEndpoint(String httpRequest);
    String parseBody(String httpRequest);

    void cleanRequest(String &httpRequest);

    String parseBodyMessage(String &httpRequest);

    boolean getManualParam(String httpRequest);
};


#endif //CLION_HTTPPARSER_H
