/******************************************
 *
 * This example works for both Industrial and STEM users.
 * If you are using the old educational platform,
 * please consider to migrate your account to a STEM plan
 *
 * Developed by Jose Garcia, https://github.com/jotathebest/
 *
 * ****************************************/

/****************************************
 * Include Libraries
 ****************************************/
#include "UbidotsEsp32Mqtt.h"

/****************************************
 * Define Constants
 ****************************************/
const char* UBIDOTS_TOKEN = "";  // Put here your Ubidots TOKEN
const char* WIFI_SSID = "";      // Put here your Wi-Fi SSID
const char* WIFI_PASS = "";      // Put here your Wi-Fi password

Ubidots ubidots(UBIDOTS_TOKEN);

/****************************************
 * Auxiliar variables
 ****************************************/
unsigned long timer;

/****************************************
 * Auxiliar Functions
 ****************************************/

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void publishToUbidots() {
  if (!ubidots.connected()) {
    ubidots.connect();
  }
  if (ubidots.connected()) {
    Serial.println("connected");
    // Publish values to 2 different data sources

    ubidots.add("stuff", 10.2);  // Insert your variable Labels and the value to be sent
    ubidots.publish("source1");
    ubidots.add("stuff", 10.2);
    ubidots.add("more-stuff", 120.2);
    ubidots.publish("source2");
  } else {
    Serial.println("could not connect");
    ubidots.disconnect();
  }
}

/****************************************
 * Main Functions
 ****************************************/

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  ubidots.connectToWifi(WIFI_SSID, WIFI_PASS);
  // ubidots.setDebug(true);  // uncomment this to make debug messages available
  ubidots.setCallback(callback);
  ubidots.setup();
  ubidots.reconnect();
  timer = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (abs(millis() - timer) > 5000) {  // triggers the routine every 5 seconds
    publishToUbidots();
    timer = millis();
  }
  if (ubidots.connected()) {
    ubidots.loop();
  }
}
