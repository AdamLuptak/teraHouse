//
// Created by adam on 24.5.2017.
//

#ifndef CLION_ACTUATORMAPPER_H
#define CLION_ACTUATORMAPPER_H


#include <Actuator.h>
#include "ArduinoJson.h"

class ActuatorMapper {
public:
    Actuator jsonToActuator(JsonObject &jsonActuator);
    String actuatorToJsonString(Actuator actuator);
    JsonObject & actuatorToJson(Actuator actuator);

};


#endif //CLION_ACTUATORMAPPER_H
