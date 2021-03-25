/******************************************
 *
 * This example works for both Industrial and STEM users.
 * If you are using the old educational platform,
 * please consider to migrate your account to a STEM plan
 *
 * ****************************************/

/****************************************
 * Include Libraries
 ****************************************/
#include "UbidotsEsp32Mqtt.h"

/****************************************
 * Define Constants
 ****************************************/
const char* UBIDOTS_TOKEN = "";                               // Put here your Ubidots TOKEN
const char* WIFI_SSID = "";                                   // Put here your Wi-Fi SSID
const char* WIFI_PASS = "";                                   // Put here your Wi-Fi password
const char* DEVICE_LABEL_TO_RETRIEVE_VALUES_FROM = "demo";    // Replace with your device label
const char* VARIABLE_LABEL_TO_RETRIEVE_VALUES_FROM = "demo";  // Replace with your variable label
#define LED 5

Ubidots ubidots(UBIDOTS_TOKEN);

/****************************************
 * Auxiliar Functions
 ****************************************/

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    if ((char)payload[0] == '1') {
      digitalWrite(LED, HIGH);
    } else {
      digitalWrite(LED, LOW);
    }
  }
  Serial.println();
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
  pinMode(LED, OUTPUT);
  ubidots.subscribeLastValue(DEVICE_LABEL_TO_RETRIEVE_VALUES_FROM,
                             VARIABLE_LABEL_TO_RETRIEVE_VALUES_FROM);  // Insert the dataSource and Variable's Labels
}

void loop() {
  // put your main code here, to run r  epeatedly:
  if (!ubidots.connected()) {
    ubidots.reconnect();
    ubidots.subscribeLastValue(DEVICE_LABEL_TO_RETRIEVE_VALUES_FROM,
                               VARIABLE_LABEL_TO_RETRIEVE_VALUES_FROM);  // Insert the dataSource and Variable's Labels
  }
  ubidots.loop();
  delay(5000);
}
