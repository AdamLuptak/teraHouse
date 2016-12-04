//
// Created by adam on 1.12.2016.
//

#ifndef CLION_JSONADAPTER_H
#define CLION_JSONADAPTER_H

#include <WString.h>
#include <Actuator.h>
#include <Sensor.h>

class JsonAdapter {
public:
    String actuatorToJson(Actuator *actuator);

    String sensorToJson(Sensor *sensor);

};


#endif //CLION_JSONADAPTER_H
