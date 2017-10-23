#include <ESP8266WiFi.h>

#include <WiFiClient.h>

#include <ESP8266WebServer.h>

/*IPAddress local_IP(192,168,5,1);
IPAddress gateway(192,168,5,1);
IPAddress subnet(255,255,255,0);*/

const char *ssid = "module4";

const char *password = "iammodule4";

ESP8266WebServer server(80);

void ontwo(){
  digitalWrite(2,LOW);
  String s = "low";
  server.send(200, "text/plain", s);
}

void offtwo(){
  digitalWrite(2,HIGH);
  String s = "high";
  server.send(200, "text/plain", s);
}

void handleRoot() {
  String s = "you did not input anything";
  server.send(200, "text/plain", s);
}

void setup() {

delay(2000);

Serial.begin(115200);

pinMode(2, OUTPUT);
Serial.println();

Serial.print("Configuring access point...");

//Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");
WiFi.disconnect();

    // Setting The Wifi Mode
WiFi.mode(WIFI_AP_STA);
Serial.println("WIFI Mode : AccessPoint Station");

// Starting The Access Point
WiFi.softAP(ssid, password);
Serial.println("WIFI < " + String(ssid) + " > ... Started");

// Wait For Few Seconds
delay(1000);

// Getting Server IP
IPAddress IP = WiFi.softAPIP();

// Printing The Server IP Address
Serial.print("AccessPoint IP : ");
Serial.println(IP);

server.on("/", handleRoot);
server.on("/gpio2/0",ontwo);
server.on("/gpio2/1",offtwo);
server.begin();

Serial.println("HTTP server started");

}

void loop() {

server.handleClient();

}


