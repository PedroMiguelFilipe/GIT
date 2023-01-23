/* This example shows how to use MQTT on the main dev boards on the market
 * HOW TO USE:
 * under connect method, add your subscribe channels.
 * under messageReceived (callback method) add actions to be done when a msg is received.
 * to publish, call client.publish(topic,msg)
 * in loop take care of using non-blocking method or it will corrupt.
 * 
 * Alberto Perro - Officine Innesto 2019
 */
 
//#define BROKER_IP    "192.168.1.2"
#define BROKER_IP    "10.58.20.229"
#define DEV_NAME     "device_random_name"
#define MQTT_USER    "PedroFilipe"
#define MQTT_PW      "tese123"

//const char ssid[] = "NOS-BEC6";
//const char pass[] = "ELR9UNTM";

const char ssid[] = "LoRa_Lab_WiFi";
const char pass[] = "iscteloralab#1";

const int sensor = 12;
const int buzzer = 14;

// setting PWM properties
const int freq = 2000;
const int channel = 0;
const int resolution = 8;

int motion = 0;
boolean estadoPorta = false;

#include <MQTT.h>

#ifdef ARDUINO_SAMD_MKRWIFI1010
#include <WiFiNINA.h>
#elif ARDUINO_SAMD_MKR1000
#include <WiFi101.h>
#elif ESP8266
#include <ESP8266WiFi.h>
#elif ESP32
#include <WiFi.h>
#else
#error unknown board
#endif

WiFiClient net;
MQTTClient client;
unsigned long lastMillis = 0;

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!client.connect(DEV_NAME, MQTT_USER, MQTT_PW)) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nconnected!");
  //client.subscribe("/motion"); //SUBSCRIBE TO TOPIC /motion
  client.subscribe("/buzzer"); //SUBSCRIBE TO TOPIC /buzzer
  client.publish("/motion", "Fechada!");
}

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
  for (int i = 0; i<= 255; i = i + 1){
    ledcWriteTone(channel, 10000);
    delay(1);
  }
  ledcWriteTone(channel, 0);
  //ledcWriteTone(channel, 10000);
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);

  pinMode(sensor, INPUT);

  // configure LED PWM functionalitites
  ledcSetup(channel, freq, resolution);

  // attach the channel to the GPIO to be controlled
  ledcAttachPin(buzzer, channel);

  client.begin(BROKER_IP, 1883, net);
  client.onMessage(messageReceived);
  connect();
}

void loop() {
  client.loop();
  if (!client.connected()) {
    connect();
  }
  if (millis() - lastMillis > 5000){
    motion = digitalRead(sensor);
    //Serial.println(motion);
    if (motion == 1){
      lastMillis = millis();
      estadoPorta = !estadoPorta;
      if (estadoPorta){
      client.publish("/motion", "Aberta!");
     }else{
      client.publish("/motion", "Fechada!");
      }
    }
  }
}
