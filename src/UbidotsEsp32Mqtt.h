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
  WiFiClient _clientTcpUbi;
  PubSubClient _clientMqttUbi = PubSubClient(_clientTcpUbi);
  char* _broker;
  ContextUbi* _context;
  char* _clientName;
  int _brokerPort;
  bool _debug = false;
  int8_t _currentContext = 0;
  uint8_t _currentValue;
  char* _ssidPassword;
  char* _ssid;
  char* _token;
  Value* _dotValue;
  void _floatToChar(char* strValue, float value);
  void _builder(const char* token, const char* clientName, const char* broker, const int brokerPort);

 public:
  Ubidots(const char* token);
  Ubidots(const char* token, const char* clientName);
  Ubidots(const char* token, const char* broker, const int brokerPort);
  Ubidots(const char* token, const char* clientName, const char* broker);
  Ubidots(const char* token, const char* clientName, const char* broker, const int brokerPort);
  ~Ubidots();
  void add(const char* variableLabel, float value);
  void add(const char* variableLabel, float value, char* context);
  void add(const char* variableLabel, float value, char* context, unsigned long dotTimestampSeconds);
  void add(const char* variableLabel, float value, char* context, unsigned long dotTimestampSeconds,
           unsigned int dotTimestampMillis);
  void addContext(char* keyLabel, char* keyValue);
  bool connected();
  bool connect();
  bool connect(const char* username, const char* password);
  bool connect(const char* clientName, const char* username, const char* password);
  void connectToWifi(const char* ssid, const char* pass);
  void disconnect();
  void getContext(char* contextResult);
  bool loop();
  bool publish();
  bool publish(const char* deviceLabel);
  void reconnect();
  void setDebug(bool debug);
  void setCallback(void (*callback)(char*, uint8_t*, unsigned int));
  void setup();
  bool subscribe(const char* topic);
  bool subscribeLastValue(const char* deviceLabel, const char* variableLabel);
};

#endif
