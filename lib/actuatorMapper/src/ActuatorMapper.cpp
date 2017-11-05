//
// Created by adam on 24.5.2017.
//

#include "ActuatorMapper.h"


Actuator ActuatorMapper::jsonToActuator(JsonObject &jsonActuator) {
    return Actuator();
}

String ActuatorMapper::actuatorToJsonString(Actuator actuator) {
    JsonObject &jsonActuator = this->actuatorToJson(actuator);
    String jsonString;

    jsonActuator.printTo(jsonString);
    return jsonString;
}

JsonObject &ActuatorMapper::actuatorToJson(Actuator actuator) {
    StaticJsonBuffer<500> jsonBuffer;
    JsonObject &actuatorJson = jsonBuffer.createObject();
    actuatorJson["name"] = actuator.getName();
    actuatorJson["pin"] = actuator.getPin();
    actuatorJson["state"] = actuator.getPinState();
    JsonArray &intervals = jsonBuffer.createArray();
    for (int index = 0; index < sizeof(actuator.getActionTimes()); ++index) {
        JsonObject &interval = jsonBuffer.createObject();
        interval["actionTime"] = actuator.getActionTime(index);
        interval["duration"] = actuator.getDuration(index);
        intervals.add(interval);
    }
    actuatorJson["intervals"] = intervals;
    return actuatorJson;
}


