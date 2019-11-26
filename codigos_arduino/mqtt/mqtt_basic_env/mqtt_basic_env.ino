/*
  Basic MQTT example
  Fuente: 
  https://programarfacil.com/esp8266/mqtt-esp8266-raspberry-pi/
  

  Modificado por: Luis Salgueiro
  Date: 19/Nov/2019


*/

#include <WiFi101.h>
#include <SPI.h>
#include <PubSubClient.h>
#include <Arduino_MKRENV.h>


// Update these with values suitable for your network.

const char* ssid = "PI_test01";//"vodafone7E0C";
const char* password = "123456789";//"VVQDMRFD7FBXF8";
//const char* mqtt_server = "test.mosquitto.org";
IPAddress mqtt_server(192,168,4,7);  // numeric IP for Google (no DNS)


const char* topicName = "MKR_ENV";
const char* topicNameTemp = "MKR_ENV/sensor1/temp";
const char* topicNameHum = "MKR_ENV/sensor2/hum";
const char* topicNamePress = "MKR_ENV/sensor1/press";
const char* topicNameIllu = "MKR_ENV/sensor2/illu";
const char* topicNameUva = "MKR_ENV/sensor1/uva";
const char* topicNameUvb = "MKR_ENV/sensor2/uvb";
const char* topicNameUV = "MKR_ENV/sensor1/UV";



WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
  pinMode(6, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  if (!ENV.begin()) {
    Serial.println("Failed to initialize MKR ENV shield!");
    while (1);
  }

  
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(5000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);
  delay(15000);
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  /*if ((char)payload[0] == '1') {
    digitalWrite(6, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else {
    digitalWrite(6, HIGH);  // Turn the LED off by making the voltage HIGH
  }*/

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ARduino")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(topicName, "Reconnect....  NEW CONNECTION");
//      delay(4000);
      // ... and resubscribe
      //client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  float temperature = ENV.readTemperature();
  float humidity    = ENV.readHumidity();
  float pressure    = ENV.readPressure();
  float illuminance = ENV.readIlluminance();
  float uva         = ENV.readUVA();
  float uvb         = ENV.readUVB();
  float uvIndex     = ENV.readUVIndex();

  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    snprintf (msg, 75, "***LECTURA #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish(topicName, msg);

    snprintf (msg, 75, "TEMPERATURA #%.2f", temperature);
//    Serial.print("Publish temperature: ");
//    Serial.println(msg);
    client.publish(topicNameTemp, msg);

    snprintf (msg, 75, "Humidity #%.2f", humidity);
    client.publish(topicNameHum, msg);
    
    snprintf (msg, 75, "Pressure #%.2f", pressure);
    client.publish(topicNamePress, msg);

    snprintf (msg, 75, "Illuminance #%.2f", illuminance);
    client.publish(topicNameIllu, msg);

    snprintf (msg, 75, "UVA #%.2f", uva);
    client.publish(topicNameUva, msg);
    snprintf (msg, 75, "UVB #%.2f", uvb);
    client.publish(topicNameUvb, msg);
    snprintf (msg, 75, "UV Index #%.2f", uvIndex);
    client.publish(topicNameUV, msg);

    snprintf (msg, 75, "***END LECTURA #%ld *****", value);
    client.publish(topicName, msg);



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

  Serial.print("Illuminance = ");
  Serial.print(illuminance);
  Serial.println(" lx");

  Serial.print("UVA         = ");
  Serial.println(uva);

  Serial.print("UVB         = ");
  Serial.println(uvb);

  Serial.print("UV Index    = ");
  Serial.println(uvIndex);

  // print an empty line
  Serial.println();
  //Serial.println(ip);





    


    
  }
}
