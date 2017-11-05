//
// Created by adam on 16.5.2017.
//

#include <DhtSensor.h>
#include "JsonSDSerializer.h"

Sensor & JsonSDSerializer::loadSensorFromFile() {
    const DhtSensor & sensor = DhtSensor();
    return (Sensor &) sensor;
}

Actuator JsonSDSerializer::loadActuatorFromFile() {

    actuatorFile = SD.open(actuatorFileName);
    if (actuatorFile) {
        Serial.println(actuatorFileName);

        // read from the file until there's nothing else in it:
        while (actuatorFile.available()) {
            Serial.write(actuatorFile.read());
        }
        // close the file:
        actuatorFile.close();
    } else {
        // if the file didn't open, print an error:
        Serial.println("error opening test.txt");
    }

    return Actuator();
}

void JsonSDSerializer::saveActuatatorToFile(Actuator actuator) {
    if (!SD.begin(4)) {
        Serial.println("initializatdsdfsdfion failed!");
        return;
    }
    actuatorFile = SD.open("actuators.txt", FILE_WRITE);

    if (actuatorFile) {
        Serial.print("Writing to actuators.txt...");
        actuatorFile.println(actuator.toJson());
        // close the file:
        actuatorFile.close();
        Serial.println("done.");
    } else {
        // if the file didn't open, print an error:
        Serial.println("error opening actuators.txt");
    }
}

void JsonSDSerializer::saveSensorToFile(Sensor &sensor) {

}
