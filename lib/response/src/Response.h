//
// Created by adam on 7.12.2016.
//

#ifndef CLION_RESPONSE_H
#define CLION_RESPONSE_H


#include <WString.h>

class Response {
public:
    String getResponse();
    String setResponse(String response);
    String getErrorMessage();
    String setErrorMessage(String message);
private:
    String response = "";
    String errorMessage = "";
};


#endif //CLION_RESPONSE_H
