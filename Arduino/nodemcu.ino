// =================
// PROJNAME
// NODEMCU
// =================
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

#include <SoftwareSerial.h>

// WIFI
const char *ssid = "bantaybaha_ap";  //ENTER YOUR WIFI SETTINGS
const char *password = "12345678";
String serverName = "http://martorenzo.click/project/baha/api.php";

// Software Serial
const byte pinRx = D6;
const byte pinTx = D5;
SoftwareSerial mySerial (pinRx, pinTx);

String buildStr = "";



// ============================
// Start
// ============================
void setup() {
  // init serial pin
  pinMode(pinRx, INPUT);
  pinMode(pinTx, OUTPUT);
  
  // init serial
  Serial.begin(9600);
  mySerial.begin(9600);

  // init wifi
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}



// ============================
// Loop
// ============================
void loop() {
  if (mySerial.available() > 0) {
    char ch = mySerial.read();
    if(ch == '\n')
    {
      // web
      WiFiClient wifiClient;
      HTTPClient http;
      http.begin(wifiClient, serverName);
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      String httpRequestData = "mode=sendsms&aMsg=" + buildStr;           
      int httpResponseCode = http.POST(httpRequestData);
    
      if (httpResponseCode > 0)
      {
        String response = http.getString();
        Serial.println("rcverr: " + String(httpResponseCode));
        Serial.println("rcvstr: " + response);
      }
      else
      {
        Serial.println("rcverr: " + String(httpResponseCode));
      }
      
      http.end();

      // clear
      Serial.println(buildStr);
      buildStr = "";
    }
    else
    {
      buildStr += ch;
    }
  }
}
