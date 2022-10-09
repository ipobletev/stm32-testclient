#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "userconfig.h"

#define TIME_TOSEND 10000

// Network
const char* wifi_ssid = WIFI_SSDI;
const char* wifi_pass = WIFI_PASS;

// Wifi localhost
// WiFiServer wifiServer(WIFI_PORT);

// Wifi objetive_server
const char* server_ip = HOST_SERVER;
const int16_t server_port = HOST_PORT;
WiFiClient objetive_server;

unsigned long last_time=0;

void setup() {

  Serial.begin(115200);

  delay(1000);

  WiFi.begin(wifi_ssid, wifi_pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");
  Serial.print("MyIP:");  
  Serial.println(WiFi.localIP());  

  //Connect to server
  objetive_server.connect(server_ip, server_port);

  last_time=millis();

}

void sendToServer(String message) 
{  
  objetive_server.print(message);
  Serial.println("SendData: " + message); 
}

void loop() {

  // Check connection
  //Serial.println(objetive_server.connected());
  if (objetive_server.connected()) {
  
    //Read Data
    if (objetive_server.available()) {
      char c = objetive_server.read();
      Serial.print(c);
    }
    
    // Write data
    if(millis() - last_time > TIME_TOSEND)
    {
      sendToServer("isSTM32");
      last_time=millis();
    }
  }else{
    //Reset connection
    Serial.println("Connection to host failed. Reconnecting...");
    objetive_server.connect(server_ip, server_port);
  }
}