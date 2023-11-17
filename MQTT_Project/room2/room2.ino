/*--------------------------------------------------------------------------------------------------------------------------------
*Date              : 25 September 2023
*Title             : ESP8266 MQTT Temperature and Lighting Control
*Description       : This ESP8266 MQTT example demonstrates several functions:
*                    1. Establishes a connection to an MQTT server (Raspberry Pi broker).
*                    2. Periodically publishes temperature readings from a DHT sensor to the "home/room2/temp" MQTT topic.
*                    3. Subscribes to various MQTT topics and controls LEDs based on received messages.
*                    4. If the first character of the received MQTT topic is '1', it switches on LEDs, and '0' switches them off.
*                    5. Implements a reconnection mechanism to the MQTT server if the connection is lost.
*                    6. Uses the PubSubClient library for MQTT communication.
*--------------------------------------------------------------------------------------------------------------------------------------*/


#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

const char* ssid = "Redmi";                    // Change to your WiFi SSID
const char* password = "bubble15";             // Change to your WiFi password
const char* mqtt_server = "192.168.120.11";  // Change to your MQTT Raspberry Pi broker IP

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

#define LIGHT1 2                                // Macro for LED 1
#define LIGHT2 4                                // Macro for LED 2


#define DHTPIN 5                                // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11                           // DHT 11
DHT dht(DHTPIN, DHTTYPE);                       // Configuration

void setup_wifi() {
  delay(10);                                    // Introduce a small delay for stability
  Serial.println();                             // Print a line break and a message indicating the start of Wi-Fi connection
  Serial.print("Connecting to ");
  Serial.println(ssid);                         // Print the Wi-Fi SSID being connected to

  WiFi.begin(ssid, password);                   // Start the Wi-Fi connection using the provided SSID and password

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);                                 // Wait for 500 milliseconds
    Serial.print(".");                          // Print a dot to indicate the ongoing connection attempt
  }

  randomSeed(micros());                         // Seed the random number generator with microsecond time

  Serial.println("");                           // Print a line break for formatting
  Serial.println("WiFi connected");             // Print a message indicating a successful Wi-Fi connection
  Serial.println("IP address: ");               // Print a message indicating the IP address is about to be displayed
  Serial.println(WiFi.localIP());               // Print the local IP address obtained from the Wi-Fi connection
}


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");                                  // Print a message to indicate that a message has arrived
  Serial.print(topic);                                                // Print the MQTT topic of the received message
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);                                   // Print the payload of the message as characters
  }
  Serial.println();                                                   // Move to a new line for clarity

  // Switch on the LED if a '1' was received as the first character
  if (strcmp(topic, "home/rooms") == 0) {                             // Check if the received topic is "home/rooms"
    if ((char)payload[0] == '1') {                                    // Check if the first character of the payload is '1'
      digitalWrite(LIGHT1, LOW);                                      // Turn on LED1 by setting its voltage level LOW
      digitalWrite(LIGHT2, LOW);                                      // Turn on LED2 by setting its voltage level LOW
    } else {
      digitalWrite(LIGHT1, HIGH);                                     // Turn off LED1 by setting its voltage level HIGH
      digitalWrite(LIGHT2, HIGH);                                     // Turn off LED2 by setting its voltage level HIGH
    }
  }
  else if (strcmp(topic, "home/room2/led") == 0) {                    // Check if the received topic is "home/room2/led"
    if ((char)payload[0] == '1') {                                    // Check if the first character of the payload is '1'
      digitalWrite(LIGHT1, LOW);                                      // Turn on LED1 by setting its voltage level LOW
      digitalWrite(LIGHT2, LOW);                                      // Turn on LED2 by setting its voltage level LOW
    } else {
      digitalWrite(LIGHT1, HIGH);                                     // Turn off LED1 by setting its voltage level HIGH
      digitalWrite(LIGHT2, HIGH);                                     // Turn off LED2 by setting its voltage level HIGH
    }
  }
  else if (strcmp(topic, "home/room2/light1") == 0) {                 // Check if the received topic is "home/room2/light1"
    if ((char)payload[0] == '1') {                                    // Check if the first character of the payload is '1'
      digitalWrite(LIGHT1, LOW);                                      // Turn on LED1 by setting its voltage level LOW
    } else {
      digitalWrite(LIGHT1, HIGH);                                     // Turn off LED1 by setting its voltage level HIGH
    }
  }
  else if (strcmp(topic, "home/room2/light2") == 0) {                 // Check if the received topic is "home/room2/light2"
    if ((char)payload[0] == '1') {                                    // Check if the first character of the payload is '1'
      digitalWrite(LIGHT2, LOW);                                      // Turn on LED2 by setting its voltage level LOW
    } else {
      digitalWrite(LIGHT2, HIGH);                                     // Turn off LED2 by setting its voltage level HIGH
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");                    // Print a message indicating an MQTT connection attempt
    // Create a random client ID
    String clientId = "ESP8266Client-";                               // Initialize a string with the base client ID
    clientId += String(random(0xffff), HEX);                          // Add a random hexadecimal number to the client ID
    // Attempt to connect to the MQTT broker with the generated client ID
    if (client.connect(clientId.c_str())) {                           // Check if the connection attempt was successful
      Serial.println("connected");                                    // Print a message indicating a successful connection
      // ... and resubscribe to specific MQTT topics
      client.subscribe("home/rooms");                                 // Subscribe to the "home/rooms" topic
      client.subscribe("home/room2/led");                             // Subscribe to the "home/room2/led" topic
      client.subscribe("home/room2/light1");                          // Subscribe to the "home/room2/light1" topic
      client.subscribe("home/room2/light2");                          // Subscribe to the "home/room2/light2" topic
    } else {
      Serial.print("failed, rc=");                                    // Print a message indicating a failed connection attempt and its return code
      Serial.print(client.state());                                   // Print the MQTT client's state
      Serial.println(" try again in 5 seconds");                      // Print a message indicating a retry in 5 seconds
      delay(5000);                                                    // Wait 5 seconds before retrying the MQTT connection
    }
  }
}


void setup() {
  pinMode(LIGHT1, OUTPUT);                                          // Initialize the LIGHT1 pin as an output (for an LED)
  pinMode(LIGHT2, OUTPUT);                                          // Initialize the LIGHT2 pin as an output (for an LED)
  dht.begin();                                                      // Initialize the DHT sensor
  Serial.begin(115200);                                             // Initialize the serial communication at a baud rate of 115200
  setup_wifi();                                                     // Call the setup_wifi function to establish a Wi-Fi connection
  client.setServer(mqtt_server, 1883);                              // Set the MQTT server and port for the MQTT client
  client.setCallback(callback);                                     // Set the callback function to handle incoming MQTT messages
}


void loop() {
  if (!client.connected()) {
    reconnect();                                                    // If the MQTT client is not connected, attempt to reconnect
  }
  client.loop();                                                    // Keep the MQTT client connection alive
  long now = millis();                                              // Get the current time in milliseconds
  if (now - lastMsg > 3000) {                                       // Check if it's been more than 3 seconds since the last message was sent
    lastMsg = now;                                                  // Update the last message time to the current time
    float temperatureC = dht.readTemperature();                     // Read the temperature from the DHT sensor in Celsius
    client.publish("home/room2/temp", String(temperatureC).c_str()); // Publish the temperature value to the MQTT topic "home/room2/temp"
  }
}

