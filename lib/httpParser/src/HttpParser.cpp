//
// Created by adam on 3.12.2016.
//

#include "HttpParser.h"

String HttpParser::parseEndpoint(String httpRequest) {
    int first = httpRequest.indexOf("GET /") + 5;
    int second = httpRequest.indexOf("/", (unsigned int) first) + 1;
    String string = httpRequest.substring((unsigned int) first, (unsigned int) httpRequest.indexOf("/",
                                                                                                 (unsigned int) second));
    string.replace(" ", "");
    return string;
}

String HttpParser::parseBody(String httpRequest) {

    return "sdfs";
}

void HttpParser::cleanRequest(String &httpRequest) {
    httpRequest.replace("%22", "\"");
    httpRequest.replace("%20", "");
    httpRequest.replace("%20", "");
    httpRequest.replace("%7D", "}");
    httpRequest.replace("%7B", "{");
    httpRequest.replace("HTTP", "");
}

String HttpParser::parseBodyMessage(String &httpRequest) {
    int start = httpRequest.indexOf("body=") + 5;
    return httpRequest.substring((unsigned int) start);
}

boolean HttpParser::getManualParam(String httpRequest) {
    String body = this->parseBodyMessage(httpRequest);
    StaticJsonBuffer<1500> jsonBuffer;
    const JsonObject &object = jsonBuffer.parse(body);

    return object["manual"];
}

