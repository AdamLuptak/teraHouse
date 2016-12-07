//
// Created by adam on 30.11.2016.
//

#ifndef CLION_TERRACONTROLLER_H
#define CLION_TERRACONTROLLER_H


#include <Actuator.h>
#include <LinkedList.h>
#include <Sensor.h>
#include <JsonAdapter.h>
#include "ArduinoJson.h"
#include "HttpParser.h"

class TerraController {
public:
    void update(int hour, int minute, int second);

    Actuator *changeActuator(int index, int pin, int startTime, float actionTimeArray[], float durationArray[]);

    LinkedList<Sensor *> &getSensorList();

    LinkedList<Actuator *> &getActuatorList();

    void registerActuator(Actuator *actuator);

    void registerSensor(Sensor *sensor);

    void toString();

    String toJson();

    String actuatorToJson(int index);

    String actuatorListToJson(String &endpoint, String &httpRequest);

    String sensorListToJson(String &endpoint, String &httpRequest);

    String sensorToJson(String requestEndpoint, String httpRequest);

    String manualToJson(String requestEndpoint, String httpRequest);

    boolean getManual();

    void setManual(boolean manual);

    String updateActuator(String &endpoint, String &httpRequest);

private:
    LinkedList<Sensor *> sensorList = LinkedList<Sensor *>();
    LinkedList<Actuator *> actuatorList = LinkedList<Actuator *>();
    JsonAdapter adapter;
    boolean manual = false;
    HttpParser httpParser;
};

#endif //CLION_TERRACONTROLLER_H
