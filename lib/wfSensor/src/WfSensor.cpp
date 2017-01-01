//
// Created by adam on 25.12.2016.
//

#include <USBAPI.h>
#include "WfSensor.h"

WfSensor::WfSensor(uint8_t pin, String &endpoint) {
    this->endpoint = endpoint;
    this->sensoPin = pin;
}

float WfSensor::getMeasurement() {
    float volts = analogRead(sensoPin);
    isWaterFlowing = volts;
    return isWaterFlowing;
}

String WfSensor::toString() {
    String string("waterFlowing: ");
    string.concat(this->isWaterFlowing);
    return string;
}

String WfSensor::toJson() {
    String string = this->toString();
    string.replace("waterFlowing", "{\"waterFlowing\"");
    string.concat("}");
    return string;
}
