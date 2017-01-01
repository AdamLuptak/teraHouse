//
// Created by adam on 25.12.2016.
//

#ifndef CLION_LM35_H
#define CLION_LM35_H


#include <Sensor.h>

class Lm35 : public Sensor {
public:

    Lm35(uint8_t pin, String &endpoint);

    virtual float getMeasurement();

    virtual String toString();

    virtual String toJson();
private:
    float temperature;
    uint8_t analogPin;
};


#endif //CLION_LM35_H
