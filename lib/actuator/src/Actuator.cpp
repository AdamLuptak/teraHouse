//
// Created by adam on 29.11.2016.
//

#include "Actuator.h"

Actuator::Actuator() {}

Actuator::Actuator(uint8_t pin) {
    pinMode(pin, OUTPUT);
    this->pin = pin;
}

Actuator::Actuator(uint8_t pin, int startTime, float *actionTimes, float *durations, String &endpoint) {
    this->actionTimes = actionTimes;
    this->durations = durations;
    this->endpoint = endpoint;
    this->pin = pin;
    this->startTime = startTime;
}

float Actuator::getActionTime(int numActionTime) {
    return actionTimes[numActionTime];
}

void Actuator::update(int hour, int minutes) {

    float time = hour + ((float) minutes / 100);
    for (int index = 0; index < numActionTimes; ++index) {

        float actionTime = actionTimes[index];
        float duration = durations[index];

        if (time >= actionTime && time < actionTime + duration) {
            pinState = true;
            break;
        } else {
            pinState = false;
        }
    }
    if (!digitalRead(pin) == pinState) {
        digitalWrite(pin, (uint8_t) pinState);
    }
}

void Actuator::setDuration(int duration, float durationPeriod) {
    this->durations[duration] = durationPeriod;
}

void Actuator::setDurations(float *durations) {
    this->durations = durations;
}

void Actuator::setActionTime(int numActionTime, float actionTime) {
    this->actionTimes[numActionTime] = actionTime;
}

void Actuator::setActionTimes(float *actionTimes) {
    this->actionTimes = actionTimes;
}

float *Actuator::getDurations() {
    return this->durations;
}

float *Actuator::getActionTimes() {
    return this->actionTimes;
}

int Actuator::getPin() {
    return this->pin;
}

int Actuator::getStartTime() {
    return this->startTime;
}

void Actuator::setPin(uint8_t pin) {
    this->pin = pin;
}

void Actuator::setStartTime(int startTime) {
    this->startTime = startTime;
}

float Actuator::getDuration(int index) {
    return this->durations[index];
}

boolean Actuator::turnOn() {
    digitalWrite(pin, HIGH);
    return (boolean) digitalRead(pin);
}

boolean Actuator::turnOff() {
    digitalWrite(pin, LOW);
    return (boolean) digitalRead(pin);

}

String Actuator::toString() {
    char buff[200];
    sprintf(buff, "pin: %d, startTime:%d, state:%d\n"
            "intervals:",this->pin,this->startTime,this->pinState);

    String string(buff);

    for (int i = 0; i < 5; ++i) {
        string.concat("[");
        string.concat(this->actionTimes[i]);
        string.concat(this->durations[i]);

        if (i == 4) {
            string.concat("]\n");
        } else {
            string.concat("],\n");
        }
    }
    return string;
}

String Actuator::toJson() {
    char buff[200];
    sprintf(buff, "{\"pin\": %d, \"startTime\":%d, \"state\":%d, \"endpoint\": \"%s\","
            "\"intervals\":",this->pin,this->startTime,this->pinState,this->endpoint.c_str());

    String string(buff);
    string.concat("[");
    for (int i = 0; i < 5; ++i) {
        string.concat("{");
        string.concat("\"actionTime\":");
        string.concat(this->actionTimes[i]);
        string.concat(",\"duration\":");
        string.concat(this->durations[i]);

        if (i == 4) {
            string.concat("}]");
        } else {
            string.concat("},");
        }
    }
    string.concat("}");
    return string;
}

 String &Actuator::getEndpoint()  {
    return endpoint;
}

void Actuator::setEndpoint( String &endpoint) {
    Actuator::endpoint = endpoint;
}
