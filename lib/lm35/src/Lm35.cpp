//
// Created by adam on 25.12.2016.
//

#include <USBAPI.h>
#include "Lm35.h"

Lm35::Lm35(uint8_t pin, String &endpoint) {
    this->endpoint = endpoint;
    pinMode(pin, INPUT);
    this->analogPin = pin;
}

float Lm35::getMeasurement() {
    temperature = 0;
    delay(1);
    float val = analogRead(analogPin);
    delay(1);
    // 500mV/1024=.48828125
    temperature = val * 0.48828125;
    return temperature;
}

String Lm35::toString() {
    String string("temperature: ");
    string.concat(this->temperature);
    return string;
}

String Lm35::toJson() {
    String string = this->toString();
    string.replace("temperature", "{\"temperature\"");
    string.concat("}");
    return string;
}

