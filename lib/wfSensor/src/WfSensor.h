//
// Created by adam on 25.12.2016.
//

#ifndef CLION_WFSENSOR_H
#define CLION_WFSENSOR_H


#include <Sensor.h>

class WfSensor : public Sensor {
public:

    WfSensor(uint8_t pin, String &endpoint);

    virtual float getMeasurement();

    virtual String toString();

    virtual String toJson();
private:
    float isWaterFlowing = 0;
    uint8_t sensoPin;
};


#endif //CLION_WFSENSOR_H
