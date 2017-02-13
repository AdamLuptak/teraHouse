//
// Created by adam on 29.11.2016.
//

#include "Actuator.h"

#define ON_RELAY LOW;
#define OFF_RELAY HIGH;

Actuator::Actuator() {}

Actuator::Actuator(uint8_t pin) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
    this->pin = pin;
}

Actuator::Actuator(uint8_t pin, long startTime, long *actionTimes, long *durations, String &endpoint, String &name) {

    for (int i = 0; i < 5; ++i) {
        this->actionTimes[i] = actionTimes[i];
    }

    for (int i = 0; i < 5; ++i) {
        this->durations[i] = durations[i];
    }

    this->name = name;
    this->endpoint = endpoint;
    this->pin = pin;
    this->startTime = startTime;
}

long Actuator::getActionTime(int numActionTime) {
    return actionTimes[numActionTime];
}

void Actuator::update(int hour, int minute, int second) {

    long time = timeToSecond(hour, minute, second);
    for (int index = 0; index < numActionTimes; ++index) {

        long actionTime = actionTimes[index];
        long duration = durations[index];

        if (time >= actionTime && time < actionTime + duration) {
            pinState = ON_RELAY;
            break;
        } else {
            pinState = OFF_RELAY;
        }
    }
    if (!digitalRead(pin) == pinState) {
        digitalWrite(pin, (uint8_t) pinState);
    }
}

void Actuator::setDuration(int duration, long durationPeriod) {
    this->durations[duration] = durationPeriod;
}

void Actuator::setDurations(long *durations) {
    memcpy(this->durations, durations, sizeof(durations));
}

void Actuator::setActionTime(int numActionTime, long actionTime) {
    this->actionTimes[numActionTime] = actionTime;
}

void Actuator::setActionTimes(long *actionTimes) {
    memcpy(this->actionTimes, actionTimes, sizeof(actionTimes));
}

long *Actuator::getDurations() {
    return this->durations;
}

long *Actuator::getActionTimes() {
    return this->actionTimes;
}

int Actuator::getPin() {
    return this->pin;
}

long Actuator::getStartTime() {
    return this->startTime;
}

void Actuator::setPin(uint8_t pin) {
    this->pin = pin;
}

void Actuator::setStartTime(long startTime) {
    this->startTime = startTime;
}

long Actuator::getDuration(int index) {
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
    sprintf(buff, "pin: %d, startTime:%d, state:%d, endpoint:%s\n"
            "intervals:", this->pin, this->startTime, this->pinState, this->endpoint.c_str());

    String string(buff);

    for (int i = 0; i < 5; ++i) {
        string.concat("[");
        string.concat(this->actionTimes[i]);
        string.concat(", ");
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
    sprintf(buff, "{\"pin\": %d, \"startTime\":%lu, \"state\":%d, \"endpoint\": \"%s\","
            "\"intervals\":", this->pin, this->startTime, this->pinState, this->endpoint.c_str());

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

String &Actuator::getEndpoint() {
    return endpoint;
}

void Actuator::setEndpoint(String & endpoint) {
    Actuator::endpoint = endpoint;
}

long Actuator::timeToSecond(int hour, int minute, int second) {
    return (long) hour * 3600 + (long) minute * 60 + second;
}

String Actuator::timeToString(long seconds) {
    int hour = (int) (seconds / 3600);
    int minute = (int) (seconds / 60 % 60);
    int second = (int) (seconds % 60);
    String timeString(hour);
    timeString.concat(":");
    timeString.concat(minute);
    timeString.concat(":");
    timeString.concat(second);
    return timeString;
}

const String &Actuator::getName() const {
    return name;
}

void Actuator::setName(const String &name) {
    Actuator::name = name;
}

boolean Actuator::getPinState() const {
    return pinState;
}

void Actuator::setPinState(boolean pinState) {
    Actuator::pinState = pinState;
}


