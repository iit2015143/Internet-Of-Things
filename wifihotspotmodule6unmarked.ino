//-- Libraries Included --------------------------------------------------------------
  #include <ESP8266WiFi.h>
  #include <ESP8266WebServer.h>
  #include <ESP8266HTTPClient.h>

//------------------------------------------------------------------------------------
  // Define I/O Pins
  #define     LED0      2           // WIFI Module LED
  #define     LED1      D0          // Connectivity With Client #1
  #define     LED2      D2          // Connectivity With Client #2
  #define     BUTTON    D1          // Connectivity ReInitiate Button
//------------------------------------------------------------------------------------
  // Authentication Variables
  char*       TKDssid;              // SERVER WIFI NAME
  char*       TKDpassword;          // SERVER PASSWORD
//------------------------------------------------------------------------------------
  #define     MAXSC     6           // MAXIMUM NUMBER OF CLIENTS

  /*
   * MAXSC => Maximum Server Clients
   * They Are 6 Because
   * 1. Blue Fighter Suit
   * 2. Red Fighter Suit
   * 3. Corner Referee 1
   * 4. Corner Referee 2
   * 5. Corner Referee 3
   * 6. Corner Referee 4
   */
  ESP8266WebServer server(80);      // THE SERVER AND THE PORT NUMBER
//====================================================================================

  IPAddress local_IP(192,168,6,1);
  IPAddress gateway(192,168,6,1);
  IPAddress subnet(255,255,255,0);


//====================================================================================
  
  void SetWifi(char* Name, char* Password)
  {
    // Stop Any Previous WIFI
    WiFi.disconnect();

    // Setting The Wifi Mode
    WiFi.mode(WIFI_AP_STA);
    Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");
    Serial.println("WIFI Mode : AccessPoint Station");
    
    // Setting The AccessPoint Name & Password
    TKDssid      = Name;
    TKDpassword  = Password;
    
    // Starting The Access Point
    WiFi.softAP(TKDssid, TKDpassword);
    Serial.println("WIFI < " + String(TKDssid) + " > ... Started");
    
    // Wait For Few Seconds
    delay(1000);
    
    // Getting Server IP
    IPAddress IP = WiFi.softAPIP();
    
    // Printing The Server IP Address
    Serial.print("AccessPoint IP : ");
    Serial.println(IP);

    // Printing MAC Address
    Serial.print("AccessPoint MC : ");
    Serial.println(String(WiFi.softAPmacAddress()));
  }

void dblforward(){
  server.send(200, "text/plain", "doubleforwarding");
  
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status

 
    HTTPClient http;  //Declare an object of class HTTPClient
 
    http.begin("http://192.168.5.1:80/forward");  //Specify request destination
    int httpCode = http.GET();                                                                  //Send the request
 
    if (httpCode > 0) { //Check the returning code
 
      String payload = http.getString();   //Get the request response payload
      Serial.println(payload);                     //Print the response payload
      Serial.println(httpCode);
    }
    else{
      Serial.println("connection failed");  
    }
 
    http.end();   //Close connection

    delay(3000);
  }
}
//====================================================================================
void CheckWiFiConnectivity()
  {
    while(WiFi.status() != WL_CONNECTED)
    {
      for(int i=0; i < 10; i++)
      {
        digitalWrite(LED0, !HIGH);
        delay(250);
        digitalWrite(LED0, !LOW);
        delay(250);
        Serial.print(".");
      }
      Serial.println("");
    }
  }
  //===================roothandle==========================
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

void forward(){

  server.send(200, "text/plain", "toggling the led");
  
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status

 
    HTTPClient http;  //Declare an object of class HTTPClient
 
    http.begin("http://192.168.5.1:80/gpio2/1");  //Specify request destination
    int httpCode = http.GET();                                                                  //Send the request
 
    if (httpCode > 0) { //Check the returning code
 
      String payload = http.getString();   //Get the request response payload
      Serial.println(payload);                     //Print the response payload
      Serial.println(httpCode);
    }
    else{
      Serial.println("connection failed");  
    }
 
    http.end();   //Close connection

    delay(3000);

    http.begin("http://192.168.5.1:80/gpio2/0");  //Specify request destination
    httpCode = http.GET();                                                                  //Send the request
 
    if (httpCode > 0) { //Check the returning code
 
      String payload = http.getString();   //Get the request response payload
      Serial.println(payload);                     //Print the response payload
      Serial.println(httpCode);
    }
    else{
      Serial.println("connection failed");  
    }
 
    http.end();   //Close connection
 
  }
 
  delay(3000);
  
}
//============================================================

  void setup()
  {
    // Setting The Serial Port
    Serial.begin(115200);           // Computer Communication
    
    // Setting The Mode Of Pins
    pinMode(LED0, OUTPUT);          // WIFI OnBoard LED Light
    pinMode(LED1, OUTPUT);          // Indicator For Client #1 Connectivity
    pinMode(LED2, OUTPUT);          // Indicator For Client #2 Connectivity
    pinMode(BUTTON, INPUT_PULLUP);  // Initiate Connectivity
    
    // Print Message Of I/O Setting Progress
    Serial.println();
    Serial.println("I/O Pins Modes Set .... Done");
    server.on("/", handleRoot);
    server.on("/gpio2/0",ontwo);
    server.on("/gpio2/1",offtwo);
    server.begin();
    // Setting Up A Wifi Access Point
    if(WiFi.status() == WL_CONNECTED)
    {
      WiFi.disconnect();
      WiFi.mode(WIFI_OFF);
      delay(50);
    }
    SetWifi("module6", "iammodule6");
    WiFi.begin("module5", "iammodule5");      // The SSID That We Want To Connect To

    // Printing Message For User That Connetion Is On Process ---------------
    Serial.println("!--- Connecting To " + WiFi.SSID() + " ---!");

    // WiFi Connectivity ----------------------------------------------------
    CheckWiFiConnectivity();
    Serial.println("connected to module4");
    server.on("/", handleRoot);
    server.on("/gpio2/0",ontwo);
    server.on("/gpio2/1",offtwo);
    server.on("/forward",forward);
    server.on("/forward/forward",dblforward);
    server.begin();
    Serial.println("HTTP server started");
  }

//====================================================================================
  
void loop()
{
  server.handleClient();
}


