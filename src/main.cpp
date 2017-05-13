#include "Arduino.h"
#include <TerraController.h>
#include <TimeLib.h>
#include <DhtSensor.h>
#include <Router.h>
#include "UIPEthernet.h"
#include "Timer.h"
#include <avr/wdt.h>
#include <Lm35.h>
#include <WfSensor.h>
#include "EEPROM.h"

#define PROPERTIES 1
#if PROPERTIES == 1

#include "properties.h"

#else

#include  "testProperties.h"

#endif

struct Date {
    int hour;
    int minute;
    int second;
    int day;
    int month;
    int year;
};

Date date = {19, 38, 0, 16, 1, 17};

Router router;

Router router2;

Timer timer;

void serialSetup();

void setPins();

void updateTemperatures();

void setupEthernet();

void clearResetCounter();

void resetCount();

template<class T>
int EEPROM_readAnything(int ee, T &value);

void setTerraController(DhtSensor &sensor, Lm35 &lm35, WfSensor &wfSensor, Actuator &daylight, Actuator &evenLight,
                        Actuator &misting, Actuator &foog, TerraController &controller, Router &router,
                        String *timeEndpoint, String *manualEndpoint, String *sensorEndpoint, String *allEndpoint,
                        String *actuatorEndpoint, String *showTimeEndpoint, String *baseEndpoint);

EthernetServer server(80);
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192, 168, 1, 177);
HttpParser httpParser;
static const char *const HEADER_JSON = "HTTP/1.0 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Connection: close\r\n\r\n";


void setup() {

    serialSetup();
    resetCount();
    Serial.println("restart device");

    setTime(date.hour,
            date.minute,
            date.second,
            date.day,
            date.month,
            date.year);

    setPins();

    timer.every(1000, updateTemperatures);

    setupEthernet();

    setTerraController(dhtSensor1, lm35, wfSensor,
                       dayLight, evenLight, misting, foog,
                       tc1,
                       router,
                       &TIME, &MANUAL_CONTROL, &SENSORS, &ALL, &ACTUATOR, &SHOW_TIME, &tc1BaseEdpoint);

//    setTerraController(dhtSensor2, lm352, wfSensor2,
//                       dayLight2, evenLight2, misting2, foog2,
//                       tc2,
//                       router2,
//                       &TIME2, &MANUAL_CONTROL2, &SENSORS2, &ALL2, &ACTUATOR2, &SHOW_TIME2, &tc2BaseEdpoint);

    dhtSensor2.begin();

    tc2.registerSensor(&dhtSensor2);
    tc2.registerSensor(&lm352);

    tc2.registerActuator(&dayLight2);
    tc2.registerActuator(&evenLight2);

    LinkedList<Actuator *> &list = tc2.getActuatorList();
    for (int i = 0; i < list.size(); ++i) {
        router2.registerRoute(&list.get(i)->getEndpoint());
    }

    LinkedList<Sensor *> &list2 = tc2.getSensorList();
    for (int i = 0; i < list2.size(); ++i) {
        router2.registerRoute(&list2.get(i)->getEndpoint());
    }

    router2.registerBaseEndpoint(&tc2BaseEdpoint);

    router2.registerRoute(&TIME2);
    router2.registerRoute(&MANUAL_CONTROL2);
    router2.registerRoute(&SENSORS2);
    router2.registerRoute(&ALL2);
    router2.registerRoute(&ACTUATOR2);
    router2.registerRoute(&SHOW_TIME2);

    LinkedList<String *> &routeList = router2.getRouteList();
    for (int i = 0; i < routeList.size(); ++i) {
        Serial.println(*routeList.get(i));
    }

    String string = tc1.toJson();
    Serial.println(string);

    //tc2.toString();

    wdt_enable(WDTO_8S);     // enable the watchdog
}

void setTerraController(DhtSensor &sensor, Lm35 &lm35, WfSensor &wfSensor, Actuator &daylight, Actuator &evenLight,
                        Actuator &misting, Actuator &foog, TerraController &controller, Router &router,
                        String *timeEndpoint, String *manualEndpoint, String *sensorEndpoint, String *allEndpoint,
                        String *actuatorEndpoint, String *showTimeEndpoint, String *baseEndpoint) {
    sensor.begin();
    sensor.setName("dht");
    lm35.setName("lm35");
    wfSensor.setName("wfSensor");
    controller.registerSensor(&sensor);
    controller.registerSensor(&lm35);
    controller.registerSensor(&wfSensor);

    controller.registerActuator(&daylight);
    controller.registerActuator(&evenLight);
    controller.registerActuator(&misting);
    controller.registerActuator(&foog);

    LinkedList<Actuator *> &list = controller.getActuatorList();
    for (int i = 0; i < list.size(); ++i) {
        router.registerRoute(&list.get(i)->getEndpoint());
    }

    LinkedList<Sensor *> &sensorList = controller.getSensorList();
    for (int i = 0; i < sensorList.size(); ++i) {
        router.registerRoute(&sensorList.get(i)->getEndpoint());
    }

    router.registerBaseEndpoint(baseEndpoint);

    router.registerRoute(timeEndpoint);
    router.registerRoute(manualEndpoint);
    router.registerRoute(sensorEndpoint);
    router.registerRoute(allEndpoint);
    router.registerRoute(actuatorEndpoint);
    router.registerRoute(showTimeEndpoint);

    LinkedList<String *> &routeList = router.getRouteList();
    for (int i = 0; i < routeList.size(); ++i) {
        Serial.println(*routeList.get(i));
    }
}

void loadActuators() {
    EEPROM.get(50, dayLight);
    EEPROM.get(100, evenLight);
    EEPROM.get(150, foog);
    EEPROM.get(200, misting);
}

void saveActuators() {
    EEPROM.put(50, dayLight);
    EEPROM.put(100, evenLight);
    EEPROM.put(150, foog);
    EEPROM.put(200, misting);
}


void resetCount() {
    int counter = EEPROM.read(0);
    counter++;
    EEPROM.put(0, counter);
}

void clearResetCounter() {
    EEPROM.put(0, 0);
}

void setupEthernet() {
    Ethernet.begin(mac, ip);
    server.begin();
    Serial.print("server is at ");
    Serial.println(Ethernet.localIP());
}

void setPins() {
    for (uint8_t i = 8; i <= 13; i++) {
        pinMode(i, OUTPUT);
        digitalWrite(i, HIGH);
    }
}

void serialSetup() {
    Serial.begin(9600);
}

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
    Serial.println(analogRead(11));
    tc1.update(hour(), minute(), second());
    tc2.update(hour(), minute(), second());

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

        String requestEndpoint = httpParser.parseEndpoint(bodyHeader);
        String response;

        if (requestEndpoint.startsWith(tc1BaseEdpoint)) {
            bodyHeader.replace(tc1BaseEdpoint, "");
            response = router.route(bodyHeader, tc1, client);
            response.replace("TerraController", "TerraController1");
        } else if (requestEndpoint.startsWith(tc2BaseEdpoint)) {
            bodyHeader.replace(tc2BaseEdpoint, "");
            response = router2.route(bodyHeader, tc2, client);
            response.replace("TerraController", "TerraController2");
        }

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
    wdt_reset();

}












