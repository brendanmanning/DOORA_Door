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

  // Parse network messages as JSON
  #include <ArduinoJson.h>

  // Define WiFi connection objects
  SSWiFi wifi = SSWiFi();
  WiFiUDP Udp;

  /**
   * Runtime variables
   *   char[] packetBuffer = Buffer that holds incoming messages
   */
  char packetBuffer[255];
  bool isConnected = false; 

  /**
   * Program options
   * 
   *    bool M_LOG_NETWORK_STATS = Should IP of sender, port of sender, etc be logged when a mesage is recieved?
   * 
   *    IPAddress ip = The IP address the door should assign itself to on the LAN
   *    unsigned int LOCAL_PORT = The port the door should listen to for messages
   *    
   *    char[] WIFI_NETWORK = WiFi network name
   *    char[] WIFI_PASSWORD = WiFi password
   *    
   */
   bool M_LOG_NETWORK_STATS = true;
   IPAddress ip(192,168,1,244);
   unsigned int LOCAL_PORT = 8989;
   char WIFI_NETWORK[] = "sunset_home";   
   char WIFI_PASSWORD[] = "Lucy@1226";
   
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
    Serial.print("   IP Address: "); Serial.print(ip); Serial.print(":"); Serial.println(LOCAL_PORT);
    Serial.println("----------------------------------------");
 
  }
}

void loop() {

  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {

    // If enabled, log statistics about the size and sender of the packet
    if(M_LOG_NETWORK_STATS) {
      Serial.println("---------------------- PACKET STATS ----------------------");
      Serial.print("     Packet size: "); Serial.print(packetSize); Serial.print(" Sender IP: "); Serial.print(Udp.remoteIP()); Serial.print(":"); Serial.println(Udp.remotePort());
      Serial.println("----------------------------------------------------------");
    }

    // Read the package into the buffer
    int len = Udp.read(packetBuffer, 255);
    if (len > 0) packetBuffer[len] = 0;

    // Handle the message with the callback
    handlePacket(packetBuffer);

  }
}

/**
 * Called everytiime a valid string package is recieved
 */
void handlePacket(char packet[]) {
  
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& message = jsonBuffer.parseObject(packet);

  const char* statusmsg;
  const char* sendermsg;

  statusmsg = message[String("status")];
  sendermsg = message[String("sender")];

  if(strcmp(statusmsg,"warn") == 0) {    
    unlock(sendermsg, message[String("method")]);
  }

  if(strcmp(statusmsg,"et") == 0) {
    et(sendermsg);
  }
}

/**
 * Called when a status=warn is recieved
 * (Unlock door)
 */
 void unlock(const char sender[], const char method[]) {
  Serial.println("Unlock door.");
  Serial.print("\tFrom: "); Serial.println(sender);
  Serial.print("\tMethod: "); Serial.println(method);
 }

 /**
  * Called when a status=et is recieved
  */
 void et(const char sensor[]) {
  Serial.print(sensor); Serial.println(" is still here");
 }

