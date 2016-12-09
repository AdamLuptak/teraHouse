//
// Created by adam on 29.11.2016.
//

#ifndef CLION_ACTUATOR_H
#define CLION_ACTUATOR_H


//
// Created by root on 14.11.2016.
//

#ifndef TEST_ACTUATOR_H
#define TEST_ACTUATOR_H

#include "Arduino.h"

class Actuator {
public:
    Actuator();

    Actuator(uint8_t pin);

    Actuator(uint8_t pin, int startTime, float actionTimes[], float durations[], String &endpoint);

    void update(int Hour, int minutes);

    void setPin(uint8_t pin);

    void setStartTime(int startTime);

    void setDuration(int duration, float durationPeriod);

    void setDurations(float *durations);

    void setActionTime(int numActionTime, float actionTime);

    void setActionTimes(float *actionTimes);

    float getActionTime(int numActionTime);

    float getDuration(int index);

    float *getDurations();

    float *getActionTimes();

    int getPin();

    int getStartTime();

    String toString();

    String toJson();

    boolean turnOn();

    boolean turnOff();

    String &getEndpoint();

    void setEndpoint(String &endpoint);

private:
    String endpoint = "empty";
    int numActionTimes = 5;
    float *durations;
    float *actionTimes;
    uint8_t pin;
    int startTime;
    boolean pinState = false;
};


#endif //TEST_ACTUATOR_H


#endif //CLION_ACTUATOR_H
