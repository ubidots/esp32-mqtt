/*
Copyright (c) 2021 Ubidots.
Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:
The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
Created by: Jose Garcia
*/

#include "UbidotsEsp32Mqtt.h"

Ubidots::Ubidots(const char* token) {
  String _deviceMac = WiFi.macAddress();
  char* clientName = new char[_deviceMac.length() + 1];
  strcpy(clientName, _deviceMac.c_str());
  _initInstance(token, clientName, UBIDOTS_BROKER, UBIDOTS_MQTT_PORT);
}

Ubidots::Ubidots(const char* token, const char* clientName) {
  _initInstance(token, clientName, UBIDOTS_BROKER, UBIDOTS_MQTT_PORT);
}

Ubidots::Ubidots(const char* token, const char* broker, const int brokerPort) {
  String _deviceMac = WiFi.macAddress();
  char* clientName = new char[_deviceMac.length() + 1];
  strcpy(clientName, _deviceMac.c_str());
  _initInstance(token, clientName, broker, brokerPort);
}

Ubidots::Ubidots(const char* token, const char* clientName, const char* broker) {
  _initInstance(token, clientName, broker, UBIDOTS_MQTT_PORT);
}

Ubidots::Ubidots(const char* token, const char* clientName, const char* broker, const int brokerPort) {
  _initInstance(token, clientName, broker, brokerPort);
}

void Ubidots::_initInstance(const char* token, const char* clientName, const char* broker, const int brokerPort) {
  sprintf(_token, "%s", token);
  sprintf(_clientName, "%s", clientName);
  Serial.print("broker:");
  Serial.println(broker);
  Serial.print("brokerPort:");
  Serial.println(brokerPort);
  _client_mqtt_ubi.setServer(broker, brokerPort);
}

// void Ubidots::add(const char* variableLabel, float value) { add(variableLabel, value, "NULL", NULL, NULL); }

// void Ubidots::add(const char* variableLabel, float value, const char* context) {
//   add(variableLabel, value, context, NULL, NULL);
// }

// void Ubidots::add(const char* variableLabel, float value, const char* context, unsigned long dot_timestamp_seconds) {
//   add(variableLabel, value, context, dot_timestamp_seconds, NULL);
// }

// void Ubidots::add(const char* variableLabel, float value, const char* context, unsigned long dot_timestamp_seconds,
//                   unsigned int dot_timestamp_millis) {
//   (_dotValue + _currentValue)->_variableLabel = variableLabel;
//   (_dotValue + _currentValue)->_value = value;
//   (_dotValue + _currentValue)->_context = context;
//   (_dotValue + _currentValue)->dotTimestamp = NULL;
//   char milliseconds[3];
//   sprintf(milliseconds, "000");
//   if (dot_timestamp_millis != NULL) {
//     uint8_t units = dot_timestamp_millis % 10;
//     uint8_t dec = (dot_timestamp_millis / 10) % 10;
//     uint8_t hund = (dot_timestamp_millis / 100) % 10;
//     sprintf(milliseconds, "%d%d%d", hund, dec, units);
//   }
//   if (dot_timestamp_seconds != NULL) {
//     char dotTimestamp[14];
//     sprintf(dotTimestamp, "%lu%s\0", dot_timestamp_seconds, milliseconds);
//     (_dotValue + _currentValue)->dotTimestamp = dotTimestamp;
//   }
//   _currentValue++;
//   if (_currentValue > MAX_VALUES) {
//     Serial.println(F("You are sending more than the maximum of consecutive variables"));
//     _currentValue = MAX_VALUES;
//   }
// }

bool Ubidots::connected() { return _client_mqtt_ubi.connected(); };
bool Ubidots::connect() { return connect(_clientName, _token, _token); }
bool Ubidots::connect(const char* username, const char* password) { return connect(_clientName, username, password); }
bool Ubidots::connect(const char* clientName, const char* username, const char* password) {
  bool result = _client_mqtt_ubi.connect(clientName, username, password);
  if (_debug) {
    Serial.println("attempting to connect");
    if (!result) {
      Serial.print("failed, rc=");
      Serial.print(_client_mqtt_ubi.state());
    }
  }
  return result;
}
void Ubidots::disconnect() { _client_mqtt_ubi.disconnect(); };
bool Ubidots::loop() { return _client_mqtt_ubi.loop(); };
bool Ubidots::subscribe(const char* topic) { return _client_mqtt_ubi.subscribe(topic); }
bool Ubidots::subscribeLastValue(const char* deviceLabel, const char* variableLabel) {
  char topic[1];
  sprintf(topic, "/v1.6/devices/%s/%s/lv", deviceLabel, variableLabel);
  Serial.print("Subscribing to");
  Serial.println(topic);
  return subscribe(topic);
}
bool Ubidots::publish(const char* deviceLabel) { return true; };
void Ubidots::reconnect() {
  while (!_client_mqtt_ubi.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (_client_mqtt_ubi.connect(_clientName, _token, _token)) {
      Serial.println("connected");
      break;
    } else {
      Serial.print("failed, rc=");
      Serial.print(_client_mqtt_ubi.state());
      Serial.println(" try again in 3 seconds");
      delay(3000);
    }
  }
}
void Ubidots::setCallback(void (*callback)(char*, uint8_t*, unsigned int)) { _client_mqtt_ubi.setCallback(callback); }
void Ubidots::setDebug(bool debug){};
void Ubidots::connectToWifi(const char* ssid, const char* pass) {
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(F("WiFi connected"));
  Serial.println(F("IP address: "));
  Serial.println(WiFi.localIP());
}
