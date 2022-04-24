#include <WiFiNINA.h>
#include <ArduinoMqttClient.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>

// --------------------------------------------------------------------------------------------
//        UPDATE CONFIGURATION TO MATCH YOUR ENVIRONMENT
// --------------------------------------------------------------------------------------------

// Watson IoT connection details
#define MQTT_HOST "95kg5h.messaging.internetofthings.ibmcloud.com"
#define MQTT_PORT 1883
#define MQTT_DEVICEID "d:95kg5h:mkr1010:7c9ebd3b5680"
#define MQTT_USER "use-token-auth"
#define MQTT_TOKEN "mydevice"
#define MQTT_TOPIC "iot-2/evt/status/fmt/json"
#define MQTT_TOPIC_DISPLAY "iot-2/cmd/display/fmt/json"

// Add WiFi connection information
char ssid[] = "ZhuHX";     //  your network SSID (name)
char pass[] = "ZhuHaoxiang";  // your network password

int WiFistatus = WL_IDLE_STATUS;     // the Wifi radio's status

int message = 255;
int buzzer = 0;
int buzzer_pin = 0;
bool buzzer_state = false;
int buzzer_value = 255;
int light_in = A0;
int fall_down = 0;
int lost_state = 0;
int medicine_state = 0;
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

void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] : ");

  payload[length] = 0; // ensure valid content is zero terminated so can treat as c-string
  Serial.println((char *)payload);
  String message0 = (char*)payload;
  message = message0.toInt();
}

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  pinMode(buzzer_pin, OUTPUT);
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

void loop() {
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

      // make unique noise when mqtt connect failed
      
      analogWrite(buzzer_pin,50);
      delay(100);
      analogWrite(buzzer_pin,100);
      delay(100);
      analogWrite(buzzer_pin,150);
      delay(100);
      analogWrite(buzzer_pin,200);
      delay(100);
      analogWrite(buzzer_pin,250);
      delay(100);
      analogWrite(buzzer_pin,0);
      delay(4500);
    }
  }

  // buzzer or stop when received different messages.
  
  if (message == 299) {
    analogWrite(buzzer_pin,buzzer_value);
    delay(500);
    analogWrite(buzzer_pin,0);
    delay(100); 
    Serial.println("bathroom buzzering");
  }
  else if (message == 300){
    analogWrite(buzzer_pin, 0);
  } 
  else if (message<256 && message > 0){
    // change the buzzer sound remotely
    buzzer_value = message;
    }
  if (message == 399) {
    analogWrite(buzzer_pin,buzzer_value);
    delay(500);
    analogWrite(buzzer_pin,0);
    delay(100); 
    Serial.println("stairs buzzering");
  }
  else if (message == 400){
    analogWrite(buzzer_pin, 0);
  } 
  if (message == 499) {
    lost_state = 1;
  }  
  else if (message == 500){
    analogWrite(buzzer_pin, 0);
    lost_state = 0;
  } 
  if (lost_state == 1){
    analogWrite(buzzer_pin,buzzer_value);
    delay(500);
    analogWrite(buzzer_pin,0);
    delay(100); 
    Serial.println("stair buzzering");    
    }
  if (message == 599) {
    medicine_state = 1;
  }
  else if (message == 600){
    analogWrite(buzzer_pin, 0);
    medicine_state = 0;
  } 
  if (medicine_state == 1){
    analogWrite(buzzer_pin,buzzer_value);
    delay(500);
    analogWrite(buzzer_pin,0);
    delay(100); 
    Serial.println("medicine buzzering");   
    }
//   Check if any reads failed and exit early (to try again).
//  if (isnan(temperature)) {
//    Serial.println("Failed to read data!");
//  } else {
//
//    // Send data to Watson IoT Platform
//    status["buzzer_state"] = buzzer;
//    serializeJson(jsonDoc, msg, 50);
//    Serial.println(msg);
//    if (!mqtt.publish(MQTT_TOPIC, msg)) {
//      Serial.println("MQTT Publish failed");
//    }
//  }

  // Pause - but keep polling MQTT for incoming messages
  for (int i = 0; i < 4; i++) {
    mqtt.loop();
    delay(50);
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
