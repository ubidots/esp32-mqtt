# ESP32 Dev module

The ESP32 is a microcontroller developed by [Espressif Systems](http://espressif.com/). Known as a WiFi Module, this microcontroller has the ability to perform WiFi related activities like Internet of things applications and home automation. Ranging in price and features, there are many types of ESP32 modules available, but all of them are incredibly useful for IoT world.

## Requirements

- [An ESP32 module](https://www.sparkfun.com/products/15663).
- [Arduino IDE 1.6.5 or higher](https://www.arduino.cc/en/Main/Software)
- [PubSubclient Library](https://github.com/knolleary/pubsubclient)
- [Ubidots library](https://github.com/ubidots/esp32/archive/main.zip)

## Setup

1. Download the [Arduino IDE](https://www.arduino.cc/en/Main/Software).
2. Install the ESP32 Platform to your Arduino IDE. Please reference the following links depending of your operating system. The following Espressif links will prompt you to install the ESP32 to your computer. 

**Note:** If you are using Linux or Mac operating systems, an additional step is required as you need to install the ESP32 Platform to the Arduino IDE from your computer's terminal first. Use the below links to execute the required Terminal functions and then return to this guide to continue the ESP32 integration.

- [Windows](https://github.com/espressif/arduino-esp32/blob/master/docs/arduino-ide/windows.md)
- [Debian/Ubuntu Linux](https://github.com/espressif/arduino-esp32/blob/master/docs/arduino-ide/debian_ubuntu.md)
- [MacOS](https://github.com/espressif/arduino-esp32/blob/master/docs/arduino-ide/mac.md)
3. After installing the ESP32 platform using the above Espressif Systems links, you will be prompted to restart the Arduino IDE - be sure to do so. 
5. Select the ESP32 board from **Tools > Board > ESP32 Dev module**.
6. Go to Sketch/Program -> Include Library -> Library Manager and install the PubSubClient library
6. Download the **Ubidots library** [here](https://github.com/ubidots/esp32-mqtt/archive/main.zip).
7. Now, click on **Sketch -> Include Library -> Add .ZIP Library**.
8. Select the .ZIP file of Ubidots and then "**Accept**".
9. Close the Arduino IDE and open it again.

**NOTE:** This library supports sending data over sockets without TLS only. Your data will not be encrypted. We encourage, if you are deploying devices to a production environment, to create your own firmware routines to send data securely or to isolate the network where the ESP32 will be connected.
We do not guarantee any issue, data miss or external sniff coming from the library usage or implementation.

# Documentation

## Constructor

### Ubidots

`Ubidots(const char* token)`
`Ubidots(const char* token, const char* clientName)`
`Ubidots(const char* token, const char* broker, const int brokerPort)`
`Ubidots(const char* token, const char* clientName, const char* broker)`
`Ubidots(const char* token, const char* clientName, const char* broker, const int brokerPort)`

Creates an Ubidots instance.

- @token, [Required]. Your Ubidots unique account [TOKEN](http://help.ubidots.com/user-guides/find-your-token-from-your-ubidots-account).
- @clientName, [Optional] [default] = device unique MAC address. The MQTT unique client id to be identified by the broker.
- @broker, [Optional] [default] = `industrial.api.ubidots.com`. The server to send data url.
- @brokerPort, [Optional], [Default] = 1883. The broker's available port to open MQTT sockets.

## Methods

`void add(const char *variable_label, float value, char *context, unsigned long dotTimestampSeconds, unsigned int dotTimestampMillis)`

Adds a dot with its related value, context and timestamp to be sent to a certain data source.

**Important:** The max payload length is 512 bytes, if your payload is greater it won't be properly sent. You can see on your serial console the payload to send if you call the `setDebug(bool debug)` method and pass a true value to it.

- @variable_label, [Required]. The label of the variable where the dot will be stored.
- @value, [Required]. The value of the dot.
- @context, [Optional]. The dot's context.
- @dotTimestampSeconds, [Optional]. The dot's timestamp in seconds.
- @dotTimestampMillis, [Optional]. The dot's timestamp number of milliseconds. If the timestamp's milliseconds values is not set, the seconds will be multplied by 1000.

`void addContext(char *keyLabel, char *keyValue)`

Adds to local memory a new key-value context key. The method inputs must be char pointers. The method allows to store up to 10 key-value pairs.

- @keyLabel, [Required]. The key context label to store values.
- @keyValue, [Required]. The key pair value.

`void getContext(char *context)`

Builds the context according to the chosen protocol and stores it in the context char pointer.

- @context, [Required]. A char pointer where the context will be stored.

`bool connected()`

Returns true if the ESP32 is connected to the broker

`bool connect(const char* clientName, const char* username, const char* password)`

Connects to the broker using a custom client name, username and password

- @clientName, [Optional] [default] = device unique MAC address. The MQTT unique client id to be identified by the broker.
- @username, [optional] [default] = Ubidots account token used in the constructor. The username to be identified by the broker
- @password, [optional] [default] = Ubidots account token used in the constructor. The password to be identified by the broker

`void connectToWifi(const char* ssid, const char* pass)`

Connects to an available WiFi network using WAP2.

- @ssid [Mandatory] SSID of the network to connect to
- @pass [Mandatory] WiFi network password

`void disconnect()`

Disconnects gracefully from the broker, closing the socket

`void loop()`

Maintains the socket connection and sends periodically the keep alive command

`bool publish(const char* deviceLabel)`
Sends all the data added using the add() method. Returns true if the data was sent.

- @deviceLabel, [Optional], [Default] = Device's MAC Address. The device label to send data. If not set, the device's MAC address will be used.

`void reconnect()`
A custom routine to reconnect to the broker. This routine is blocking, if the ESP32 cannot connect to the broker will be stucked in this routine until it can be open a socket to the broker

`void setDebug(bool debug)`

Makes available debug messages through the serial port.

- @debug, [Required]. Boolean type to turn off/on debug messages.

`void setCallback(void (*callback)(char*, uint8_t*, unsigned int))`

Sets the callback to be used to process the data incoming from the subscribed topics

- @callback [Mandatory] Pointer to the callback function that will process the incoming data

`void setup()`

Method to configure all the necessary settings in the Ubidots instance to connect to the broker. Should be called in the main Arduino setup() function (see examples).

`bool subscribe(const char* topic)`

Retrieves data from Ubidots by subscribing to a custom topic. All data will be processed in the callback function set by the setCallback() method.
- @topic [Mandatory] topic to be subscribed

`bool subscribeLastValue(const char* deviceLabel, const char* variableLabel)`

Retrieves data from an Ubidots variable by subscribing to a topic that will be updated if any new value is stored. All data will be processed in the callback function set by the setCallback() method.
- @deviceLabel [Mandatory] Device label that stores the variable to retrieve values from
- @variableLabel [Mandatory] Variable label to retrieve values from

# Examples

Refer to the [examples](/examples) folder
