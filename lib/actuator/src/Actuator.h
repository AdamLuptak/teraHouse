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

    Actuator(uint8_t pin, long startTime, long actionTimes[], long durations[], String &endpoint, String &name);

    void update(int Hour, int minute, int second);

    void setPin(uint8_t pin);

    void setStartTime(long startTime);

    void setDuration(int duration, long durationPeriod);

    void setDurations(long *durations);

    void setActionTime(int numActionTime, long actionTime);

    void setActionTimes(long *actionTimes);

    long getActionTime(int numActionTime);

    long getDuration(int index);

    long *getDurations();

    long *getActionTimes();

    int getPin();

    long getStartTime();

    String toString();

    String toJson();

    boolean turnOn();

    boolean turnOff();

    String &getEndpoint();

    void setEndpoint(String &endpoint);

    const String &getName() const;

    void setName(const String &name);

    boolean getPinState() const;

    void setPinState(boolean pinState);

private:
    String endpoint = "empty";
    String name = "name";
    int numActionTimes = 5;
    long durations [5];
    long actionTimes [5];
    uint8_t pin;
    long startTime;
    boolean pinState = false;
    String timeToString(long seconds);
    long timeToSecond(int hour, int minute, int second);
};


#endif //TEST_ACTUATOR_H


#endif //CLION_ACTUATOR_H
