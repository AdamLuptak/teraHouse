//
// Created by adam on 3.12.2016.
//

#include <TerraController.h>
#include "Router.h"

void Router::route(String httpRequest, TerraController &terraController) {
    httpParser.cleanRequest(httpRequest);

    int routeListSize = routeList.size();
    for (int i = 0; i < routeListSize; ++i) {
        String requestEndpoint = this->httpParser.parseEndpoint(httpRequest);

        if (*routeList.get(i) == requestEndpoint) {

            terraController.updateActuator(requestEndpoint, httpRequest);

        }
    }


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
