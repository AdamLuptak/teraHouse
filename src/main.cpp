#include "Arduino.h"]
#include <TerraController.h>
#include <Adafruit_Sensor.h>
#include <TimeLib.h>
#include <DhtSensor.h>
#include <LinkedList.h>
#include <Router.h>
#include "SPI.h"
#include "UIPEthernet.h"
#include "DHT.h"
#include "Timer.h"
#include "ArduinoJson.h"
#include "Actuator.h"
#include "EEPROM.h"
#include "constants.h"
#include "objectDeclaration.h"
#include "variables.h"

String errMessage;
boolean manual = false;
String response = "";
String responseSecond = "";

Router router;

Timer timer;

void loadActuators();

void serialSetup();

void setPins();

void prepareJsonOutputObject();

void sentResponse();

void unauthorizedResponse();

void processResponse(Actuator &actuator);

void saveActuators();

boolean requestToActuator(Actuator &data, char *string, char *endpoint, int size);

void setTimeJson(JsonObject &jsonTime);

void actuatorAction(const char *data);

void bandInputDataResponse();

boolean isInInterval(int value, int max, int min);

void updateTemperatures();

void sendResponse(UIPClient &client, const char HEADER[]);

void setupEthernet();

EthernetServer server(80);

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
String ALL = "all";


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

LinkedList<Sensor *> sensorList = LinkedList<Sensor *>();


TerraController tc;

void setup() {
    serialSetup();

    //saveActuators();

    //loadActuators();

    setTime(14, 20, 56, 1, 1, 11);

    setPins();

    timer.every(1000, updateTemperatures);

    setupEthernet();

    pinMode(8, OUTPUT);


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

//    router.registerRoute(&EVENLIGHT);
//    router.registerRoute(&MISTING);
//    router.registerRoute(&FOOG);
//    router.registerRoute(&TIME);
//    router.registerRoute(&MANUAL_CONTROL);
//    router.registerRoute(&SENSORS);
}

void setupEthernet() {
    Ethernet.begin(mac, ip);
    server.begin();
    Serial.print("server is at ");
    Serial.println(Ethernet.localIP());
}

void setPins() {
    for (int i = 8; i <= 11; i++) {
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
    int hours = hour();
    int minutes = minute();
    int seconds = second();
    tc.update(hours, minutes, seconds);
    tc.toJson();
    delay(1);
}

String getValue(String data, char separator, int index) {
    int found = 0;
    int strIndex[] = {
            0, -1};
    int maxIndex = data.length() - 1;
    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i + 1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

JsonObject &getObject(JsonObject &actuatorJson, const char *key, Actuator &actuator) {
    JsonObject &daylightLocalJson = actuatorJson.createNestedObject(key);
    JsonArray &actionTimesJson = daylightLocalJson.createNestedArray("actionTimes");
    JsonArray &durationsJson = daylightLocalJson.createNestedArray("durations");

    for (int i = 0; i < 5; ++i) {
        actionTimesJson.add(actuator.getActionTime(i));
        durationsJson.add(actuator.getDuration(i));
    }

    daylightLocalJson["startTime"] = actuator.getStartTime();
    daylightLocalJson["pin"] = actuator.getPin();
    daylightLocalJson["pinState"] = digitalRead(actuator.getPin());

    return daylightLocalJson;
}


void loop() {
    delay(1);
    char buff[200];
    // listen for incoming clients
    EthernetClient client = server.available();
    String bodyHeader = "";
    String bodyy;
    if (client) {
        Serial.println("new client");
        // an http request ends with a blank line
        boolean currentLineIsBlank = true;
        char getBuffer[10];
        char httpBuff[10];
        int finalString = 0;
        char next;
        char previous;
        long prev = millis();
        boolean timeout = false;
        while (client.connected()) {

            long time = millis();


            if (time - prev > 5000) {
                timeout = true;
                break;
            }

            if (client.available()) {

                char actual = client.read();
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

        /*  String getRequest = headerRequest.substring(body.indexOf("GET /") + 5, body.indexOf("HTTP"));

          if (getRequest.indexOf(DAY_LIGHT)) {
          }*/


        // give the web browser time to receive the data
        delay(1);
        // close the connection:
        client.stop();
        Serial.println("client disonnected");
    }


    digitalWrite(8, HIGH);

    timer.update();
}

void sendResponse(UIPClient &client, const char *HEADER) {
    client.print(HEADER);
    client.println(tc.toJson());
}

boolean isInInterval(int value, int max, int min) {
    if (value < min || value > max) {
        return false;
    }
    return true;
}

void
actuatorAction(char *data, Actuator &actuator, const char *ON_FLAG, const char *OFF_FLAG, char endpoint[], int size) {
    if (!manual) {
        if (requestToActuator(actuator, data, endpoint, size)) {
            saveActuators();
            loadActuators();
            processResponse(actuator);
        } else {
            bandInputDataResponse();
        }
    } else {
        String request(data);
        int acPin = actuator.getPin();
        if (request.indexOf(ON_FLAG) > -1) {
            digitalWrite(acPin, HIGH);
        } else if (request.indexOf(OFF_FLAG) > -1) {
            digitalWrite(acPin, LOW);
        }
        //  bfill.emit_p(PSTR("$F {\"dayLight\" : $D}"), HTTP_JSON, digitalRead(acPin));
    }
}

void setTimeJson(JsonObject &jsonTime) {

    jsonTime["hours"] = hour();
    jsonTime["minutes"] = minute();
    jsonTime["seconds"] = second();
    jsonTime["day"] = day();
    jsonTime["month"] = month();
    jsonTime["year"] = year();
}

boolean requestToActuator(Actuator &actuator, char *data, char *endpoint, int size) {
    String body(data);
    char bodyChar[200];
    body.substring(body.indexOf(endpoint) + size, body.indexOf(HTTP)).toCharArray(bodyChar, 200);
    int count = 0;
    for (int i = 0; i < strlen(bodyChar); ++i) {
        if (bodyChar[i] == ',') {
            count++;
        }
    }

    if (count == 0) {
        return false;
    }

    int indexAction = 0;
    while (count >= 0) {
        if (count == 0) {
            int pin = getValue(bodyChar, ',', count).toInt();
            if (pin < 8 || pin > 11) {
                errMessage = errMessage + " pin ";
            }
            actuator.setPin(getValue(bodyChar, ',', count).toInt());
        } else if (count == 1) {
            int startTime = getValue(bodyChar, ',', count).toInt();
            if (startTime < 0 || startTime > 24) {
                errMessage = errMessage + " startTime ";
            }
            actuator.setStartTime(startTime);

        } else if (count % 2 == 0) {
            float acTime = atof(getValue(bodyChar, ',', count).c_str());
            if (acTime <= 0 || acTime > 24) {
                errMessage = errMessage + " actionTime" + indexAction + " ";
            }
            actuator.setActionTime(indexAction, acTime);
            indexAction++;

        } else if (count % 2 == 1) {
            float durTime = atof(getValue(bodyChar, ',', count).c_str());
            //Serial.println(durTime);
            if (durTime <= 0 || durTime > 24) {
                errMessage = errMessage + " duration" + indexAction + " ";
            }
            actuator.setDuration(indexAction, durTime);
        }
        count--;
    }
    return (errMessage.length() > 0) ? false : true;
}

void processResponse(Actuator &actuator) {
    StaticJsonBuffer<500> jsBuff;
    JsonObject &actuatorJson = jsBuff.createObject();
    JsonArray &actionTimesJson = actuatorJson.createNestedArray("actionTime");
    JsonArray &durationsJson = actuatorJson.createNestedArray("duration");
    responseSecond = "";
    for (int i = 0; i < 5; ++i) {
        actionTimesJson.add(actuator.getActionTime(i));
        durationsJson.add(actuator.getDuration(i));
    }

    actuatorJson["startTime"] = actuator.getStartTime();
    actuatorJson["pin"] = actuator.getPin();
    actuatorJson["pinState"] = digitalRead(actuator.getPin());

    actuatorJson.printTo(responseSecond);
    //  bfill.emit_p(PSTR("$F $S"), HTTP_JSON, responseSecond.c_str());
}

void unauthorizedResponse() {// bfill.emit_p(http_Unauthorized);
}

void bandInputDataResponse() {
    //bfill.emit_p(PSTR("$F {\"error\" : \"invalid input data [$S]\"}"), HTTP_JSON, errMessage.c_str());
    errMessage = "";
}

void sentResponse() {
    //ether.httpServerReply(bfill.position());
}




