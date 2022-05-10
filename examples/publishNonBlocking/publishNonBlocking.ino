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
const char *UBIDOTS_TOKEN = "";  // Put here your Ubidots TOKEN
const char *WIFI_SSID = "";      // Put here your Wi-Fi SSID
const char *WIFI_PASS = "";      // Put here your Wi-Fi password
const char *DEVICE_LABEL = "";   // Put here your Device label to which data  will be published
const char *VARIABLE_LABEL = ""; // Put here your Variable label to which data  will be published

const int PUBLISH_FREQUENCY = 5000; // Update rate in milliseconds

uint8_t analogPin = 34; // Pin used to read data from GPIO34 ADC_CH6.

Ubidots ubidots(UBIDOTS_TOKEN);

/****************************************
 * Auxiliar variables
 ****************************************/
unsigned long timer;

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

void publishToUbidots()
{
  if (!ubidots.connected())
  {
    ubidots.connect();
  }
  if (ubidots.connected())
  {
    Serial.println("connected");
    // Publish values to the Device label
    float value = analogRead(analogPin);
    ubidots.add(VARIABLE_LABEL, value); // Insert your variable Labels and the value to be sent
    ubidots.publish(DEVICE_LABEL);
  }
  else
  {
    Serial.println("could not connect");
    ubidots.disconnect();
  }
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
  ubidots.connect();
  timer = millis();
}

void loop()
{
  // put your main code here, to run repeatedly:
  if ((millis() - timer) > PUBLISH_FREQUENCY)
  { // triggers the routine every PUBLISH_FREQUENCY milliseconds
    timer = millis();
    publishToUbidots();
  }
  if (ubidots.connected())
  {
    ubidots.loop();
  }
}
