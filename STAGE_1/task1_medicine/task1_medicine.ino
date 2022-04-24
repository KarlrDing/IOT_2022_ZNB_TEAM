//libraries
#include <Arduino_MKRENV.h>
//MQTT
#include <WiFiNINA.h>
#include <ArduinoMqttClient.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include "wifi_secrets.h" 
// Add WiFi connection information
char ssid[] = SECRET_SSID_3;          //  your network SSID (name)
char pass[] = SECRET_PASS_3;          // your network password
int WiFistatus = WL_IDLE_STATUS;      // the Wifi radio's status

//======================================================================//
#define MQTT_HOST "95kg5h.messaging.internetofthings.ibmcloud.com"
#define MQTT_PORT 1883
#define MQTT_DEVICEID "d:95kg5h:mkr1010:3c610516400c"
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
void mqtt_send(float,const char*,float,const char*,float,const char*,bool);
//mqtt get data from could

//mqtt components
void callback(char*,byte*,unsigned int);
void printData();
//======================================================================//

//define the sensor pins
#define touch_pin 1
#define button_pin 0
#define buzzer_pin 3
#define red_led_pin 2
//define the wait time
//#define wait_time 40//double the target

//define state varieties
//control states
bool touch_value;
bool button_value;
bool button_state = false;
int bottle_state = 0;
bool time_up = false;
int time_up_counter = 0;
bool state_reset = false;
int wait_time = 40;//double the target global
//buffer
bool buffer_diff = false;
bool buffer_1;
bool buffer_2;
int  buffer_3;
//global clock counter
int clock_counter = 0;
int clock_sec = 0;
int clock_min = 0;
int clock_hou = 0;
//used for faded light
int brightness = 0;    // how bright the LED is
int fadeAmount = 51;    // how many points to fade the LED by

//functions declaration
//red light control
void red_light_on();
void red_light_off();
void red_light_alert();
//global clock prepare
int clock_p(int);
//global clock sec
int clock_s(int,int);
//global clock min
int clock_m(int,int);
//global clock hour
int clock_h(int,int);
//button state control
bool button_state_control(bool,bool);
//bottle state control
int bottle_state_control(int,bool,bool,bool,bool);
//light control
void light_control(int);
//buzzer control
void buzzer_slow_alert();
//time up control
bool time_up_control(bool,int,int*,int);
//send buffer
bool buffer_control(bool,bool,int,bool,bool*,bool*,int*);

//initialize
void setup() {
  //MQTT INIT
  mqtt_init();
  //INOUT MODE
  pinMode(touch_pin,INPUT);
  pinMode(button_pin,INPUT);
  //OUTPUT MODE
  pinMode(red_led_pin,OUTPUT);
  pinMode(buzzer_pin,OUTPUT);
  //INITIAL VALUES
  
  //TEST
  Serial.begin(9600);
}

//main loop
void loop(){
  //global delay
  clock_counter = clock_p(clock_counter);
  clock_sec = clock_s(clock_counter,clock_sec);
  clock_min = clock_m(clock_sec,clock_min);
  clock_hou = clock_h(clock_min,clock_hou);
  //get values from sensors
  touch_value = digitalRead(touch_pin);
  button_value = digitalRead(button_pin);

  //control the button state
  button_state = button_state_control(button_state,button_value);

  //time up control
  time_up = time_up_control(time_up,clock_sec,&time_up_counter,bottle_state);

  //control the bottle state
  bottle_state = bottle_state_control(bottle_state,button_state,time_up,touch_value,&state_reset);

  //control the light mode
  light_control(bottle_state);

  //MQTT send data
  //buffer -> diff
  buffer_diff = buffer_control(touch_value,button_value,bottle_state,buffer_diff,&buffer_1,&buffer_2,&buffer_3);
  mqtt_send(touch_value,"touch_value",button_value,"button_value",bottle_state,"bottle_state",buffer_diff);
  //Serial.println(bottle_state);
  //Serial.println(time_up_counter);
  //Serial.println(time_up);
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

void mqtt_send(float send_data, const char* send_name,float send_data1, const char* send_name1,float send_data2, const char* send_name2,bool diff){
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
  if (isnan(send_data)) {
    Serial.println("Failed to read data!");
  } else {

    // Send data to Watson IoT Platform
    if (diff == true){
      status[send_name]   = send_data;
      status[send_name1]  = send_data1;
      status[send_name2]  = send_data2;
      serializeJson(jsonDoc, msg, 100);
      Serial.println(msg);
      if (!mqtt.publish(MQTT_TOPIC, msg)) {
        Serial.println("MQTT Publish failed");
      }
    } 
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] : ");

  payload[length] = 0; // ensure valid content is zero terminated so can treat as c-string
  Serial.println((char *)payload);
  //JSON ANALYSIS
  StaticJsonDocument<256> jsonBuffer;
  deserializeJson(jsonBuffer,payload);
  JsonObject root = jsonBuffer.as<JsonObject>();
  int re_state  = root["state"];
  int re_time   = root["wait_time"];
  bottle_state  = re_state;
  wait_time     = re_time;

  //String received_message = (char *)payload;//received message
  //String state      = getValue(received_message, '_', 0);
  //String wait_time  = getValue(received_message, '_', 1);
  //bottle_state = state.toInt();
  //wait_time = wait_time.toInt();
  Serial.println("state");
  Serial.println(bottle_state);
  Serial.println("wait time");
  Serial.println(wait_time);
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

int clock_p(int counter){
  delay(400);
  if (counter == 1)
  {
    counter = 0;
  }
  else
  {
    counter = counter + 1;
  }
  return counter;
}

int clock_s(int counter, int sec){
  if (counter == 1)
  {
    if (sec == 59)
    {
      sec = 0;
    }
    else
    {
      sec = sec + 1;
    }
  }
  return sec;
}

int clock_m(int counter, int minu){
  if (counter == 0)
  {
    if (minu == 118)
    {
      minu = 0;
    }
    else
    {
      minu = minu + 1;
    }
  }
  return minu;
}

int clock_h(int counter, int hour){
  if (counter == 0)
  {
    if (hour == 46)
    {
      hour = 0;
    }
    else
    {
      hour = hour + 1;
    }
  }
  return hour;
}

void red_light_on(){
  //digitalWrite(red_led_pin,HIGH);
  analogWrite(red_led_pin, brightness);
  brightness = brightness + fadeAmount;
  if (brightness <= 0 || brightness >= 255) {
    fadeAmount = -fadeAmount;
  }
  //delay(30);
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

bool button_state_control(bool state,bool value){
  state = state or value;
  return state;
}

int bottle_state_control(int state,bool button, bool time_up, bool value,bool* state_reset){
  switch(state){
    case 0:
    {
      *state_reset = false;
      if (value == false)
      {
        return 5;//forgot to put the medicine bottle back to its position
      }
      else{
        return 1;//normal
      }
      break;
    }
    //END of CASE 0
    case 1:
    {
      if (time_up == true)
      {
        return 7;//forgot to take medicine
      }
      else if (value == false)
      {
        return 2;//the bottle has been taken up at least once
      }
      else
      {
        return state;
      }
      break;
    }
    //END of CASE 1
    case 2:
    {
      if (time_up == true)
      {
        return 7;//forgot to take medicine
      }
      else if (value == true)
      {
        return 3;//the bottle has been returned after taken
      }
      else
      {
        return state;
      }
      break;
    }
    //END of CASE 2
    case 3:
    {
      if (time_up == true)
      {
        return 7;//forgot to take medicine
      }
      else if (button == true)
      {
        return 6;//medicine has been taken
      }
      else
      {
        return state;
      }
      break;
    }
    //END of CASE 3
    case 5:
    {
      if (value == true)
      {
        return 3;//medicine bottle has been returned
      }
      else
      {
        return state;
      }
      break;
    }
    //END of CASE 5
    case 6:
    {
      if(*state_reset == true)
      {
        return 0;
      }
      else
      {
        return state;
      }
      break;
    }
    //END of CASE 5
    case 7:
    {
      if (button == true)
      {
        return 6;//medicine has been taken
      }
      else
      {
        buzzer_slow_alert();
        return state;
      }
      break;
    }
    //END of CASE 7
  }
  return state;
}

void light_control(int bottle){
  switch(bottle){
    case 5:
    red_light_alert();
    break;
    case 6:
    red_light_off();
    break;
    default:
    red_light_on();//dynamic light is better to be a reminder
  }
}

bool time_up_control(bool time_up, int sec, int* counter,int state){
  if (!(state == 6))
  {
    if (sec == 0 && *counter < wait_time)
    {
      *counter = *counter + 1;
      return false;
    }
    else if( sec == 1 && *counter == wait_time)
    {
      return true;
    }
    else
    {
      return false;
    }
  }
  else
  {
    *counter = 0;
    return false;
  }
}
bool buffer_control(bool touch, bool button, int state, bool diff, bool* bt,bool* bb, int* bs)
{
  if (touch != *bt)
  {
    *bt = touch;
    return true;
  }
  else if(button != *bb)
  {
    *bb = button;
    return true;
  }
  else if(state != *bs)
  {
    *bs = state;
    return true;
  }
  else
  {
    return false;
  }
}

//get value
String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
//get value
