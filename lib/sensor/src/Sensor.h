//
// Created by adam on 30.11.2016.
//

#ifndef CLION_SENSOR_H
#define CLION_SENSOR_H


#include <WString.h>

class Sensor {
public:
    Sensor() {};

    virtual float getMeasurement()=0;

    virtual String toString()=0;

    virtual String toJson()=0;

    String &getEndpoint();

    void setEndpoint(String &endpoint);

private:

protected:
    String endpoint = "empty";
};

#endif //CLION_SENSOR_H
