//
// Created by adam on 30.11.2016.
//

#ifndef CLION_DHTSENSOR_H
#define CLION_DHTSENSOR_H

#include <Sensor.h>
#include <DHT.h>

class DhtSensor : public Sensor {
public:
    DhtSensor(int pin, uint8_t type, String &endpoint);

    virtual float getMeasurement();

    void begin();

    float getTemperature() const;

    void setTemperature(float temperature);

    float getHumidity() const;

    void setHumidity(float humidity);

    const DHT &getSensor() const;

    void setSensor(const DHT &sensor);

    virtual String toString();

    virtual String toJson();

private:
    float temperature;
    float humidity;
    boolean isBegin = false;
    DHT sensor;
};

#endif //CLION_DHTSENSOR_H
