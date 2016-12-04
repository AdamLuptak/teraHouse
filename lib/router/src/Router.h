//
// Created by adam on 3.12.2016.
//

#ifndef CLION_ROUTER_H
#define CLION_ROUTER_H

#include <WString.h>
#include <LinkedList.h>
#include "HttpParser.h"
#include "Arduino.h"
#include "ArduinoJson.h"

class Router {
public:
    void route(String httpRequest, TerraController &terraController);

    void registerRoute(String *route);

    LinkedList<String *> &getRouteList();

    void setRouteList(LinkedList<String *> &routeList);

private:
    LinkedList<String *> routeList = LinkedList<String *>();
    HttpParser httpParser;
};


#endif //CLION_ROUTER_H
