//
// Created by adam on 30.11.2016.
//

#include "Sensor.h"


String &Sensor::getEndpoint() {
    return endpoint;
}

void Sensor::setEndpoint(String &endpoint) {
    Sensor::endpoint = endpoint;
}

const String &Sensor::getName() const {
    return name;
}

void Sensor::setName(const String &name) {
    Sensor::name = name;
}
