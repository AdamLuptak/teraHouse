//
// Created by adam on 30.11.2016.
//

#include <DhtSensor.h>
#include <HttpParser.h>
#include "TerraController.h"

Actuator *
TerraController::changeActuator(int index, int pin, int startTime, float actionTimeArray[], float durationArray[]) {

    Actuator *pActuator = actuatorList.get(index);

    pActuator->setPin(pin);
    pActuator->setStartTime(startTime);

    for (int i = 0; i < 5; ++i) {
        pActuator->setActionTime(i, actionTimeArray[i]);
        pActuator->setDuration(i, durationArray[i]);
    }

    return pActuator;
}

LinkedList<Sensor *> &TerraController::getSensorList() {
    return sensorList;
}

LinkedList<Actuator *> &TerraController::getActuatorList() {
    return actuatorList;
}

void TerraController::toString() {
    Serial.println("TerraController: ");
    int atuatorSize = actuatorList.size();
    for (int i = 0; i < atuatorSize; ++i) {
        Actuator *pActuator = actuatorList.get(i);
        Serial.print("Actuator: ");
        Serial.print(i);
        Serial.print(",");
        Serial.println(pActuator->toString());
    }

    int sensorListSize = sensorList.size();
    for (int i = 0; i < sensorListSize; ++i) {
        Sensor *pSensor = sensorList.get(i);
        Serial.println(pSensor->toString());
    }

    Serial.println("");
}

void TerraController::registerActuator(Actuator *actuator) {
    actuatorList.add(actuator);

}

void TerraController::registerSensor(Sensor *sensor) {
    sensorList.add(sensor);
}

String TerraController::actuatorToJson(int index) {
    return adapter.actuatorToJson(actuatorList.get(index));
}

void TerraController::update(int hour, int minute, int second) {

    int atuatorSize = actuatorList.size();
    for (int i = 0; i < atuatorSize; ++i) {
        Actuator *pActuator = actuatorList.get(i);
        pActuator->update(hour, minute, second);
    }

    int sensorListSize = sensorList.size();
    for (int i = 0; i < sensorListSize; ++i) {
        Sensor *pSensor = sensorList.get(i);
        pSensor->getMeasurement();
    }
}

String TerraController::toJson() {
    String json("{\"TerraController\":{");
    int actuatorSize = actuatorList.size();
    for (int i = 0; i < actuatorSize; ++i) {
        Actuator *pActuator = actuatorList.get(i);
        json.concat("\"");
        json.concat(pActuator->getEndpoint());
        json.concat("\":");
        json.concat(pActuator->toJson());
        json.concat(",");
    }

    int sensorListSize = sensorList.size();
    for (int i = 0; i < sensorListSize; ++i) {
        json.concat("\"Sensor");
        json.concat(i);
        json.concat("\":");
        Sensor *pSensor = sensorList.get(i);
        json.concat(pSensor->toJson());
        if (i != sensorListSize - 1)
            json.concat(",");
    }
    json.concat("}}\n");

    return json;
}

boolean TerraController::getManual() {
    return manual;
}

void TerraController::setManual(boolean manual) {
    TerraController::manual = manual;
}

void TerraController::updateActuator(String &endpoint, String &httpRequest) {
    for (int j = 0; j < actuatorList.size(); ++j) {
        Actuator *pActuator = actuatorList.get(j);
        if (endpoint == pActuator->getEndpoint()) {
            String body = httpParser.parseBodyMessage(httpRequest);
            StaticJsonBuffer<1500> jsonBuffer;
            const JsonObject &json = jsonBuffer.parse(body);

            JsonObject &root = json[endpoint].asObject();

            int newPin = root["pin"];
            if (newPin != 0)
                pActuator->setPin(newPin);

            int newStartTime = root["startTime"];
            if (newStartTime != 0)
                pActuator->setStartTime(newStartTime);

            boolean state = (boolean) root["state"];
            if (this->getManual()) {
                if (state) {
                    pActuator->turnOn();
                } else {
                    pActuator->turnOff();
                }
            }

            JsonArray &intervals = root["intervals"].asArray();
            for (int k = 0; k < intervals.size(); ++k) {
                JsonObject &interval = intervals[k].asObject();
                pActuator->setActionTime(k, interval["actionTime"]);
                pActuator->setDuration(k, interval["duration"]);
            }
        }
    }
}
