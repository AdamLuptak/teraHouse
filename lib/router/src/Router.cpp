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

            if (requestEndpoint.startsWith("actuator")) {
                if (requestEndpoint.length() > 9) {
                    response = terraController.updateActuator(requestEndpoint, httpRequest);
                } else {
                    response = terraController.actuatorListToJson(requestEndpoint, httpRequest);
                }
                break;
            } else if (requestEndpoint.startsWith("sensor")) {
                if (requestEndpoint.length() > 7) {
                    response = terraController.sensorToJson(requestEndpoint, httpRequest);
                } else {
                    response = terraController.sensorListToJson(requestEndpoint, httpRequest);
                }
                break;

            } else if (requestEndpoint.indexOf("time") > 0) {
                this->updateTime();
                response = this->timeToJson();
            } else if (requestEndpoint.indexOf("manualControl") > 0) {
                terraController.setManual(httpParser.parseParameter(httpRequest));
                response = terraController.manualToJson(requestEndpoint, httpRequest);
            } else if (requestEndpoint.indexOf("all") > 0) {
                response = terraController.toJson();
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
    return String();
}

void Router::updateTime() {

}
