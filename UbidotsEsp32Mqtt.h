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
Created by: Jose Garcia @jotathebest at github: https://github.com/jotathebest
*/

#ifndef UbidotsESP32MQTT_H
#define UbidotsESP32MQTT_H
#include <PubSubClient.h>
#include <WiFi.h>

#include "UbiConstants.h"
#include "UbiTypes.h"

class Ubidots {
 private:
  // void (*callback)(char*, uint8_t*, unsigned int);
  WiFiClient _client_tcp_ubi;
  PubSubClient _client_mqtt_ubi = PubSubClient(_client_tcp_ubi);
  char _clientName[50];
  bool _debug = false;
  uint8_t _currentValue;
  char* _ssidPassword;
  char* _ssid;
  char _token[50];
  void _initInstance(const char* token, const char* clientName, const char* broker, const int brokerPort);
  Value* _dotValue;

 public:
  Ubidots(const char* token);
  Ubidots(const char* token, const char* clientName);
  Ubidots(const char* token, const char* broker, const int brokerPort);
  Ubidots(const char* token, const char* clientName, const char* broker);
  Ubidots(const char* token, const char* clientName, const char* broker, const int brokerPort);
  // void add(const char* variableLabel, float value);
  // void add(const char* variableLabel, float value, const char* context);
  // void add(const char* variableLabel, float value, const char* context, unsigned long dot_timestamp_seconds);
  // void add(const char* variableLabel, float value, const char* context, unsigned long dot_timestamp_seconds,
  //          unsigned int dot_timestamp_millis);
  bool connected();
  bool connect();
  bool connect(const char* username, const char* password);
  bool connect(const char* clientName, const char* username, const char* password);
  void connectToWifi(const char* ssid, const char* pass);
  void disconnect();
  bool loop();
  bool publish(const char* deviceLabel);
  void reconnect();
  void setDebug(bool debug);
  void setCallback(void (*callback)(char*, uint8_t*, unsigned int));
  bool subscribe(const char* topic);
  bool subscribeLastValue(const char* deviceLabel, const char* variableLabel);
};

#endif
