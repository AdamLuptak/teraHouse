//
// Created by adam on 30.11.2016.
//

#include "DhtSensor.h"

float DhtSensor::getMeasurement() {
    if (isBegin) {
        this->temperature = this->sensor.readTemperature();
        this->humidity = this->sensor.readHumidity();
    }
    return 0;
}

const DHT &DhtSensor::getSensor() const {
    return sensor;
}

void DhtSensor::setSensor(const DHT &sensor) {
    DhtSensor::sensor = sensor;
}

DhtSensor::DhtSensor(uint8_t pin, uint8_t type, String &endpoint) {
    this->endpoint = endpoint;
    this->sensor = DHT(pin, type);
}

void DhtSensor::begin() {
    this->sensor.begin();
    this->isBegin = true;
}

float DhtSensor::getTemperature() const {
    return temperature;
}

void DhtSensor::setTemperature(float temperature) {
    DhtSensor::temperature = temperature;
}

float DhtSensor::getHumidity() const {
    return humidity;
}

void DhtSensor::setHumidity(float humidity) {
    DhtSensor::humidity = humidity;
}

String DhtSensor::toString() {
    String string("temperature: ");
    string.concat(this->temperature);
    string.concat("humidity: ");
    string.concat(this->humidity);
    return string;
}

String DhtSensor::toJson() {
    String string = this->toString();
    string.replace("temperature", "{\"temperature\"");
    string.replace("humidity", ",\"humidity\"");
    string.concat("}");
    return string;
}






