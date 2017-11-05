//
// Created by adam on 16.5.2017.
//

#ifndef CLION_JSONSDSERIALIZER_H
#define CLION_JSONSDSERIALIZER_H


#include <Sensor.h>
#include <Actuator.h>
#include <SD.h>
#include <SPI.h>

class JsonSDSerializer {
public:
    void saveActuatatorToFile(Actuator actuator);

    void saveSensorToFile(Sensor &sensor);

    Sensor & loadSensorFromFile();

    Actuator loadActuatorFromFile();

private:
    File actuatorFile;
    String actuatorFileName = "actuators.txt";

};


#endif //CLION_JSONSDSERIALIZER_H
