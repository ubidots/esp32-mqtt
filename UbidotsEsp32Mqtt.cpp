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

/**************************************************************************
 * Overloaded constructors
 ***************************************************************************/

Ubidots::Ubidots(const char* token) {
  String _deviceMac = WiFi.macAddress();
  char* clientName = new char[_deviceMac.length() + 1];
  strcpy(clientName, _deviceMac.c_str());
  _builder(token, clientName, UBIDOTS_BROKER, UBIDOTS_MQTT_PORT);
}

Ubidots::Ubidots(const char* token, const char* clientName) {
  _builder(token, clientName, UBIDOTS_BROKER, UBIDOTS_MQTT_PORT);
}

Ubidots::Ubidots(const char* token, const char* broker, const int brokerPort) {
  String _deviceMac = WiFi.macAddress();
  char* clientName = new char[_deviceMac.length() + 1];
  strcpy(clientName, _deviceMac.c_str());
  _builder(token, clientName, broker, brokerPort);
}

Ubidots::Ubidots(const char* token, const char* clientName, const char* broker) {
  _builder(token, clientName, broker, UBIDOTS_MQTT_PORT);
}

Ubidots::Ubidots(const char* token, const char* clientName, const char* broker, const int brokerPort) {
  _builder(token, clientName, broker, brokerPort);
}

/**************************************************************************
 * Overloaded destructor
 ***************************************************************************/

Ubidots::~Ubidots() {
  free(_dotValue);
  _client_mqtt_ubi.disconnect();
}

void Ubidots::_builder(const char* token, const char* clientName, const char* broker, const int brokerPort) {
  _brokerPort = brokerPort;
  _broker = strdup(broker);
  _token = strdup(token);
  _clientName = strdup(clientName);
  _dotValue = (Value*)malloc(MAX_VALUES * sizeof(Value));
  _context = (ContextUbi*)malloc(MAX_VALUES * sizeof(ContextUbi));
}

void Ubidots::setup() {
  Serial.print("broker:");
  Serial.println(_broker);
  Serial.print("brokerPort:");
  Serial.println(_brokerPort);
  Serial.println(_clientName);
  Serial.println(_token);
  _client_mqtt_ubi.setServer(_broker, _brokerPort);
}

/***************************************************************************
FUNCTIONS TO SEND/RETRIEVE DATA
***************************************************************************/

/**
 * Retrieves data from Ubidots by subscribing to a custom topic
 * @arg topic [Mandatory] topic to be subscribed
 */
bool Ubidots::subscribe(const char* topic) { return _client_mqtt_ubi.subscribe(topic); }

/**
 * Retrieves data from Ubidots by subscribing to an lv topic
 * @arg deviceLabel [Mandatory] device label where the dot is stored
 * @arg variableLabel [Mandatory] variable label where the dot is stored
 */

bool Ubidots::subscribeLastValue(const char* deviceLabel, const char* variableLabel) {
  char topic[MAX_BUFFER_SIZE];
  sprintf(topic, "/v2.0/devices/%s/%s/lv", deviceLabel, variableLabel);
  Serial.print("Subscribing to");
  Serial.println(topic);
  return subscribe(topic);
}

/**
 * Sends data to Ubidots
 * @arg device_label [Mandatory] device label where the dot will be stored
 */

bool Ubidots::publish() { return publish(_clientName); }

bool Ubidots::publish(const char* deviceLabel) {
  char topic[150];
  char payload[500];
  sprintf(topic, "/v2.0/devices/%s", deviceLabel);
  sprintf(payload, "{");
  for (uint8_t i = 0; i <= _currentValue;) {
    char str_value[20];
    _floatToChar(str_value, (_dotValue + i)->dotValue);
    sprintf(payload, "%s\"%s\": [{\"value\": %s", payload, (_dotValue + i)->variableLabel, str_value);
    if ((_dotValue + i)->dotTimestamp != NULL) {
      sprintf(payload, "%s, \"timestamp\": %s", payload, (_dotValue + i)->dotTimestamp);
    }
    if (strcmp((_dotValue + i)->dotContext, "NULL") != 0) {
      sprintf(payload, "%s, \"context\": {%s}", payload, (_dotValue + i)->dotContext);
    }
    i++;
    if (i >= _currentValue) {
      sprintf(payload, "%s}]}", payload);
      break;
    } else {
      sprintf(payload, "%s}], ", payload);
    }
  }
  if (_debug) {
    Serial.println("publishing to TOPIC: ");
    Serial.println(topic);
    Serial.print("JSON dict: ");
    Serial.println(payload);
  }
  _currentValue = 0;
  return _client_mqtt_ubi.publish(topic, payload, MAX_BUFFER_SIZE);
}

/***************************************************************************
FUNCTIONS TO MANAGE SOCKET CONNECTION
***************************************************************************/

/**
 * returns true if the ESP32 is connected to the broker
 */
bool Ubidots::connected() { return _client_mqtt_ubi.connected(); };

/**
 * Overloaded connect() methods.
 * Connects to the broker using a custom username and password
 * @arg clientName [Optional] Unique MQTT client id
 * @arg username [Mandatory] MQTT username to be identified by the broker
 * @arg password [Mandatory] MQTT password to be identified by the broker
 */
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

/**
 * Disconnects gracefully from the broker, closing the socket
 */
void Ubidots::disconnect() { _client_mqtt_ubi.disconnect(); };

/**
 * Maintains the socket connection and sends periodically the keep alive command
 */
bool Ubidots::loop() { return _client_mqtt_ubi.loop(); };

/**
 * Attempts to reconnect to the server using as password and username the Ubidots token
 * This is a blocking function
 */
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

/***************************************************************************
AUXILIAR FUNCTIONS
***************************************************************************/

/**
 * Add a value of variable to save
 * @arg variable_label [Mandatory] variable label where the dot will be stored
 * @arg value [Mandatory] Dot value
 * @arg context [optional] Dot context to store. Default NULL
 * @arg dot_timestamp_seconds [optional] Dot timestamp in seconds, usefull for
 * datalogger. Default NULL
 * @arg dot_timestamp_millis [optional] Dot timestamp in millis to add to
 * dot_timestamp_seconds, usefull for datalogger.
 */

void Ubidots::add(const char* variableLabel, float value) { add(variableLabel, value, "NULL", NULL, NULL); }

void Ubidots::add(const char* variableLabel, float value, char* context) {
  add(variableLabel, value, context, NULL, NULL);
}

void Ubidots::add(const char* variableLabel, float value, char* context, unsigned long dot_timestamp_seconds) {
  add(variableLabel, value, context, dot_timestamp_seconds, NULL);
}

void Ubidots::add(const char* variableLabel, float value, char* context, unsigned long dot_timestamp_seconds,
                  unsigned int dot_timestamp_millis) {
  (_dotValue + _currentValue)->variableLabel = variableLabel;
  (_dotValue + _currentValue)->dotValue = value;
  (_dotValue + _currentValue)->dotContext = context;
  sprintf((_dotValue + _currentValue)->dotTimestamp, "%s", "NULL");
  char milliseconds[3];
  sprintf(milliseconds, "%s", "000");
  if (dot_timestamp_millis != NULL) {
    uint8_t units = dot_timestamp_millis % 10;
    uint8_t dec = (dot_timestamp_millis / 10) % 10;
    uint8_t hund = (dot_timestamp_millis / 100) % 10;
    sprintf(milliseconds, "%d%d%d", hund, dec, units);
  }
  if (dot_timestamp_seconds != NULL) {
    sprintf((_dotValue + _currentValue)->dotTimestamp, "%lu%s", dot_timestamp_seconds, milliseconds);
  }
  _currentValue++;
  if (_currentValue > MAX_VALUES) {
    if (_debug) {
      Serial.println(F("You are sending more than the maximum of consecutive variables"));
    }
    _currentValue = MAX_VALUES;
  }
}

/*
 * Adds to the context structure values to retrieve later it easily by the user
 */

void Ubidots::addContext(char* key_label, char* key_value) {
  (_context + _current_context)->key_label = key_label;
  (_context + _current_context)->key_value = key_value;
  _current_context++;
  if (_current_context >= MAX_VALUES) {
    Serial.println(
        F("You are adding more than the maximum of consecutive "
          "key-values pairs"));
    _current_context = MAX_VALUES;
  }
}

/*
 * Retrieves the actual stored context properly formatted
 */

void Ubidots::getContext(char* context_result) {
  sprintf(context_result, "");
  for (uint8_t i = 0; i < _current_context;) {
    sprintf(context_result, "%s%s=%s", context_result, (_context + i)->key_label, (_context + i)->key_value);
    i++;
    if (i < _current_context) {
      sprintf(context_result, "%s$", context_result);
    } else {
      sprintf(context_result, "%s", context_result);
      _current_context = 0;
    }
  }
}

/**
 * Sets the callback to be used to process the data incoming from the subscribed topics
 * @arg callback [Mandatory] Pointer to the callback function that will process the incoming data
 */
void Ubidots::setCallback(void (*callback)(char*, uint8_t*, unsigned int)) { _client_mqtt_ubi.setCallback(callback); }

/**
 * Makes available the debug messages
 */
void Ubidots::setDebug(bool debug) { _debug = debug; };

/**
 * Connects to the WiFi network. This method supports just WAP and WAP2 connections
 * @arg ssid [Mandatory] SSID of the network to connect to
 * @arg pass [Mandatory] WiFi network password
 */
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

/**
 * Converts from float to char, returning the less possible length char array with the
 * float value conversion
 * @arg str_value [Mandatory] Pointer to the char array that will store the result
 * @arg value [Mandatory] float type value
 */
void Ubidots::_floatToChar(char* str_value, float value) {
  char temp_arr[20];
  sprintf(temp_arr, "%17g", value);
  uint8_t j = 0;
  uint8_t k = 0;
  while (j < 20) {
    if (temp_arr[j] != ' ') {
      str_value[k] = temp_arr[j];
      k++;
    }
    if (temp_arr[j] == '\0') {
      str_value[k] = temp_arr[j];
      break;
    }
    j++;
  }
}
