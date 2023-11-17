/*
  Name          : Sreeya L Karthik
  Date          : 30 June 2023
  Project Title : INDUSTRIAL MACHINE STATE MONITORING SYSTEM (ACCESS POINT AND STATION)
*/


// Setup the server to receive data over WiFi
#include <WiFi.h>
#include <WebServer.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include "ThingSpeak.h"

int status = WL_IDLE_STATUS;           // WL_IDLE_STATUS temporary status assigned when WiFi.begin() is called

int i, len, passlen;

String ssid = "", pass = "";

char S_SSID[20];
char S_PASSWD[20];

bool flag = 1;

static int k = 0;
float current_1[100] = {0};
float current_2[100] = {0};
float current_3[100] = {0};

unsigned long myChannelnumber = 2206877;      // ThingSpeak Channel ID
const char* apiKey = "9YID8FGOLB1AIGMF";      // ThingSpeak Key


IPAddress ip(192, 168, 11, 4);                // Arbitrary IP address (doesn't conflict w/ local network)
IPAddress gateway(192, 168, 11, 1);
IPAddress subnet(255, 255, 255, 0);

// Set up the server object
WebServer server;
WiFiClient client;                        // Create a WiFiClient object named 'client'

void setup()
{
  Serial.begin(115200);
  WiFi.mode(WIFI_AP_STA);                 // Set WiFi to station mode and disconnect from any AP if it was previously connected
  WiFi.softAP("LAPTOP", "12345678");
  WiFi.softAPConfig(ip, gateway, subnet);
  WiFi.disconnect();
  delay(100);

  Serial.println("Setup done");

  Serial.println("scan start");
  int n = WiFi.scanNetworks();          // WiFi.scanNetworks will return the number of networks found
  Serial.println("scan done");

  if (n == 0)                           // If no of networks scanned is zero
  {
    Serial.println("no networks found"); // Then print network not found
  }
  else
  {
    Serial.print(n);                      // Print the number of networks found
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)                      // Print SSID and RSSI for each network found
    {
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));         // SSID
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));         // RSSI
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*"); // Check whether password is required
      delay(10);
    }
  }
  Serial.println("");

  delay(5000);                                     // Wait a bit before scanning again

  // to request data

  server.on("/update_cur_1", handleUpdate_cur_1); // use this route to update the sensor1 value
  server.on("/update_cur_2", handleUpdate_cur_2); // use this route to update the sensor2 value
  server.on("/update_cur_3", handleUpdate_cur_3); // use this route to update the sensor3 value

  server.begin();                                 // Sever begin
  ThingSpeak.begin(client);                       // ThingSpeak begin
}

void loop()
{
  if (flag)
  {
    // Read ssid and password
    Serial.println("Enter the network index : ");   // Prompt for the network index
    delay(10000);
    if (Serial.available() > 0)
    {
      i = Serial.parseInt();                        // Read and parse the input from the Serial monitor to obtain the network index
    }
    ssid = WiFi.SSID(i - 1);                        // Get the selected SSID
    len = ssid.length();                            // Length of ssid
    Serial.print("SSID len = ");
    Serial.println(len);
    ssid.toCharArray(S_SSID, len + 1);              // Convert SSID to a character array
    S_SSID[len + 1] = '\0';
    Serial.println(S_SSID);


    Serial.println("Enter the password: ");         // Prompt for the password
    delay(10000);
    if (Serial.available() > 0)
    {
      pass = Serial.readString();                   // Read the input from the Serial monitor and store it as a string
    }

    passlen = pass.length();                        // Length of password
    Serial.print("Password len = ");
    Serial.println(passlen);
    pass.toCharArray(S_PASSWD, passlen + 1);        // Convert password to a character array
    S_PASSWD[passlen + 1] = '\0';
    Serial.println(S_PASSWD);
    delay(5000);

    while (status != WL_CONNECTED)                  // Attempt to connect to the WiFi network
    {
      Serial.print("Attempting to connect to SSID: ");
      Serial.println(ssid);
      status = WiFi.begin(S_SSID, S_PASSWD);        // Connect to WPA/WPA2 network
      Serial.print("Connecting to WiFi ..");
      delay(10000);                                 // Wait 10 seconds for connection
    }
    printWifiStatus();                              // Print the WiFi connection status

    flag = 0;
  }
  delay(2000);

  server.handleClient();
  if (k++ == 99)
  {
    k = 0;
  }
}

void handleUpdate_cur_1()
{
  // The value will be passed as a URL argument
  current_1[k] = server.arg("cur_1").toFloat();  // Store the value passed as "cur_1" URL argument in the current_1 array
  Serial.print("current_1 :");
  Serial.println(current_1[k]);                 // Print the value of current_1[k] to the serial monitor

  ThingSpeak.writeField(myChannelnumber, 1, current_1[k], apiKey);  // Write the value of current_1[k] to ThingSpeak channel field 1 using the provided API key
}


void handleUpdate_cur_2()
{
  // The value will be passed as a URL argument
  current_2[k] = server.arg("cur_2").toFloat();   // Store the value passed as "cur_2" URL argument in the current_2 array
  Serial.print("current_2 :");
  Serial.println(current_2[k]);                   // Print the value of current_2[k] to the serial monitor
  ThingSpeak.writeField(myChannelnumber, 2, current_2[k], apiKey); // Write the value of current_2[k] to ThingSpeak channel field 2 using the provided API key
}

void handleUpdate_cur_3()
{
  // The value will be passed as a URL argument
  current_3[k] = server.arg("cur_3").toFloat();   // Store the value passed as "cur_3" URL argument in the current_3 array
  Serial.print("current_3 :");
  Serial.println(current_3[k]);                  // Print the value of current_3[k] to the serial monitor
  ThingSpeak.writeField(myChannelnumber, 3, current_3[k], apiKey); // Write the value of current_3[k] to ThingSpeak channel field 3 using the provided API key
}

void printWifiStatus()
{
  Serial.print("SSID: ");                        // Print the SSID of the network you're attached to
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();                 // Print your WiFi shield's IP address
  Serial.print("IP Address: ");
  Serial.println(ip);

  long rssi = WiFi.RSSI();                       // Print the received signal strength
  Serial.print("Signal Strength (RSSI): ");
  Serial.print(rssi);
  Serial.println(" dBm");
}
