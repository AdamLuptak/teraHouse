//
// Created by adam on 3.12.2016.
//

#ifndef CLION_ROUTER_H
#define CLION_ROUTER_H

static const char *const ACTUATOR_END = "actuator";

static const char *const SENSOR_END = "sensor";

static const char *const TIME_END = "time";

static const char *const MANUAL_END = "manualControl";

static const char *const ALL_END = "all/";

#include <WString.h>
#include <LinkedList.h>
#include "HttpParser.h"
#include "Arduino.h"
#include "ArduinoJson.h"
#include "TimeLib.h"
#include "Time.h"
#include <UIPClient.h>

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

    boolean updateTime(String httpRequest);
};


#endif //CLION_ROUTER_H
