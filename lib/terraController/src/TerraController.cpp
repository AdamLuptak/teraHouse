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

void TerraController::update(int hour, int minute, int second) {
    if (!manual) {
        int actuatorSize = actuatorList.size();
        for (int i = 0; i < actuatorSize; ++i) {
            Actuator *pActuator = actuatorList.get(i);
            pActuator->update(hour, minute, second);
        }

        int sensorListSize = sensorList.size();
        for (int i = 0; i < sensorListSize; ++i) {
            Sensor *pSensor = sensorList.get(i);
            pSensor->getMeasurement();
            Serial.println(pSensor->toString());

        }
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
    String errorMessage = "{\"error\" : \"";

    for (int j = 0; j < actuatorList.size(); ++j) {
        Actuator *pActuator = actuatorList.get(j);
        String body = httpParser.parseBodyMessage(httpRequest);
        StaticJsonBuffer<1500> jsonBuffer;
        const JsonObject &json = jsonBuffer.parse(body);
        if (endpoint == pActuator->getEndpoint()) {

            if (json.containsKey("state") && !manual) {
                return "{ \"error\" : \"you are not in manual mod\"}";
            }

            if (manual) {
                boolean state = json["state"];
                if (state) {
                    pActuator->turnOn();
                } else {
                    pActuator->turnOff();
                }
                return pActuator->toJson();

            } else {
                JsonObject &root = json[endpoint].asObject();

                uint8_t newPin = root["pin"];

                if (newPin != 0 && isPinAvialable(newPin) && newPin >= startPin && newPin <= endPin) {
                    pActuator->setPin(newPin);
                } else {
                    errorMessage.concat(" pin ");
                    errorMessage.concat(newPin);
                    errorMessage.concat(" is in use or not in interval[8-12], ");
                }
                int newStartTime = root["startTime"];
                if (newStartTime <= 24 && newStartTime > 0) {
                    pActuator->setStartTime(newStartTime);
                } else {
                    errorMessage.concat(" startTime ");
                    errorMessage.concat(newStartTime);
                    errorMessage.concat(" out off interval [1 - 24], ");
                }

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
                    float actionTime = interval["actionTime"];
                    float duration = interval["duration"];
                    if (actionTime > 24) {
                        errorMessage.concat(" on of action Time out of interval [0 - 24], ");
                    } else {
                        pActuator->setActionTime(k, actionTime);
                    }
                    if (duration > 24) {
                        errorMessage.concat(" one of duration out of interval [0 - 24], ");
                    } else {
                        pActuator->setDuration(k, duration);
                    }
                }

                if (errorMessage.length() <= 12) {
                    return pActuator->toJson();
                } else {
                    errorMessage.concat("\"}");
                    return errorMessage;
                };
            }
        }

    }
    return "{ \"error\" : \"cant find actuator with this name\"}";
}

boolean TerraController::isPinAvialable(uint8_t pin) {
    for (int j = 0; j < actuatorList.size(); ++j) {
        Actuator *pActuator = actuatorList.get(j);
        if (pActuator->getPin() == pin)
            return false;
    }
    return true;
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
    sensorListJson.concat("[");
    int listSize = sensorList.size();
    for (int j = 0; j < listSize; ++j) {
        Sensor *pSensor = sensorList.get(j);
        sensorListJson.concat(pSensor->toJson());
        if (j < listSize - 1)
            sensorListJson.concat(",");
    }
    sensorListJson.concat("]");
    return sensorListJson;
}

void TerraController::createTC(TerraController terraController) {
    EEPROM.put(eeTerraControllerIndex, terraController);
}

void TerraController::updateTC(TerraController terraController) {
    EEPROM.put(eeTerraControllerIndex, terraController);
}

TerraController TerraController::read(TerraController &terraController) {
    return EEPROM.get(50,terraController);
}
