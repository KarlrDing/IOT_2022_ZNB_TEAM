#include <WiFiNINA.h>
#include <ArduinoMqttClient.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>


// --------------------------------------------------------------------------------------------
//        UPDATE CONFIGURATION TO MATCH YOUR ENVIRONMENT
// --------------------------------------------------------------------------------------------

// Watson IoT connection details
#define MQTT_HOST "x8mvmc.messaging.internetofthings.ibmcloud.com"
#define MQTT_PORT 1883
#define MQTT_DEVICEID "d:x8mvmc:mkr1010:7c9ebd3b5a34"
#define MQTT_USER "use-token-auth"
#define MQTT_TOKEN "mrk1010_7c9ebd3b5a34"
#define MQTT_TOPIC "iot-2/evt/status/fmt/json"
#define MQTT_TOPIC_DISPLAY "iot-2/cmd/display/fmt/json"

// Add WiFi connection information
char ssid[] = "JNN";     //  your network SSID (name)
char pass[] = "jianannan";  // your network password

int WiFistatus = WL_IDLE_STATUS;     // the Wifi radio's status

// --------------------------------------------------------------------------------------------
//        SHOULD NOT NEED TO CHANGE ANYTHING BELOW THIS LINE
// --------------------------------------------------------------------------------------------

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
void mqtt_send(int,const char*);
//mqtt components
void callback(char*,byte*,unsigned int);
void printData();
//======================================================================//

//define port
#define button_pin 0
#define wait_time 1
//define variable
int button_value;
//global clock counter
int clock_counter = 0;
int clock_sec = 0;
int clock_min = 0;
//alarm counter
int alarm_counter = 0;
//counter_buffer
int sec_buffer = 0;
//alarm message to cloud
int alarm = 0;

//functions declaration
//global clock prepare
int clock_p(int);
//global clock sec
int clock_s(int,int);
//global clock min
int clock_m(int,int);
//alarm
int fall_alarm_monitor(int, int, int, int, int*);

//initialize
void setup() {
    //MQTT INIT
    mqtt_init();
    //setup sensor
    pinMode(button_pin, INPUT);
    Serial.begin(9600);
}

//main loop
void loop(){
  //global delay
  clock_counter = clock_p(clock_counter);
  clock_sec = clock_s(clock_counter,clock_sec);
  clock_min = clock_m(clock_sec,clock_min);
  //test
  Serial.println("clock");
  Serial.println(clock_sec);
  Serial.println(clock_min);
  //get values from sensors
  button_value = digitalRead(button_pin);
  delay(500);
  //Serial.println(button_value);

  alarm_counter = fall_alarm_monitor(button_value,alarm_counter,clock_sec,clock_min,&sec_buffer);
  //MQTT send data
  mqtt_send(alarm,"alarm");
}


//======================================================================//
//====================MQTT function definition==========================//
//======================================================================//
void mqtt_init() {
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
    delay(1000);
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

void mqtt_send(int send_data, const char* send_name) {
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

    // Send button state to Watson IoT Platform
    status["alarm"] = send_data;
    serializeJson(jsonDoc, msg, 50);
    Serial.println(msg);
    if (!mqtt.publish(MQTT_TOPIC, msg)) {
      Serial.println("MQTT Publish failed");
    }
  }

  // Pause - but keep polling MQTT for incoming messages
  for (int i = 0; i < 4; i++) {
    mqtt.loop();
    delay(100);
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


//======================================================================//
//=========================function definition==========================//
//======================================================================//

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


int fall_alarm_monitor(int button, int counter, int sec, int minu,int* sec_buffer){
    if (button == 0){
      counter = 0;
      alarm = 0;
      *sec_buffer = sec;
      }
      else {
        if (sec == *sec_buffer && counter < wait_time){
          counter = counter + 1;
        }
        else if (sec == *sec_buffer && counter == wait_time){
            alarm = 1;
            Serial.println("alarm"); 
            Serial.println(alarm);
          }
       }
  return counter;
}
