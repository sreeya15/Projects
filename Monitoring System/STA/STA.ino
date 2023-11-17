/*
  Name          : Sreeya L Karthik
  Date          : 30 June 2023
  Project Title : INDUSTRIAL MACHINE STATE MONITORING SYSTEM (STATION)
*/

#include "DHT.h"

#include "WiFi.h"

#define LDRPIN 14
#define POTPIN 27
#define DHTPIN 26

#define DHTTYPE DHT11                   // DHT 11
DHT dht(DHTPIN, DHTTYPE);               // Initialize DHT sensor.

int status = WL_IDLE_STATUS;           // WL_IDLE_STATUS temporary status assigned when WiFi.begin() is called

int i, len, passlen;

String ssid = "", pass = "";

char S_SSID[20];
char S_PASSWD[20];

int ldrValue;
int potValue;
bool flag = 1;

const char* host = "192.168.11.4"; // as specified in server.ino


// Set up the client objet
WiFiClient client;

void setup()
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);              // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.disconnect();
  delay(100);

  Serial.println("Setup done");

  dht.begin();

  pinMode(LDRPIN, INPUT);
  pinMode(POTPIN, INPUT);
  pinMode(DHTPIN, INPUT);


  Serial.println("scan start");
  int n = WiFi.scanNetworks();       // WiFi.scanNetworks will return the number of networks found
  Serial.println("scan done");

  if (n == 0)
  {
    Serial.println("no networks found");
  }
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)                // Print SSID and RSSI for each network found
    {
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*"); // Check whether password is required
      delay(10);
    }
  }
  Serial.println("");

  delay(5000);                       // Wait a bit before scanning again
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
    ssid.toCharArray(S_SSID, len + 1);                  // Convert SSID to a character array
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
    pass.toCharArray(S_PASSWD, passlen + 1);            // Convert password to a character array
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


  // Read sensor values
  ldrValue = analogRead(LDRPIN);                  // Read LDR value
  delay(1000);

  //ldrValue = dht1.readTemperature();
  float temperature = dht.readTemperature();      // Read temperature
  delay(1000);

  potValue = analogRead(POTPIN);                  // Read potentiometer value
  delay(1000);

  // Print sensor values
  //  Serial.print("LDR Value: ");
  //  Serial.println(ldrValue);
  //
  //  Serial.print("Temperature: ");
  //  Serial.println(temperature);
  //
  //  Serial.print("Potentiometer Value: ");
  //  Serial.println(potValue);

  delay(2000);

  current_update1(ldrValue);
  current_update2(temperature);
  current_update3(potValue);

  delay(10000);

}

void current_update1(int Current)
{
  // Connect to the server and send the data of sensor 1(LDR) as a URL parameter
  if (client.connect(host, 80)) {
    String url = "/update_cur_1?cur_1=";
    Serial.print("current_1 :");
    Serial.println(Current);
    url += String(Current);
    client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host +  "\r\n" +
                 "Connection: keep-alive\r\n\r\n"); // minimum set of required URL headers
    client.stop();
  }
}

void current_update2(int Current)
{
  // Connect to the server and send the data of sensor 2(DHT11) as a URL parameter
  if (client.connect(host, 80)) {
    String url = "/update_cur_2?cur_2=";
    Serial.print("current_2 :");
    Serial.println(Current);
    url += String(Current);
    client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host +  "\r\n" +
                 "Connection: keep-alive\r\n\r\n"); // minimum set of required URL headers
    client.stop();
  }
}

void current_update3(int Current)
{
  // Connect to the server and send the data of sensor 3(Potentiometer) as a URL parameter
  if (client.connect(host, 80)) {
    String url = "/update_cur_3?cur_3=";
    Serial.print("current_3 :");
    Serial.println(Current);
    url += String(Current);
    client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host +  "\r\n" +
                 "Connection: keep-alive\r\n\r\n"); // minimum set of required URL headers
    client.stop();
  }
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
