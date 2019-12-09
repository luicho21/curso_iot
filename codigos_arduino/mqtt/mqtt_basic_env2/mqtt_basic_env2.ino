#include <MQTT.h>
#include <MQTTClient.h>

/*
  MKR ENV Shield - Read Sensors

  This example reads the sensors on-board the MKR ENV shield
  and prints them to the Serial Monitor once a second.

  The circuit:
  - Arduino MKR board
  - Arduino MKR ENV Shield attached

  This example code is in the public domain.
*/
#include <SPI.h>
#include <WiFi101.h>
#include <Arduino_MKRENV.h>
#include <MQTT.h>
//#include <MQTTClient.h>

#include "arduino_secrets.h" 


///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;            // your network SSID (name)
char pass[] = SECRET_PASS;            // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;          // the WiFi radio's status
char mqttHost[] = SECRET_MQTT_HOST;   // your MQTT Host direction 
char mqttUser[] = SECRET_MQTT_USER;   // your MQTT user name
char mqttPass[] = SECRET_MQTT_PASS;   // youtr MQTT user password
char mqttID[] = SECRET_MQTT_ID;       // your device ID
// Device Information
char mqttDevHomie[] = DEVICE_HOMIE;
char mqttDevName[] = DEVICE_NAME;
char mqttDevNodes[] = DEVICE_NODES;
char mqttDevImpl[] = DEVICE_IMPLEMENTATION;
char mqttDevState[] = DEVICE_STATE;
//Node Information
char mqttNodeId[] = NODE_ID;
char mqttNodeName[] = NODE_NAME;
char mqttNodeType[] = NODE_TYPE;
char mqttNodeProp[] = NODE_PROPERTIES;


     // your device ID
// Initialize the client library
WiFiClient net;
MQTTClient client;

unsigned long lastMillis = 0;

const int TIME_OUT = 5000;

void setup() 
{
    //Initialize serial and wait for port to open:
    Serial.begin(9600);
    while (!Serial);

    if (!ENV.begin()) 
    {
        Serial.println("Failed to initialize MKR ENV shield!");
        while (true);
    }

    // check for the presence of the shield:
    if (WiFi.status() == WL_NO_SHIELD) 
    {
        printWifiStatus(WiFi.status());
        // don't continue:
        while (true);
    }
    WiFi.begin(ssid, pass);

    client.begin(mqttHost, net);
    client.onMessage(messageReceived);

    connect();
}

void loop() 
{
    // check the network connection once every loop:
    wifiConnection();
    // MQTT client
    client.loop();

    if (!client.connected()) 
    {
        connect();
    }
        // publish a message roughly every second.
    if (millis() - lastMillis > 5000) 
    {        
      // read all the sensor values
      float temperature = ENV.readTemperature();
      float humidity    = ENV.readHumidity();
      float pressure    = ENV.readPressure();
      float uva         = ENV.readUVA();
      float uvb         = ENV.readUVB();
      float uvIndex     = ENV.readUVIndex();

      printSerialSensorValues (temperature, humidity, pressure, uva, uvb, uvIndex);

        lastMillis = millis();
        publishDevice( mqttID, mqttDevHomie, mqttDevName, mqttDevNodes, mqttDevImpl, mqttDevState);
        publishNode( mqttID, mqttNodeId, mqttNodeName, mqttNodeType, mqttNodeProp);

        publishProperty(mqttID,mqttNodeId,"temperature","temperatura","float","false","true","ºC","-40:120",temperature);
        publishProperty(mqttID,mqttNodeId,"humidity","humitat","float","false","true","%","0:10",humidity);
        publishProperty(mqttID,mqttNodeId,"pressure","pressió atmosfèrica","float","false","true","hPa","260:1260",pressure);
        publishProperty(mqttID,mqttNodeId,"uva","uva","float","false","true","","",uva);
        publishProperty(mqttID,mqttNodeId,"uvb","uvb","float","false","true","","",uvb);
        publishProperty(mqttID,mqttNodeId,"uvindex","uvIndex","float","false","true","","1:11",uvIndex);

    }

}

void printSerialSensorValues (float temperature, float humidity, float pressure, float uva, float uvb, float uvIndex)
{
    // print each of the sensor values
    Serial.print("Temperature = ");
    Serial.print(temperature);
    Serial.println(" °C");

    Serial.print("Humidity    = ");
    Serial.print(humidity);
    Serial.println(" %");

    Serial.print("Pressure    = ");
    Serial.print(pressure);
    Serial.println(" kPa");

    Serial.print("UVA         = ");
    Serial.println(uva);

    Serial.print("UVB         = ");
    Serial.println(uvb);

    Serial.print("UV Index    = ");
    Serial.println(uvIndex);

    // print an empty line
    Serial.println();
}

void wifiConnection()
{
 // attempt to connect to WiFi network if it's not connected:
  if(WiFi.status() != WL_CONNECTED) {
    printWifiStatus(WiFi.status());
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    WiFi.begin(ssid, pass);
    while (WiFi.status() == WL_IDLE_STATUS);

    printWifiStatus(WiFi.status());
  }
  
}
void printWifiIP() 
{
  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}

void printCurrentNet() {
    // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.println(rssi);
    printWifiIP();

}

void printWifiStatus(int Status) {
  // print WiFi status:
  switch (Status) {
  case WL_CONNECTED:
    Serial.println("Connected to a WiFi network");
    printCurrentNet();
    break;
  case WL_NO_SHIELD:
    Serial.println("No wifi shield present");
    break;  
  case WL_IDLE_STATUS:
    Serial.println("Connecting to a WiFi network");
    break;
  case WL_NO_SSID_AVAIL:
    Serial.println("No SSID are avaible");
    break; 
  case WL_SCAN_COMPLETED:
    Serial.println("Scan completed");
    break;
  case WL_CONNECT_FAILED:
    Serial.println("Connection failed");
    break;         
  case WL_CONNECTION_LOST:
    Serial.println("Connection lost");
    break;
  case WL_DISCONNECTED:
    Serial.println("Disconnected network");
    break;
  default:
    break;
  }
}

void connect() 
{
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!client.connect(mqttID, mqttUser, mqttPass)) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

  client.subscribe("/hello");
  // client.unsubscribe("/hello");
}

void messageReceived(String &topic, String &payload)
{
  Serial.println("incoming: " + topic + " - " + payload);
}

void publishDevice( char device[]
                    ,char pHomie[]
                    , char pName[]
                    , char pNodes[]
                    , char pImplementation[]
                    , char pState[])
{
  String string = "homie/";
  string = string + device;

  client.publish(string + "/$homie", pHomie);
  client.publish(string + "/$name", pName);
  client.publish(string + "/$nodes", pNodes);
  client.publish(string + "/$implementation", pImplementation);
  client.publish(string + "/$state", pState);
}

void publishNode( char device[]
                , char node[]
                , char pName[]
                , char pType[]
                , char pProperties[])
{
  String string = "homie/";
  string = string + device + "/" + node ;

  client.publish(string + "/$name", pName);
  client.publish(string + "/$type", pType);
  client.publish(string + "/$properties", pProperties);
}

void publishProperty( char device[]
                    , char node[]
                    , char property[]
                    , char pName[]
                    , char pDataType[]
                    , char pSetTable[]
                    , char pRetained[]
                    , char pUnit[]
                    , char pFormat[]
                    , float value)
{
  String string = "homie/";
  string = string + device + "/" + node + "/" + property;

  client.publish(string , String(value));
  client.publish(string + "/$name", pName);
  client.publish(string + "/$datatype", pDataType);
  client.publish(string + "/$settable", pSetTable);
  client.publish(string + "/$retained", pRetained);
  client.publish(string + "/$unit", pUnit);
  client.publish(string + "/$format", pFormat);
}
