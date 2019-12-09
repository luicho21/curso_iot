//MKRENV_and_MQTT v0.1.0

#include <Arduino_MKRENV.h>
#include <SPI.h>
#include <SD.h>
#include <WiFi101.h>
#include <PubSubClient.h>
//#include "RoundRobinbyJR.h"

#include "arduino_secrets.h"   //please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;     // your network SSID (name)
char pass[] = SECRET_PASS;     // your network password

void getENVValues(int marca = 2);  //valor inicial = 1
 
int status = WL_IDLE_STATUS;
//IPAddress server(34, 255, 208, 144); //MQTT Broker ip
IPAddress server(192, 168, 4, 1); //MQTT Broker ip
int port = 8883;
WiFiSSLClient client;
PubSubClient mqttClient(server, port, client);

//File myFile;
//char fileName[20] = "reg24h.txt";
//const int trigger = 12 * 25; //25 hours
//const int logsToRemove = 12; //1 hour
//unsigned long timeSD = 5 * 60 * 1000; //every 5 minutes
unsigned long timeMQTT = 5000;
unsigned long prevTime1 = 0 , prevTime2 = 0;
//byte insertedSD = 0;
char buffer [20];

void setup() {
  delay(5000);
  Serial.begin(9600);

  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);      // Connect to WPA/WPA2 network
    delay(5000);
  }
  printWiFiStatus();
  IPAddress ip = WiFi.localIP();
  snprintf(buffer, 20, "%d.%ld.%ld.%ld" , ip[0], ip[1], ip[2], ip[3]);

  while (!mqttClient.connect("arduinoClient", SECRET_USERNAME, SECRET_PASSWORD, "home/arduino/$state", 2, 1, "lost", true)) {
    Serial.println("Retrying to connect to MQTT broker... ");
    delay(5000);
  }

  mqttClient.publish("home/arduino/$state", "init", true);

  Serial.println("Initializing MKR ENV shield...");
  if (!ENV.begin()) {
    Serial.println("Failed to initialize MKR ENV shield!");
    mqttClient.publish("home/arduino/$state", "alert", true);
    while (1);
  }

  mqttClient.publish("home/arduino/$homie", "4.0.0", true);
  mqttClient.publish("home/arduino/$name", "arduino mkr1000", true);
  mqttClient.publish("home/arduino/$nodes", "mkrenv,sd", true);
  mqttClient.publish("home/arduino/$extensions", "", true);
  mqttClient.publish("home/arduino/$ip", buffer, true);
  mqttClient.publish("home/arduino/mkrenv/$name", "mkr env shield", true);
  mqttClient.publish("home/arduino/mkrenv/$type", "", true);
  mqttClient.publish("home/arduino/mkrenv/$properties", "temperature,humidity,pressure,uva,uvb,uvindex,sd", true);
  mqttClient.publish("home/arduino/mkrenv/temperature/$name", "Temperature", true);
  mqttClient.publish("home/arduino/mkrenv/temperature/$datatype", "float", true);
  mqttClient.publish("home/arduino/mkrenv/temperature/$unit", "ºC", true);
  mqttClient.publish("home/arduino/mkrenv/humidity/$name", "Humidity", true);
  mqttClient.publish("home/arduino/mkrenv/humidity/$datatype", "float", true);
  mqttClient.publish("home/arduino/mkrenv/humidity/$unit", "%", true);
  mqttClient.publish("home/arduino/mkrenv/pressure/$name", "Pressure", true);
  mqttClient.publish("home/arduino/mkrenv/pressure/$datatype", "float", true);
  mqttClient.publish("home/arduino/mkrenv/pressure/$unit", "kPa", true);
  mqttClient.publish("home/arduino/mkrenv/uva/$name", "UVA", true);
  mqttClient.publish("home/arduino/mkrenv/uva/$datatype", "float", true);
  mqttClient.publish("home/arduino/mkrenv/uva/$unit", "", true);
  mqttClient.publish("home/arduino/mkrenv/uvb/$name", "UVB", true);
  mqttClient.publish("home/arduino/mkrenv/uvb/$datatype", "float", true);
  mqttClient.publish("home/arduino/mkrenv/uvb/$unit", "", true);
  mqttClient.publish("home/arduino/mkrenv/uvindex/$name", "UV Index", true);
  mqttClient.publish("home/arduino/mkrenv/uvindex/$datatype", "float", true);
  mqttClient.publish("home/arduino/mkrenv/uvindex/$unit", "", true);
//  mqttClient.publish("home/arduino/sd/$name", "micro-SD", true);
//  mqttClient.publish("home/arduino/sd/$type", "8GB", true);
//  mqttClient.publish("home/arduino/sd/$properties", "state", true);
//  mqttClient.publish("home/arduino/sd/state/$name", "state", true);
//  mqttClient.publish("home/arduino/sd/state/$datatype", "enum", true);
//  mqttClient.publish("home/arduino/sd/state/$format", "ready,missing,error,writing", true);


//  Serial.println("Initializing SD card...");
//  if (!SD.begin(4)) {
//    Serial.println("initialization failed!");
//    mqttClient.publish("home/arduino/sd/state", "missing", true);
//  } else {
//    mqttClient.publish("home/arduino/sd/state", "ready", true);
//    insertedSD = 1;
//    Serial.println("initialization done.");
//  }
  mqttClient.publish("home/arduino/$state", "ready", true);
}

void loop() {
//  if (insertedSD && ((prevTime1 + timeSD) < millis())) {
//    getENVValues();
//    prevTime1 = millis();
//    if (NumberOfLogs(fileName) >= trigger) {
//      Serial.print("Deleting oldest logs.....");
//      mqttClient.publish("home/arduino/sd/state", "writing", true);
//      RemoveOldLogs(fileName, trigger, logsToRemove);
//      mqttClient.publish("home/arduino/sd/state", "ready", true);
//      Serial.println("done.");
//    }
//  }

  if ((prevTime2 + timeMQTT) < millis()) {
    getENVValues(2);
    prevTime2 = millis();
  }

  mqttClient.loop();
}

void getENVValues(int marca) {
  // read all the sensor values
  float temperature = ENV.readTemperature();
  float humidity    = ENV.readHumidity();
  float pressure    = ENV.readPressure();
  //  float illuminance = ENV.readIlluminance();
  float uva         = ENV.readUVA();
  float uvb         = ENV.readUVB();
  float uvIndex     = ENV.readUVIndex();
  if (marca == 1) {
//    if (myFile = SD.open(fileName, FILE_WRITE)) {
//      Serial.print("Writing to ");
//      Serial.print(fileName);
//      Serial.print(" .....");
//      unsigned int prueba = millis();
//      myFile.print(int(millis() / 1000.0));
//      myFile.print(" s ");
//      myFile.print(temperature);
//      myFile.print(" °C ");
//      myFile.print(humidity);
//      myFile.print(" % ");
//      myFile.print(pressure);
//      myFile.print(" kPa ");
//      myFile.print(uva);
//      myFile.print(" UVA ");
//      myFile.print(uvb);
//      myFile.print(" UVB ");
//      myFile.print(uvIndex);
//      myFile.println(" UVIndex");
//      myFile.close();
//      Serial.println("done.");
//      mqttClient.publish("home/arduino/sd/state", "ready", true);
//    } else {
      Serial.println("error opening test.txt");
      mqttClient.publish("home/arduino/sd/state", "error", true);
//    }
  }
  if (marca == 2) {
    snprintf(buffer, 20, "%f", temperature);
    mqttClient.publish("home/arduino/mkrenv/temperature", buffer, true);
    snprintf(buffer, 20, "%f", humidity);
    mqttClient.publish("home/arduino/mkrenv/humidity", buffer, true);
    snprintf(buffer, 20, "%f", pressure);
    mqttClient.publish("home/arduino/mkrenv/pressure", buffer, true);
    snprintf(buffer, 20, "%f", uva);
    mqttClient.publish("home/arduino/mkrenv/uva", buffer, true);
    snprintf(buffer, 20, "%f", uvb);
    mqttClient.publish("home/arduino/mkrenv/uvb", buffer, true);
    snprintf(buffer, 20, "%f", uvIndex);
    mqttClient.publish("home/arduino/mkrenv/uvindex", buffer, true);
  }
}

void printWiFiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  Serial.print("IP: ");
  Serial.println(ip);

  Serial.print("signal strength (RSSI):");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");
}
