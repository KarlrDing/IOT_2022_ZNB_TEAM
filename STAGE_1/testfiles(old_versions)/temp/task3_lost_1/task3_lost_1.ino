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
#define MQTT_DEVICEID "d:95kg5h:mkr1010:3c6105166844"
#define MQTT_USER "use-token-auth"
#define MQTT_TOKEN "mkr1010_3c6105166844"
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
static char msg[50];

//functions declared
//mqtt init
void mqtt_init();
//mqtt send
void mqtt_send(int,int*);
//mqtt get data from could

//mqtt components
void callback(char*,byte*,unsigned int);
void printData();
//======================================================================//

//define the sensor pins
#define button1_pin 1
#define button2_pin 2
#define light_pin 0
#define mag_pin 3
#define hand_pin 5
//define the wait time
#define wait_time 2//double the target

//door code
bool door_code[4] = {true,true,false,true};

//define state varieties
//control states
int send_data_buffer = 0;
int button1_value;
int button2_value;
int mag_value;
bool door_state;
bool hand_value;
bool inout_value;//true is out false is in
//patient detection control
int patient_detection_state = 0;
int pdc_counter = 0;
bool pdc_flag = false;
//global clock counter
int clock_counter = 0;
int clock_sec = 0;
int clock_min = 0;
int clock_hou = 0;

//functions declaration
//global clock prepare
int clock_p(int);
//global clock sec
int clock_s(int,int);
//global clock min
int clock_m(int,int);
//global clock hour
int clock_h(int,int);
//door detection
bool door_state_detection_mag(bool,bool);
//patient detection control
int patient_detection_control(int,bool,bool,int,bool,int,int*,bool*);
//inout control
bool inout_control(int,bool);

//initialize
void setup() {
  //MQTT INIT
  mqtt_init();
  //INOUT MODE
  pinMode(button1_pin,INPUT);
  pinMode(button2_pin,INPUT);
  pinMode(mag_pin,INPUT);
  pinMode(hand_pin,INPUT);
  //OUTPUT MODE
  pinMode(light_pin,OUTPUT);
  //INITIAL VALUES
  patient_detection_state = 0;
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
  button1_value     = digitalRead(button1_pin);
  button2_value     = digitalRead(button2_pin);
  mag_value         = digitalRead(mag_pin);
  hand_value        = digitalRead(hand_pin);
  //test
//  Serial.println("indicators");
//  Serial.println(button1_value);
//  Serial.println(button2_value);
//  Serial.println(mag_value);
  //door state detection
  door_state = door_state_detection_mag(door_state,mag_value);
  //inout control
  inout_value = inout_control(mag_value,hand_value);
  //patient detection control
  patient_detection_state = patient_detection_control(patient_detection_state,button1_value,button2_value,mag_value,inout_value,clock_sec,&pdc_counter,&pdc_flag);
  //test
//  Serial.println("state");
//  Serial.println(inout_value);
//  Serial.println(patient_detection_state);
//  Serial.println(pdc_counter);
  
  //MQTT send data
  mqtt_send(patient_detection_state,&send_data_buffer);
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

void mqtt_send(int send_data,int* send_data_buffer){
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
    if (*send_data_buffer == send_data)
    {
      ;//send nothing
    }
    else
    {
      *send_data_buffer = send_data;
      status["door_state"] = send_data;
      serializeJson(jsonDoc, msg, 50);
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

bool inout_control(int mag, bool hand){
  if (mag == false && hand == true)
  {
    return true;
  }
  else
  {
    return false;
  }
}

int patient_detection_control(int state, bool b1, bool b2, int mag,bool inout,int s,int* counter,bool* flag){
  switch(state){
    case 0:
    {
      *counter = 0;
      *flag = false;
      analogWrite(light_pin, 0); 
      if (b1 == true || b2 == true)
      {
        //*counter = 0;
        *flag = true;
        return 1;
      }
      else if(inout == true)//true is out
      {
        return 2;
      }
      else if(mag == LOW)
      {
        //*counter = 0;
        return 5;
      }
      return state;
      break;
    }
    //END of CASE 0
    case 5:
    {
      if (mag == HIGH)
      {
        return 1;
        break;
      }
      else
      {
        return state;
        break;
      }
    }
    //END of CASE 5
    case 1://put in code
    {
      analogWrite(light_pin, 255);
      if (s == 0 && *counter < wait_time)
      {
        *counter = *counter + 1;
        return state;
        break;
      }
      else if(s == 0 && *counter == wait_time)
      {
        return 3;
        break;
      }
      else
      {
        if (b1 == false && b2 == false)
        {
          analogWrite(light_pin, 200);
          return 6;
          break;
        }
        else
        {
          return state;
          break;
        }
      }
    }
    //END of CASE 1
    case 6:
    {
      if (s == 0 && *counter < wait_time)
      {
        *counter = *counter + 1;
        return state;
        break;
      }
      else if(s == 0 && *counter == wait_time)
      {
        return 3;
        break;
      }
      else
      {
        if (b1 == true)//b1
        {
          analogWrite(light_pin, 170);
          return 7;
          break;
        }
        else
        {
          return state;
          break;
        }
      }
    }
    //END of CASE 6
    case 7:
    {
      if (s == 0 && *counter < wait_time)
      {
        *counter = *counter + 1;
        return state;
        break;
      }
      else if(s == 0 && *counter == wait_time)
      {
        return 3;
        break;
      }
      else
      {
        if (b1 == false && b2 == false)
        {
          analogWrite(light_pin, 140);
          return 8;
          break;
        }
        else
        {
          return state;
          break;
        }
      }
    }
    //END of CASE 7
    case 8:
    {
      if (s == 0 && *counter < wait_time)
      {
        *counter = *counter + 1;
        return state;
        break;
      }
      else if(s == 0 && *counter == wait_time)
      {
        return 3;
        break;
      }
      else
      {
        if (b2 == true)//b2
        {
          analogWrite(light_pin, 110);
          return 9;
          break;
        }
        else
        {
          return state;
          break;
        }
      }
    }
    //END of CASE 8
    case 9:
    {
      if (s == 0 && *counter < wait_time)
      {
        *counter = *counter + 1;
        return state;
        break;
      }
      else if(s == 0 && *counter == wait_time)
      {
        return 3;
        break;
      }
      else
      {
        if (b1 == false && b2 == false)
        {
          analogWrite(light_pin, 80);
          return 10;
          break;
        }
        else
        {
          return state;
          break;
        }
      }
    }
    //END of CASE 9
    case 10:
    {
      if (s == 0 && *counter < wait_time)
      {
        *counter = *counter + 1;
        return state;
        break;
      }
      else if(s == 0 && *counter == wait_time)
      {
        return 3;
        break;
      }
      else
      {
        if (b2 == true)//b2
        {
          analogWrite(light_pin, 50);
          return 11;
          break;
        }
        else
        {
          return state;
          break;
        }
      }
    }
    //END of CASE 10
    case 11:
    {
      if (s == 0 && *counter < wait_time)
      {
        *counter = *counter + 1;
        return state;
        break;
      }
      else if(s == 0 && *counter == wait_time)
      {
        return 3;
        break;
      }
      else
      {
        if (b1 == false && b2 == false)
        {
          analogWrite(light_pin, 20);
          return 12;
          break;
        }
        else
        {
          return state;
          break;
        }
      }
    }
    //END of CASE 11
    case 12:
    {
      if (s == 0 && *counter < wait_time)
      {
        *counter = *counter + 1;
        return state;
        break;
      }
      else if(s == 0 && *counter == wait_time)
      {
        return 3;
        break;
      }
      else
      {
        if (b1 == true)//b1
        {
          analogWrite(light_pin, 0);
          if (*flag == false)
          {
            return 0;
            break;
          }
          else
          {
            *counter = 0;
            return 13;
            break;
          }
        }
        else
        {
          return state;
          break;
        }
      }
    }
    //END of CASE 12
    case 13:
    {
      analogWrite(light_pin, 255);
      if (s == 0 && *counter < wait_time)
      {
        *counter = *counter + 1;
        return 13;
        break;
      }
      else if(s == 0 && *counter == wait_time)
      {
        return 0;
        break;
      }
      else if (inout == true)
      {
        return 14;
        break;
      }
      else
      {
        return 13;
        break;
      }
    }
    //END of CASE 13
    case 14:
    {
      if (mag == HIGH)
      {
        return 0;
        break;
      }
      else
      {
        return state;
        break;
      }
    }
    //END of CASE 14
    case 2:
    {
      if (mag == HIGH)
      {
        light_spark();
        return 0;
        break;
      }
      return state;
      break;
    }
    //END of CASE 2
    case 3:
    {
      if (mag == HIGH)
      {
        light_spark();
        return 0;
        break;
      }
      return state;
      break;
    }
    //END of CASE 3
  }
}

void light_spark(){
  analogWrite(light_pin, 255);
  delay(200);
  analogWrite(light_pin, 0);
  delay(200);
  analogWrite(light_pin, 255);
  delay(200);
  analogWrite(light_pin, 0);
  delay(200);
  analogWrite(light_pin, 255);
  delay(200);
  analogWrite(light_pin, 0);
}
