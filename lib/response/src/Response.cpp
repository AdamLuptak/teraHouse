//
// Created by adam on 7.12.2016.
//

#include "Response.h"

String Response::getResponse() {
    return response;
}

String Response::setResponse(String response) {
    return this->response = response;
}

String Response::getErrorMessage() {
    return errorMessage;
}

String Response::setErrorMessage(String message) {
    return this->errorMessage = message;
}

