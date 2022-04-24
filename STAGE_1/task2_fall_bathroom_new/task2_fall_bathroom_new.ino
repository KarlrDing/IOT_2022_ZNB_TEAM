//libraries
#include <Arduino_MKRENV.h>
//MQTT
#include <WiFiNINA.h>
#include <ArduinoMqttClient.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include "wifi_secrets.h" 
// Add WiFi connection information
char ssid[] = SECRET_SSID_2;          //  your network SSID (name)
char pass[] = SECRET_PASS_2;          // your network password
int WiFistatus = WL_IDLE_STATUS;      // the Wifi radio's status

//======================================================================//
#define MQTT_HOST "zpbhjh.messaging.internetofthings.ibmcloud.com"
#define MQTT_PORT 1883
#define MQTT_DEVICEID "d:zpbhjh:mkr1010:3c610515f1ec"
#define MQTT_USER "use-token-auth"
#define MQTT_TOKEN "mkr1010_3c610515f1ec"
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
static char msg[1000];

//functions declared
//mqtt init
void mqtt_init();
//mqtt send
void mqtt_send(int,float,const char*,float,const char*,float,const char*);
//mqtt get data from could

//mqtt components
void callback(char*,byte*,unsigned int);
void printData();
//======================================================================//

//define the sensor pins
#define buzzer_pin 0
#define sound_pin A0
#define light_pin A1
#define rotate_pin A4
#define mag_pin 3
//define the wait time
#define wait_time 2//double the target
//define the alarm trigger conditions
#define alarm_sound 100
#define alarm_humidity 0
#define alarm_temp 0 
#define door_light_threshold 200
#define door_rotate_threshold 150
//rotate use
#define ADC_REF 5
#define GROVE_VCC 5
#define FULL_ANGLE 300

//define state varieties
//control states
int sound_value;
int sound_max;
int light_value;
int rotate_value;
int mag_value;
bool door_state;
int buzzer_state = 0;
int trans_rate = 10;
int message = 11;
int switch_state = 0;
//global clock counter
int clock_counter = 0;
int clock_sec = 0;
int clock_min = 0;
int clock_hou = 0;
//humidity
float humidity_value = 0;
//temperature
float temperature_value = 0;
//alarm counter
bool alarm_counter_start = false;
int alarm_counter = 0;

//functions declaration
//ENV initialize
void env_init();
//global clock prepare
int clock_p(int);
//global clock sec
int clock_s(int,int);
//global clock min
int clock_m(int,int);
//global clock hour
int clock_h(int,int);
//door open/close detection
bool door_state_detection_light(bool,int);
bool door_state_detection_rotate(bool,int);
bool door_state_detection_mag(bool,bool);
//sound max values
int sound_value_max(int,int,bool);
//alarm
bool fall_alarm_detect(int,float,float,bool);
int fall_alarm_buzzer(bool,int,int,int);
bool door_close_detect(bool);
int fall_alarm_monitor(int,float,float,bool,int,int,int*);

//initialize
void setup() {
  //MQTT INIT
  mqtt_init();
  //INOUT MODE
  pinMode(sound_pin,INPUT);
  pinMode(light_pin,INPUT);
  pinMode(rotate_pin,INPUT);
  pinMode(mag_pin,INPUT);
  //OUTPUT MODE
  pinMode(buzzer_pin,OUTPUT);
  //INITIAL VALUES
  
  //INITIAL SHIELD
  env_init();
  //test
  Serial.begin(9600);
}

//main loop
void loop(){
  //global delay
  clock_counter = clock_p(clock_counter);
  clock_sec = clock_s(clock_counter,clock_sec);
  clock_min = clock_m(clock_sec,clock_min);
  clock_hou = clock_h(clock_min,clock_hou);
  //test
  //Serial.println("clock");
  //Serial.println(clock_sec);
  //Serial.println(clock_min);
  //get values from sensors
  sound_value       = analogRead(sound_pin);
  light_value       = analogRead(light_pin);
  rotate_value      = analogRead(rotate_pin);
  mag_value         = digitalRead(mag_pin);
  humidity_value    = ENV.readHumidity();
  temperature_value = ENV.readTemperature();
  //test
  //Serial.println("indicators");
  //Serial.println(sound_max);
  //Serial.println(humidity_value);
  //Serial.println(door_state);
  //Serial.println("flag");
  //Serial.println(temperature_value);
  //Serial.println(alarm_counter_start);
  //Serial.println(alarm_counter);
  //get the sound max value
  sound_max = sound_value_max(sound_max,sound_value,door_state);
  //detect door state (open or not)
  //door_state = door_state_detection_light(door_state,light_value);
  //door_state = door_state_detection_rotate(door_state,rotate_value);
  door_state = door_state_detection_mag(door_state,mag_value);
  //alarm
  //alarm_counter_start = fall_alarm_detect(sound_value,humidity_value,temperature_value,door_state);
  //alarm_counter = fall_alarm_buzzer(alarm_counter_start,alarm_counter,clock_min,clock_sec);
  alarm_counter_start = door_close_detect(door_state);
  alarm_counter = fall_alarm_monitor(sound_max,humidity_value,temperature_value,alarm_counter_start,alarm_counter,clock_sec,&buzzer_state);
  //MQTT send data
  mqtt_send(clock_sec ,door_state,"door_state",sound_value,"sound_value",temperature_value,"temperature", humidity_value, "humidity", buzzer_state,"buzzer");
  //Serial.println(sound_value);
}






//======================================================================//
//=========================function definition==========================//
//======================================================================//
void env_init(){
  if (!ENV.begin())
  {//check correct initialisation of shield
    Serial.println("Failed to initialize MKR ENV shield!");
    while(1);//infinite loop, avoids loop to be executed
  }
}

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

    // wait 3 seconds for connection:
    delay(3000);
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

void mqtt_send(int sec, float send_data, const char* send_name,float send_data1, const char* send_name1,float send_data2, const char* send_name2, float send_data3, const char* send_name3, float send_data4, const char* send_name4){
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
    if (sec % trans_rate == 0 && switch_state == 1)
    {
      status["door_state"] = send_data;
      status["sound_value"] = send_data1;
      status["temperature"] = send_data2;
      status["humidity_value"] = send_data3;
      status["buzzer_state"] = send_data4;
      serializeJson(jsonDoc, msg, 1000);
      Serial.println(msg);
      if (!mqtt.publish(MQTT_TOPIC, msg)) {
        Serial.println("MQTT Publish failed");
      }
    }else{;}
  }
  
}

void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] : ");

  payload[length] = 0; // ensure valid content is zero terminated so can treat as c-string
  Serial.println((char *)payload);
  String message1 = (char *)payload;
  message = message1.toInt();
  if (message == 31){
    sound_max = 0;
    message = 0;
    }
  else if (message <31 && message >0 ){
    trans_rate = message;
    }
  if (message == 32){
    switch_state = 1;
    }      
  else if(message == 33){
    switch_state = 0;
  }
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
  delay(500);
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

bool door_state_detection_light(bool door, int light){
  if (light >= door_light_threshold)
  {
    return true;
  }
  else
  {
    return false;//test using true
  }
}

bool door_state_detection_rotate(bool door, int rotate){
  float voltage = (float)rotate*ADC_REF/1023;
  float degrees = (voltage*FULL_ANGLE)/GROVE_VCC;

  if (degrees >= door_rotate_threshold)
  {
    return false;
  }
  else
  {
    return true;
  }
}

bool door_state_detection_mag(bool door, bool mag){
  if (mag == HIGH)
  {
    return true;
  }
  else
  {
    return false;
  }
}

bool fall_alarm_detect(int sound, float humidity, float temperature,bool door){
  if (sound >= alarm_sound && humidity >= alarm_humidity && temperature >= alarm_temp && door == false)//change threshold
  {
    return true;
  }
}

int fall_alarm_buzzer(bool flag, int counter, int minu, int sec){
  if (flag == false)
  {
    counter = 0;
  }
  else
  {
    if (sec == 0 && counter < wait_time)
    {
      counter = counter + 1;
    }
    else if (sec == 0 && counter == wait_time)
    {
      tone(buzzer_pin, 1000);
      delay(500);
      noTone(buzzer_pin);
      delay(500); 
      counter == 0;
    }
  }
  return counter;
}

bool door_close_detect(bool door){
  if (door == true)//change threshold
  {
    return true;
  }
  else
  {
    return false;
  }
}

int fall_alarm_monitor(int sound, float humidity, float temperature, bool flag, int counter, int sec, int* buzzer){
  if (flag == false)
  {
    counter = 0;
    *buzzer = 0;
  }
  else
  {
    if (sec == 0 && counter < wait_time)
    {
      counter = counter + 1;
    }
    else if (counter == wait_time)
    //else if (sec == 1 && counter == wait_time)
    {
      if (sound >= alarm_sound && humidity >= alarm_humidity && temperature >= alarm_temp)
      {
        *buzzer = 1;
      }else{
        *buzzer = 0;
        counter = 0;
      }
      if (*buzzer == 1)
      {
        Serial.println("Buzzing");
        tone(buzzer_pin, 1000);
        delay(500);
        noTone(buzzer_pin);
        delay(500);
      }
    }
  }
  return counter;
}

int sound_value_max(int max,int sound,bool door){
  if (sound >= max)
  {
    max = sound;
  }
  if (door == false)
  {
    max = 0;
  }
  return max;
}
