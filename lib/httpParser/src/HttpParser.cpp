//
// Created by adam on 3.12.2016.
//

#include "HttpParser.h"

String HttpParser::parseEndpoint(String httpReuest) {
    int first = httpReuest.indexOf("GET /") + 5;
    int second = httpReuest.indexOf("/", first) + 1;
    return httpReuest.substring(first, httpReuest.indexOf("/", second));
}

String HttpParser::parseBody(String httpReuest) {

    return "sdfs";
}

void HttpParser::cleanRequest(String &httpRequest) {
    httpRequest.replace("%22", "\"");
    httpRequest.replace("%20", "");
    httpRequest.replace("HTTP", "");
}

String HttpParser::parseBodyMessage(String &httpReuest) {
    int start = httpReuest.indexOf("body=") + 5;
    return httpReuest.substring(start);
}