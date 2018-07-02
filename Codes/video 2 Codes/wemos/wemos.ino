#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <functional>
#include "switch.h"
#include "UpnpBroadcastResponder.h"
#include "CallbackFunction.h"

// prototypes
boolean connectWifi();

//on/off callbacks 
bool lightOneOn();
bool lightOneOff();
bool lighttwoOn();
bool lighttwoOff();

// Change this before you flash
const char* ssid = "YOUR SSID";
const char* password = "YOUR PASSWORD";

boolean wifiConnected = false;

UpnpBroadcastResponder upnpBroadcastResponder;

Switch *lightone = NULL;
Switch *lighttwo = NULL;

bool islightOneOn = false;
bool islighttwotsOn = false;

void setup()
{
  Serial.begin(9600);
   
  // Initialise wifi connection
  wifiConnected = connectWifi();
  
  if(wifiConnected){
    upnpBroadcastResponder.beginUdpMulticast();
    
    // Define your switches here. Max 10
    // Format: Alexa invocation name, local port no, on callback, off callback
    lightone = new Switch("light one", 80, lightOneOn, lightOneOff);
    lighttwo = new Switch("light two", 81, lighttwoOn, lighttwoOff);

    Serial.println("Adding switches upnp broadcast responder");
    upnpBroadcastResponder.addDevice(*lightone);
    upnpBroadcastResponder.addDevice(*lighttwo);
  }
}
 
void loop()
{
   if(wifiConnected){
      upnpBroadcastResponder.serverLoop();
      
      lightone->serverLoop();
      lighttwo->serverLoop();
   }
}

bool lightOneOn() {
    Serial.println("Switch 1 turn on ...");
    
    islightOneOn = true;    
    return islightOneOn;
}

bool lightOneOff() {
    Serial.println("Switch 1 turn off ...");

    islightOneOn = false;
    return islightOneOn;
}

bool lighttwoOn() {
    Serial.println("Switch 2 turn on ...");

    islighttwotsOn = true;
    return islighttwotsOn;
}

bool lighttwoOff() {
  Serial.println("Switch 2 turn off ...");

  islighttwotsOn = false;
  return islighttwotsOn;
}

// connect to wifi – returns true if successful or false if not
boolean connectWifi(){
  boolean state = true;
  int i = 0;
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting ...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 10){
      state = false;
      break;
    }
    i++;
  }
  
  if (state){
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("");
    Serial.println("Connection failed.");
  }
  
  return state;
}
