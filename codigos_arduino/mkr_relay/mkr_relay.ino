//Relay v0.0.1

#include <SPI.h>
#include <WiFi101.h>
#include <PubSubClient.h>
#include "arduino_secrets.h"   //please enter your sensitive data in the Secret tab/arduino_secrets.h

void callback(char* topic, byte* payload, unsigned int length);

char ssid[] = SECRET_SSID;     // your network SSID (name)
char pass[] = SECRET_PASS;     // your network password

int status = WL_IDLE_STATUS;
IPAddress server(192, 168, 4, 1); //MQTT Broker ip
int port = 1883;
WiFiClient client;
PubSubClient mqttClient(server, 1883, callback, client);

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

  while (!mqttClient.connect("arduinoClient", SECRET_USERNAME, SECRET_PASSWORD, "home/arduino2/$state", 2, 1, "lost", true)) {
    Serial.println("Retrying to connect to MQTT broker... ");
    delay(5000);
  }

  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  digitalWrite(1, LOW);
  digitalWrite(2, LOW);

  mqttClient.publish("home/arduino2/$homie", "4.0.0", true);
  mqttClient.publish("home/arduino2/$name", "arduino mkr1000", true);
  mqttClient.publish("home/arduino2/$nodes", "relayshield", true);
  mqttClient.publish("home/arduino2/$extensions", "", true);
  mqttClient.publish("home/arduino2/$ip", buffer, true);
  mqttClient.publish("home/arduino2/relayshield/$name", "mkr relayshield", true);
  mqttClient.publish("home/arduino2/relayshield/$type", "", true);
  mqttClient.publish("home/arduino2/relayshield/$properties", "relay1,relay2", true);
  mqttClient.publish("home/arduino2/relayshield/relay1/$name", "relay1", true);
  mqttClient.publish("home/arduino2/relayshield/relay1/$datatype", "boolean", true);
  mqttClient.publish("home/arduino2/relayshield/relay1/$settable", "true", true);
  mqttClient.publish("home/arduino2/relayshield/relay2/$name", "relay2", true);
  mqttClient.publish("home/arduino2/relayshield/relay2/$datatype", "boolean", true);
  mqttClient.publish("home/arduino2/relayshield/relay2/$settable", "true", true);
  mqttClient.publish("home/arduino2/relayshield/relay1", "false", true);
  mqttClient.publish("home/arduino2/relayshield/relay2", "false", true);
  mqttClient.subscribe("home/arduino2/relayshield/relay1/set");
  mqttClient.subscribe("home/arduino2/relayshield/relay2/set");
  mqttClient.publish("home/arduino2/$state", "ready", true);
}

void loop() {
  mqttClient.loop();
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

void callback(char* topic, byte* payload, unsigned int length) {
  String s = "";
  for (int i = 0; i < length; i++) {
    s = s + (char)payload[i];
  }
  if (topic[31] == '1') {
    if (s == "true") {
      digitalWrite(1, HIGH);
      Serial.println("power on relay 1");
      mqttClient.publish("home/arduino2/relayshield/relay1", "true", true);
    } else if (s == "false") {
      digitalWrite(1, LOW);
      Serial.println("poweroff on relay 1");
      mqttClient.publish("home/arduino2/relayshield/relay1", "false", true);
    } else {
      Serial.println("invalid payload recieved on topic home/arduino2/relayshield/relay1/set");
    }
  } else {
    if (s == "true") {
      digitalWrite(2, HIGH);
      Serial.println("power on relay 2");
      mqttClient.publish("home/arduino2/relayshield/relay2", "true", true);
    } else if (s == "false") {
      digitalWrite(2, LOW);
      Serial.println("poweroff on relay 2");
      mqttClient.publish("home/arduino2/relayshield/relay2", "false", true);
    } else {
      Serial.println("invalid payload recieved on topic home/arduino2/relayshield/relay2/set");
    }
  }
}
