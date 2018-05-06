  /**
   * **************************************************** *
   *                    DOORA (SENSOR)                    *
   *              Bishop Shanahan STEM Team               * 
   *             Programmer: Brendan Manning              *
   *            Copyright 2018 Brendan Manning            *
   * **************************************************** *
   */

  // Include requried WiFi connection libraries
  #include <SPI.h>
  #include <WiFi101.h>
  #include "SSWiFi.h"
  #include <WiFiUdp.h>

String READY_MSG = "Arduino is ready!";
bool isConnected = false;

SSWiFi wifi =  SSWiFi();
WiFiUDP Udp;
IPAddress ip(192,168,1,244);

unsigned int localPort = 8989;      // local port to listen on

char packetBuffer[255]; //buffer to hold incoming packet
char  ReplyBuffer[] = "acknowledged";       // a string to send back

void setup() {
   Serial.begin(9600);
   while(!Serial){
       // Do nothing... Wait for serial connection
       isConnected  = false;
   }
   // If we were not connected, it means we need to greet with READY_MSG
   if(!isConnected ){
       isConnected = true;
       Serial.println(READY_MSG);
   }
   
   // attempt to connect using WPA2 encryption:
  Serial.println("Attempting to connect to WPA network...");
  WiFi.config(ip);
  int status = WiFi.begin("sunset_home", "Lucy@1226");

  // if you're not connected, stop here:
  if ( status != WL_CONNECTED) { 
    Serial.println("Couldn't get a wifi connection");
    wifi.listNetworks();
    while(true);
  } 
  // if you are connected, print out info about the connection:
  else {
    Serial.println("Connected to network");

IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);


Udp.begin(localPort);
  }

  
}

void loop() {

  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remoteIp = Udp.remoteIP();
    Serial.print(remoteIp);
    Serial.print(", port ");
    Serial.println(Udp.remotePort());

    // read the packet into packetBufffer
    int len = Udp.read(packetBuffer, 255);
    if (len > 0) packetBuffer[len] = 0;
    Serial.println("Contents:");
    Serial.println(packetBuffer);

    // send a reply, to the IP address and port that sent us the packet we received
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(ReplyBuffer);
    Udp.endPacket();
  }
}

