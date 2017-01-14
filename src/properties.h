//
// Created by adam on 12.1.2017.
//

#ifndef CLION_TESTPROPERTIES_H
#define CLION_TESTPROPERTIES_H

#include <WString.h>

static const int dthPin = 3;

static const int lm35Pin = 10;
static const int wfPin = 11;

String SEN_1_ENDPOINT = "sensor/sen1";
String SEN_2_ENDPOINT = "sensor/sen2";
String SEN_3_ENDPOINT = "sensor/sen3";
String DAYLIGHT = "actuator/dayLight";
String EVENLIGHT = "actuator/evenLight";
String MISTING = "actuator/misting";
String TIME = "setup/time";
String MANUAL_CONTROL = "setup/manualControl";
String FOOG = "actuator/foog";
String SENSORS = "sensor/";
String ACTUATOR = "actuator/";
String ALL = "all/";
String SHOW_TIME = "get/time";
//21600
long dayLightAT[5] = {
        21600}; //06:00:00
long dayLightDurations[5] = {
        36000};//10:00:00
static const long dayLightST = 21600;
static const int dayLightPin = 8;
Actuator dayLight(dayLightPin, dayLightST, dayLightAT, dayLightDurations, DAYLIGHT);

long evenLightAT[5] = {
        21600, 0, 0, 0, 0};//06:00:00
long evenLightDruations[5] = {
        43200, 0, 0, 0, 0};//12:00:00
static const long evenLightST = 21600;
static const int evenLightPin = 9;
Actuator evenLight(evenLightPin, evenLightST, evenLightAT, evenLightDruations, EVENLIGHT);

long foogAT[5] = {
        21600, 0, 0, 0, 0};//06:00:00
long foogDurations[5] = {
        20, 0, 0, 0, 0};//00:00:20
static const long foogST = 21600;
static const int foogPin = 13;
Actuator foog(foogPin, foogST, foogAT, foogDurations, FOOG);

long mistingAT[5] = {
        21600, 64800, 0, 0, 0};//06:00:00, 18:00:00
long mistingDurations[5] = {
        30, 30};//00:00:30
static const long mistingST = 21600;
static const int mistingPin = 11;
Actuator misting(mistingPin, mistingST, mistingAT, mistingDurations, MISTING);

DhtSensor dhtSensor1(dthPin, DHT11, SEN_1_ENDPOINT);
Lm35 lm35(lm35Pin, SEN_2_ENDPOINT);
WfSensor wfSensor(wfPin, SEN_3_ENDPOINT);
//DhtSensor dhtSensor2(3, DHT11, SEN_2_ENDPOINT);
//DhtSensor dhtSensor3(4, DHT11, SEN_3_ENDPOINT);

Actuator *actuatorArray[5] = {&dayLight, &evenLight, &foog, &misting};

/*
 * TeraControler 2 defualt values
 * */
static const int dthPin2 = 4;
static const int lm35Pin2 = 12;
static const int wfPin2 = 1;

String SEN_1_ENDPOINT2 = "sensor/sen1";
String SEN_2_ENDPOINT2 = "sensor/sen2";
String SEN_3_ENDPOINT2 = "sensor/sen3";
String DAYLIGHT2 = "actuator/dayLight";
String EVENLIGHT2 = "actuator/evenLight";
String MISTING2 = "actuator/misting";
String TIME2 = "setup/time";
String MANUAL_CONTROL2 = "setup/manualControl";
String FOOG2 = "actuator/foog";
String SENSORS2 = "sensor/";
String ACTUATOR2 = "actuator/";
String ALL2 = "all/";
String SHOW_TIME2 = "get/time";
//21600
long dayLightAT2[5] = {
        21600};//06:00:00
long dayLightDurations2[5] = {
        36000};//10:00:00
static const long dayLightST2 = 21600;
static const int dayLightPin2 = 10;
Actuator dayLight2(dayLightPin2, dayLightST2, dayLightAT2, dayLightDurations2, DAYLIGHT2);

long evenLightAT2[5] = {
        21600, 0, 0, 0, 0};//06:00:00
long evenLightDruations2[5] = {
        43200, 0, 0, 0, 0};//12:00:00
static const int evenLightST2 = 21600;//06:00:00
static const int evenLightPin2 = 12;
Actuator evenLight2(evenLightPin2, evenLightST2, evenLightAT2, evenLightDruations2, EVENLIGHT2);

long foogAT2[5] = {
        21600, 0, 0, 0, 0};//06:00:00
long foogDurations2[5] = {
        20, 0, 0, 0, 0};
static const int foogST2 = 21600;//06:00:00
static const int foogPin2 = 14;
Actuator foog2(foogPin2, foogST2, foogAT2, foogDurations2, FOOG2);

long mistingAT2[5] = {
        21600, 64800, 0, 0, 0};//06:00:00, 18:00:00
long mistingDurations2[5] = {
        30, 30};
static const int mistingST2 = 21600;//06:00:00
static const int mistingPin2 = 15;
Actuator misting2(mistingPin2, mistingST2, mistingAT2, mistingDurations2, MISTING2);

DhtSensor dhtSensor2(dthPin2, DHT11, SEN_1_ENDPOINT2);
Lm35 lm352(lm35Pin2, SEN_2_ENDPOINT2);
WfSensor wfSensor2(wfPin2, SEN_3_ENDPOINT2);
//DhtSensor dhtSensor2(3, DHT11, SEN_2_ENDPOINT);
//DhtSensor dhtSensor3(4, DHT11, SEN_3_ENDPOINT);
TerraController tc1;
TerraController tc2;

String tc1BaseEdpoint = "tc1/";
String tc2BaseEdpoint = "tc2/";

TerraController *tcArray[2] = {&tc1, &tc2};

#endif //CLION_TESTPROPERTIES_H
