//
// Created by adam on 30.11.2016.
//

#include <DhtSensor.h>
#include <HttpParser.h>
#include "TerraController.h"

Actuator *
TerraController::changeActuator(int index, uint8_t pin, int startTime, float actionTimeArray[], float durationArray[]) {

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

void TerraController::update(int hour, int minute) {

    int actuatorSize = actuatorList.size();
    for (int i = 0; i < actuatorSize; ++i) {
        Actuator *pActuator = actuatorList.get(i);
        pActuator->update(hour, minute);
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

String TerraController::updateActuator(String &endpoint, String &httpRequest) {
    for (int j = 0; j < actuatorList.size(); ++j) {
        Actuator *pActuator = actuatorList.get(j);
        if (endpoint == pActuator->getEndpoint()) {
            String body = httpParser.parseBodyMessage(httpRequest);
            StaticJsonBuffer<1500> jsonBuffer;
            const JsonObject &json = jsonBuffer.parse(body);

            JsonObject &root = json[endpoint].asObject();

            uint8_t newPin = root["pin"];
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

            return pActuator->toJson();
        }
    }
    return "{ \"error\" : \"cant find actuator with this name\"}";
}

String TerraController::manualToJson() {
    char buff[200];
    sprintf(buff,
            "{\"manual\" : \"%d\" }",
            this->manual);
    return String(buff);
}

String TerraController::sensorToJson(String requestEndpoint, String httpRequest) {
    for (int j = 0; j < sensorList.size(); ++j) {
        Sensor *pSensor = sensorList.get(j);
        if (requestEndpoint == pSensor->getEndpoint()) {
            String body = httpParser.parseBodyMessage(httpRequest);
            return pSensor->toJson();
        }
    }
    return "{ \"error\" : \"cant find sesor with this name\"}";
}

String TerraController::actuatorListToJson(String &endpoint, String &httpRequest) {
    String actuatorLisJson = "";
    actuatorLisJson.concat("{[");
    int listSize = actuatorList.size();
    for (int j = 0; j < listSize; ++j) {
        Actuator *pActuator = actuatorList.get(j);
        actuatorLisJson.concat(pActuator->toJson());
        if (j < listSize - 1)
            actuatorLisJson.concat(",");
    }
    actuatorLisJson.concat("]}");
    return actuatorLisJson;
}

String TerraController::sensorListToJson(String &endpoint, String &httpRequest) {
    String sensorListJson = "";
    sensorListJson.concat("{[");
    int listSize = sensorList.size();
    for (int j = 0; j < listSize; ++j) {
        Sensor *pSensor = sensorList.get(j);
        sensorListJson.concat(pSensor->toJson());
        if (j < listSize - 1)
            sensorListJson.concat(",");
    }
    sensorListJson.concat("]}");
    return sensorListJson;
}
