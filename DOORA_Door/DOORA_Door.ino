  /**
   * **************************************************** *
   *                     DOORA (DOOR)                     *
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

  // Define WiFi connection objects
  SSWiFi wifi =  SSWiFi();
  WiFiUDP Udp;

  /**
   * Runtime variables
   *   char[] packetBuffer = Buffer that holds incoming messages
   */
  char packetBuffer[255];
  bool isConnected = false; 

  /**
   * Program options
   *    IPAddress ip = The IP address the door should assign itself to on the LAN
   *    unsigned int LOCAL_PORT = The port the door should listen to for messages
   *    
   *    char[] WIFI_NETWORK = WiFi network name
   *    char[] WIFI_PASSWORD = WiFi password
   *    
   */
   IPAddress ip(192,168,1,244);
   unsigned int LOCAL_PORT = 8989;
   char[] WIFI_NETWORK = "sunset_home";   char[] WIFI_PASSWORD = "Lucy@1226";
   
void setup() {

  // Wait until serial connection confirmed
  Serial.begin(9600);
  while(!Serial)
      isConnected  = false;
  isConnected = true;
   
   // Attempt to connect using WPA2 encryption:
  Serial.println("Attempting to connect to WPA network...");
  WiFi.config(ip);
  int status = WiFi.begin(WIFI_NETWORK, WIFI_PASSWORD);

  // If connection failed, log this information and halt the device
  if ( status != WL_CONNECTED) { 
    Serial.println("Couldn't get a wifi connection.");
    wifi.listNetworks();
    pinMode(6, OUTPUT);
    while(true) {
      digitalWrite(6, HIGH);
    }
  } 
  
  // if you are connected, print out info about the connection:
  else {

    // Begin UDP connection
    Udp.begin(LOCAL_PORT);

    // Emit a Serial message indicating the device is ready!
    Serial.println("----------------------------------------");
    Serial.println("             DOORA IS READY!            ");
    Serial.print("        IP Address: "); Serial.println(localIp);
    Serial.println("----------------------------------------");
 
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

