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
const char *UBIDOTS_TOKEN = "";                    // Put here your Ubidots TOKEN
const char *WIFI_SSID = "";                        // Put here your Wi-Fi SSID
const char *WIFI_PASS = "";                        // Put here your Wi-Fi password
const char *CUSTOM_TOPIC = "/v2.0/devices/demo/+"; // This will subscribe to all the messages received by the Device labeled as "demo"

Ubidots ubidots(UBIDOTS_TOKEN);

/****************************************
 * Auxiliar Functions
 ****************************************/

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

/****************************************
 * Main Functions
 ****************************************/

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  // ubidots.setDebug(true);  // uncomment this to make debug messages available
  ubidots.connectToWifi(WIFI_SSID, WIFI_PASS);
  ubidots.setCallback(callback);
  ubidots.setup();
  ubidots.reconnect();
  ubidots.subscribe(CUSTOM_TOPIC); // Insert the topic to subscribe to
}

void loop()
{
  // put your main code here, to run repeatedly:
  if (!ubidots.connected())
  {
    ubidots.reconnect();
    ubidots.subscribe(CUSTOM_TOPIC); // Insert the topic to subscribe to
  }
  ubidots.loop();
}
