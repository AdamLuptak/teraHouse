//
// Created by adam on 3.12.2016.
//

#include <TerraController.h>
#include "Router.h"


String Router::route(String httpRequest, TerraController &terraController, UIPClient &client) {
    httpParser.cleanRequest(httpRequest);

    String response = "";

    int routeListSize = routeList.size();
    for (int i = 0; i < routeListSize; ++i) {
        String requestEndpoint = this->httpParser.parseEndpoint(httpRequest);

        if (*routeList.get(i) == requestEndpoint) {
            Serial.println(requestEndpoint);

            if (requestEndpoint.startsWith(ACTUATOR_END)) {
                if (requestEndpoint.length() > 9) {
                    response = terraController.updateActuator(requestEndpoint, httpRequest);
                } else {
                    response = terraController.actuatorListToJson(requestEndpoint, httpRequest);
                }
                break;
            } else if (requestEndpoint.startsWith(SENSOR_END)) {
                if (requestEndpoint.length() > 7) {
                    response = terraController.sensorToJson(requestEndpoint, httpRequest);
                } else {
                    response = terraController.sensorListToJson(requestEndpoint, httpRequest);
                }
                break;

            } else if (requestEndpoint.indexOf(TIME_END) > 0) {
                this->updateTime(httpRequest);
                response = this->timeToJson();
                break;
            } else if (requestEndpoint.indexOf(MANUAL_END) > 0) {
                terraController.setManual(httpParser.getManualParam(httpRequest));
                response = terraController.manualToJson();
                break;

            } else if (requestEndpoint.startsWith(ALL_END)) {
                response = terraController.toJson();
                break;
            }

        } else {
            response = "{\"error\" : \"unexpected error\"}";
        }
    }
    return response;
}

void Router::registerRoute(String *route) {
    routeList.add(route);

}

LinkedList<String *> &Router::getRouteList() {
    return routeList;
}

void Router::setRouteList(LinkedList<String *> &routeList) {
    Router::routeList = routeList;
}

String Router::timeToJson() {
    char buff[200];
    sprintf(buff,
            "{\"time\" : {\"hour\" : %d, \"minute\" : %d, \"second\" : %d, \"day\" : %d, \"month\" : %d ,\"year\" : %d}}",
            hour(), minute(), second(), day(), month(), year());
    return String(buff);
}

void Router::updateTime(String httpRequest) {
    Serial.println(httpRequest);

    String body = httpParser.parseBodyMessage(httpRequest);
    StaticJsonBuffer<1500> jsonBuffer;
    const JsonObject &objectTime = jsonBuffer.parse(body);
    Serial.println(body);

    int hour = objectTime["hour"];
    int minute = objectTime["minute"];
    int second = objectTime["second"];
    int day = objectTime["day"];
    int month = objectTime["month"];
    int year = objectTime["year"];

    objectTime.prettyPrintTo(Serial);
    Serial.println(hour);

    setTime(hour, minute, second, day, month, year);
}
