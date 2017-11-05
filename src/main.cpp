#include "Arduino.h"
#include <TerraController.h>
#include <TimeLib.h>
#include <DhtSensor.h>
#include <Router.h>
#include <SD.h>
#include <SPI.h>
#include <Ethernet.h>
#include "Timer.h"
#include <avr/wdt.h>
#include <Lm35.h>
#include <WfSensor.h>
#include "EEPROM.h"
#include <MemoryFree.h>
#include <ActuatorMapper.h>


#define PROPERTIES 1
#if PROPERTIES == 1

#include "properties.h"
#include "../lib/jsonSdSerializer/src/JsonSDSerializer.h"

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

enum httpRequestType {
    GET,
    POST,
    PUT
};

Date date = {19, 38, 0, 16, 1, 17};

String request = "";

Router router;

Router router2;

Timer timer;

void serialSetup();

void setPins();

void updateTemperatures();

void setupEthernet();

void clearResetCounter();

void resetCount();

ActuatorMapper actuatorMapper;

File myFile;

template<class T>
int EEPROM_readAnything(int ee, T &value);

void setTerraController(DhtSensor &sensor, Lm35 &lm35, WfSensor &wfSensor, Actuator &daylight, Actuator &evenLight,
                        Actuator &misting, Actuator &foog, TerraController &controller, Router &router,
                        String *timeEndpoint, String *manualEndpoint, String *sensorEndpoint, String *allEndpoint,
                        String *actuatorEndpoint, String *showTimeEndpoint, String *baseEndpoint);

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
        0x90, 0xA2, 0xDA, 0x0D, 0x58, 0x6E};
IPAddress ip(192, 168, 1, 77);
IPAddress gateway(192, 168, 1, 1);
IPAddress mask(255, 255, 255, 0);
// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
int port = 80;
EthernetServer server = EthernetServer(port);
HttpParser httpParser;
static const char *const HEADER_JSON = "HTTP/1.0 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Connection: close\r\n\r\n";


void setup() {

    serialSetup();
    resetCount();
    Serial.println("restart device");


    const String &jsonString = actuatorMapper.actuatorToJsonString(dayLight);
    Serial.println(jsonString);


    setTime(date.hour,
            date.minute,
            date.second,
            date.day,
            date.month,
            date.year);

    setPins();

    timer.every(1000, updateTemperatures);
    Actuator actuators[10];

    actuators[0] = dayLight;
    actuators[1] = misting;


    if (!SD.begin(4)) {
        Serial.println("initializatdsdfsdfion failed!");
        return;
    }
    File actuatorFile;


    actuatorFile = SD.open("ac.txt");
    String oneLine;
    StaticJsonBuffer<1000> jsonBuffer;

    if (actuatorFile) {
        Serial.println("test.txt:");

        // read from the file until there's nothing else in it:
        while (actuatorFile.available()) {
            int i = actuatorFile.read();
            if (i == '\n') {
                
                
                
                
                JsonObject &object = jsonBuffer.parse(oneLine);
                object.prettyPrintTo(Serial);
                oneLine = "";
            } else {
                oneLine.concat((char) i);
            }
        }
        // close the file:
        actuatorFile.close();
    } else {
        // if the file didn't open, print an error:
        Serial.println("error opening test.txt");
    }




//    JsonSDSerializer jsonSDSerializer;
//
//    jsonSDSerializer.saveActuatatorToFile(misting);
//    const Actuator &actuator = jsonSDSerializer.loadActuatorFromFile();


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


    // if the file opened okay, write to it:
//    if (myFile) {
//        Serial.print("Writing to test.txt...");
//        myFile.println("testing 1, 2, 3.");
//        // close the file:
//        myFile.close();
//        Serial.println("done.");
//    } else {
//        // if the file didn't open, print an error:
//        Serial.println("error opening test.txt");
//    }

    // re-open the file for reading:




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
    Serial.print(Ethernet.localIP());
    Serial.print(":");
    Serial.println(port);
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
    request = "";
    httpRequestType requestType;
    // listen for incoming clients
    EthernetClient client = server.available();
    if (client) {
        Serial.println("new client");
        // an http request ends with a blank line
        boolean currentLineIsBlank = true;
        int endOfBodyCounter = 0;
        int contentLenght = 0;
        int headerCharCounter = 0;
        while (client.connected()) {
            if (client.available()) {
                char c = client.read();
                request.concat(c);
                if (c == '\n' && currentLineIsBlank) {

                    if (contentLenght == 0) {

                        if (request.indexOf("GET") > -1) {
                            Serial.println(headerCharCounter);
                            client.println("HTTP/1.1 200 OK");
                            client.println("Content-Type: application/json");
                            client.println(
                                    "Connection: close");  // the connection will be closed after completion of the response
                            break;
                        } else if (request.indexOf("POST") > -1) {
                            Serial.println("Request type POST");
                            int startIndexOfContentLenght = request.indexOf("Content-Length: ") + 16;
                            String stringStartsFromContentLenght = request.substring(startIndexOfContentLenght);
                            int indexOfEndOfContenLenght = stringStartsFromContentLenght.indexOf("\r\n");
                            contentLenght = stringStartsFromContentLenght.substring(0,
                                                                                    indexOfEndOfContenLenght).toInt();
                            Serial.print("ContentLenght: ");
                            Serial.println(contentLenght);

                        } else if (request.indexOf("PUT") > -1) {
                            Serial.println("Request type POST");
                            int startIndexOfContentLenght = request.indexOf("Content-Length: ") + 16;
                            String stringStartsFromContentLenght = request.substring(startIndexOfContentLenght);
                            int indexOfEndOfContenLenght = stringStartsFromContentLenght.indexOf("\r\n");
                            contentLenght = stringStartsFromContentLenght.substring(0,
                                                                                    indexOfEndOfContenLenght).toInt();
                            Serial.print("ContentLenght: ");
                            Serial.println(contentLenght);

                        }


                    }

                }
                if (contentLenght != 0) {
                    endOfBodyCounter++;
                    if (endOfBodyCounter == contentLenght + 1) {
                        Serial.println(headerCharCounter);
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-Type: application/json");
                        client.println(
                                "Connection: close");  // the connection will be closed after completion of the response
                        break;
                    }
                } else {
                    headerCharCounter++;
                }


                if (c == '\n') {
                    // you're starting a new line
                    currentLineIsBlank = true;
                } else if (c != '\r') {
                    // you've gotten a character on the current line
                    currentLineIsBlank = false;
                }
            }
        }
        // give the web browser time to receive the data
        request.remove(0, headerCharCounter + 1);
        StaticJsonBuffer<1000> jsonBuffer;
        JsonObject &object = jsonBuffer.parse(request.c_str());
        object.prettyPrintTo(Serial);
        delay(1);

        // close the connection:
        client.stop();
        Serial.println("client disconnected");
    }

    timer.update();
    wdt_reset();
}












