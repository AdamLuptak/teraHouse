#include "Arduino.h"]
#include <TerraController.h>
#include <TimeLib.h>
#include <DhtSensor.h>
#include <Router.h>
#include "UIPEthernet.h"
#include "Timer.h"

Router router;

Timer timer;

void serialSetup();

void setPins();

void updateTemperatures();

void setupEthernet();

EthernetServer server(80);
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192, 168, 1, 177);
static const char *const HEADER_JSON = "HTTP/1.0 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Connection: close\r\n\r\n";

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


float dayLightAT[5] = {
        14.10};
float dayLightDurations[5] = {
        0.40};
static const int dayLightST = 13;
static const int dayLightPin = 8;
Actuator dayLight(dayLightPin, dayLightST, dayLightAT, dayLightDurations, DAYLIGHT);

float evenLightAT[5] = {
        14.15};
float evenLightDruations[5] = {
        0.40};
static const int evenLightST = 13;
static const int evenLightPin = 9;
Actuator evenLight(evenLightPin, evenLightST, evenLightAT, evenLightDruations, EVENLIGHT);

float mistingAT[5] = {
        14.20, 14.40, 14.50};
float mistingDurations[5] = {
        0.02, 0.02, 0.02};
static const int mistingST = 13;
static const int mistingPin = 10;
Actuator misting(mistingPin, mistingST, mistingAT, mistingDurations, MISTING);

float foogAT[5] = {
        14.10};
float foogDurations[5] = {
        0.10};
static const int foogST = 13;
static const int foogPin = 11;
Actuator foog(foogPin, foogST, foogAT, foogDurations, FOOG);

DhtSensor dhtSensor1(2, DHT11, SEN_1_ENDPOINT);
DhtSensor dhtSensor2(3, DHT11, SEN_2_ENDPOINT);
DhtSensor dhtSensor3(4, DHT11, SEN_3_ENDPOINT);

TerraController tc;

void setup() {
    serialSetup();

    //saveActuators();

    //loadActuators();

    setTime(14, 20, 56, 1, 1, 11);

    setPins();

    timer.every(1000, updateTemperatures);

    setupEthernet();

    dhtSensor1.begin();
    dhtSensor2.begin();
    dhtSensor3.begin();

    tc.registerSensor(&dhtSensor1);
    tc.registerSensor(&dhtSensor2);
    tc.registerSensor(&dhtSensor3);

    tc.registerActuator(&dayLight);
    tc.registerActuator(&evenLight);
    tc.registerActuator(&misting);
    tc.registerActuator(&foog);

    LinkedList<Actuator *> &list = tc.getActuatorList();
    for (int i = 0; i < list.size(); ++i) {
        router.registerRoute(&list.get(i)->getEndpoint());
    }

    LinkedList<Sensor *> &sensorList = tc.getSensorList();
    for (int i = 0; i < sensorList.size(); ++i) {
        router.registerRoute(&sensorList.get(i)->getEndpoint());
    }

    router.registerRoute(&TIME);
    router.registerRoute(&MANUAL_CONTROL);
    router.registerRoute(&SENSORS);
    router.registerRoute(&ALL);
    router.registerRoute(&ACTUATOR);

    LinkedList<String *> &routeList = router.getRouteList();
    for (int i = 0; i < routeList.size(); ++i) {
        Serial.println(*routeList.get(i));
    }
}

void setupEthernet() {
    Ethernet.begin(mac, ip);
    server.begin();
    Serial.print("server is at ");
    Serial.println(Ethernet.localIP());
}

void setPins() {
    for (uint8_t i = 8; i <= 11; i++) {
        pinMode(i, OUTPUT);
        digitalWrite(i, LOW);
    }
}

void serialSetup() {
    Serial.begin(9600);
}

//void loadActuators() {
//    dayLight = EEPROM.get(eeDayLight, dayLight);
//    evenLight = EEPROM.get(eeEvenLight, evenLight);
//    foog = EEPROM.get(eeFoog, foog);
//    misting = EEPROM.get(eeMisting, misting);
//}
//
//void saveActuators() {
//    EEPROM.put(eeDayLight, dayLight);
//    EEPROM.put(eeEvenLight, evenLight);
//    EEPROM.put(eeFoog, foog);
//    EEPROM.put(eeMisting, misting);
//}

void printDigits(int digits) {
    Serial.print(":");
    if (digits < 10)
        Serial.print('0');
    Serial.print(digits);
}

void digitalClockDisplay() {
    Serial.print(hour());
    printDigits(minute());
    printDigits(second());
    Serial.println();
}

void updateTemperatures() {
    tc.update(hour(), minute());
    delay(1);
}

void loop() {
    delay(1);
    EthernetClient client = server.available();
    String bodyHeader = "";
    if (client) {
        Serial.println("new client");
        int finalString = 0;
        long prev = millis();
        boolean timeout = false;
        while (client.connected()) {

            long time = millis();
            //timeout protection
            if (time - prev > 5000) {
                timeout = true;
                break;
            }

            if (client.available()) {

                char actual = (char) client.read();
                bodyHeader.concat(actual);
                if (bodyHeader.indexOf("\n{") > 0) {
                } else {
                    finalString++;
                }
                if (bodyHeader.indexOf("HTTP") > 0) {
                    break;
                }
            }
        }

        String response = router.route(bodyHeader, tc, client);
        if (timeout) {
            response = "{\"error\" : \"timeout\"}";
        }

        client.print(HEADER_JSON);
        client.println(response);

        delay(1);
        client.stop();
        Serial.println("client disconnected");
    }

    timer.update();
}












