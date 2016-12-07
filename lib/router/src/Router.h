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
#include "TimeLib.h"
#include "Time.h"
#include <UIPClient.h>
#include "Response.h"

class Router {
public:
    String route(String httpRequest, TerraController &terraController, UIPClient &client);

    void registerRoute(String *route);

    LinkedList<String *> &getRouteList();

    void setRouteList(LinkedList<String *> &routeList);

private:
    LinkedList<String *> routeList = LinkedList<String *>();
    HttpParser httpParser;

    String timeToJson();

    void updateTime();
};


#endif //CLION_ROUTER_H
