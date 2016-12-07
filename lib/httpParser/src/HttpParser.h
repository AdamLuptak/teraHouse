//
// Created by adam on 3.12.2016.
//

#ifndef CLION_HTTPPARSER_H
#define CLION_HTTPPARSER_H

#include <WString.h>
#include <USBAPI.h>

typedef enum REST{ // <-- the use of typedef is optional.
    POST,
    GET
};

class HttpParser {
public:
    String  parseEndpoint(String httpReuest);
    String parseBody(String httpReuest);

    void cleanRequest(String &httpRequest);

    String parseBodyMessage(String &httpReuest);

    boolean parseParameter(String httpRequest);
};


#endif //CLION_HTTPPARSER_H
