//libraries
#include <Arduino_MKRENV.h>
//MQTT
#include <WiFiNINA.h>
#include <ArduinoMqttClient.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include "wifi_secrets.h" 
// Add WiFi connection information
char ssid[] = SECRET_SSID_0;          //  your network SSID (name)
char pass[] = SECRET_PASS_0;          // your network password
int WiFistatus = WL_IDLE_STATUS;      // the Wifi radio's status

//======================================================================//
#define MQTT_HOST "bowcro.messaging.internetofthings.ibmcloud.com"
#define MQTT_PORT 1883
#define MQTT_DEVICEID "d:bowcro:mkr1010:3c610516400c"
#define MQTT_USER "use-token-auth"
#define MQTT_TOKEN "karldevice"
#define MQTT_TOPIC "iot-2/evt/status/fmt/json"
#define MQTT_TOPIC_DISPLAY "iot-2/cmd/display/fmt/json"
// MQTT objects
void callback(char* topic, byte* payload, unsigned int length);
WiFiClient wifiClient;
PubSubClient mqtt(MQTT_HOST, MQTT_PORT, callback, wifiClient);

// variables to hold data
StaticJsonDocument<100> jsonDoc;
JsonObject payload = jsonDoc.to<JsonObject>();
JsonObject status = payload.createNestedObject("d");
static char msg[100];

//functions declared
//mqtt init
void mqtt_init();
//mqtt send
void mqtt_send();
//mqtt get data from could

//mqtt components
void callback(char*,byte*,unsigned int);
void printData();
//======================================================================//

//define the sensor pins
#define buzzer_pin 1
#define red_led_pin 0
//define the wait time

//define state varieties
//control states
int warning_state = 0;
int warning_buffer = 1;
//used for faded light
int brightness = 0;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by

//functions declaration
//red light control
void red_light_on();
void red_light_off();
void red_light_alert();
//buzzer control
void buzzer_slow_alert();
//control led and buzzer
void warn_control(int);

//initialize
void setup() {
  //MQTT INIT
  mqtt_init();
  //INOUT MODE

  //OUTPUT MODE
  pinMode(red_led_pin,OUTPUT);
  pinMode(buzzer_pin,OUTPUT);
  //INITIAL VALUES
  
  //TEST
  Serial.begin(9600);
}

//main loop
void loop(){
  mqtt_send();
  //control led and buzzer
  warn_control(warning_state);
  
  if (warning_state != warning_buffer)
  {
    Serial.println(warning_state);
    warning_buffer = warning_state;
  }
}






//======================================================================//
//=========================function definition==========================//
//======================================================================//

//========================MQTT func definition==========================//
void mqtt_init(){
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // attempt to connect to Wifi network:

  while (WiFistatus != WL_CONNECTED) {
    Serial.print("Attempting to connect to network: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    WiFistatus = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }

  // you're connected now, so print out the data:
  Serial.println("You're connected to the network");
  
  Serial.println("----------------------------------------");
  printData();
  Serial.println("----------------------------------------");

  // Connect to MQTT - IBM Watson IoT Platform
  if (mqtt.connect(MQTT_DEVICEID, MQTT_USER, MQTT_TOKEN)) {
    Serial.println("MQTT Connected");
    mqtt.subscribe(MQTT_TOPIC_DISPLAY);

  } else {
    Serial.println("MQTT Failed to connect!");
  }
}

void mqtt_send(){
  mqtt.loop();
  while (!mqtt.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqtt.connect(MQTT_DEVICEID, MQTT_USER, MQTT_TOKEN)) {
      Serial.println("MQTT Connected");
      mqtt.subscribe(MQTT_TOPIC_DISPLAY);
      mqtt.loop();
    } else {
      Serial.println("MQTT Failed to connect!");
      delay(5000);
    }
  }
  // Check if any reads failed and exit early (to try again).
}

void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] : ");

  payload[length] = 0; // ensure valid content is zero terminated so can treat as c-string
  Serial.println((char *)payload);
  String received_message = (char *)payload;//received message
  warning_state = received_message.toInt();
}

void printData() {
  Serial.println("Board Information:");
  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  Serial.println();
  Serial.println("Network Information:");
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);

  byte encryption = WiFi.encryptionType();
  Serial.print("Encryption Type:");
  Serial.println(encryption, HEX);
  Serial.println();
}
//========================MQTT func definition==========================//

void red_light_on(){
  //digitalWrite(red_led_pin,HIGH);
  analogWrite(red_led_pin, brightness);
  brightness = brightness + fadeAmount;
  if (brightness <= 0 || brightness >= 255) {
    fadeAmount = -fadeAmount;
  }
  delay(30);
}

void red_light_off(){
  //digitalWrite(red_led_pin,LOW);
  analogWrite(red_led_pin, 0);
}

void red_light_alert(){
  //digitalWrite(red_led_pin,HIGH);
  analogWrite(red_led_pin, 255);
  delay(500);
  //digitalWrite(red_led_pin,LOW);
  analogWrite(red_led_pin, 0);
  delay(500);
}

void buzzer_slow_alert(){
  analogWrite(buzzer_pin,80);
  delay(500);
  analogWrite(buzzer_pin,0);
  delay(500);
}

void warn_control(int state)
{
  if (state == 1)
  {
    red_light_on();
    buzzer_slow_alert();
  }
  else
  {
    red_light_off();
  }
}
