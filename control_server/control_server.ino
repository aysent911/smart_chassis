#include <Arduino.h>
// Load Wi-Fi library
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

// initialize network credentials
const char* ssid = "ControlPanel";
const char* password = "RegBrake1";

// Set web server port number to 80
WiFiServer server(80);

// Assign output variables to GPIO pins
const int int0 = 2;
//const int int1 = 2;
int val = 0;

void setup() {
  //initialize GPIO2 as output HIGH
  pinMode(int0, OUTPUT);
  digitalWrite(int0, HIGH);
  
  //wait for chassis ECU to initialize
  delay (1500);

  Serial.begin(9600);

  //start WIFi AP with SSID and PASSWORD
  //Serial.print("Configuring Access Point...");
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  //Serial.print("AP IP Address: ");
  //Serial.println(myIP);
  
  // Start Web Server
  server.begin();
  
}

// Main loop
void loop(){
  // listen for incoming clients
  WiFiClient client = server.available();   

  // Do nothing if client is not available
  if (!client) {
      
     return;
  }
  
  // Wait a client 
  while(!client.available()){

      }
    
  // A new client is connected, get the request
  String request = client.readStringUntil('\r');

  digitalWrite(int0, LOW);
  delay(1);
  digitalWrite(int0, HIGH);
  
  client.flush();
 
  if (request.indexOf("/NEUTRAL") != -1)
  {
    val = 10;
    Serial.print('A'); 
  }
  if (request.indexOf("/REVERSE") != -1)
  {
    val = 1;
    Serial.print(1);
  }
  if (request.indexOf("/CENTRE") != -1)
  {
    val = 2;
    Serial.print(2); 
  }
  else if (request.indexOf("/LEFT") != -1)
  {
    val = 3;
    Serial.print(3); 
  }
  else if (request.indexOf("/RIGHT") != -1)
  {
    val = 4;
    Serial.print(4); 
  }
  else if (request.indexOf("/FARLEFT") != -1)
  {
    val = 5;
    Serial.print(5); 
  }
  else if (request.indexOf("/FARRIGHT") != -1) 
  {
    val = 6;
    Serial.print(6);
  } 
  else if (request.indexOf("DRIVE1") != -1)
  {
    val = 7;
    Serial.print(7);
  }
  else if (request.indexOf("DRIVE2") != -1)
  {
    val = 8;
    Serial.print(8);
  }
  else if (request.indexOf("HANDBRAKE") != -1)
  {
    val = 9;
    Serial.print(9);
  }
  else 
  {
    val = 0;
    Serial.print(0);
  }
  // Display GPIO status
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); 

  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.print("<h1>State: </h1>"); 

  if(val ==10) {
    client.print("<h1>NEUTRAL</h1>");  
  }
  else if(val ==1){
    client.print("<h1>REVERSE</h1>");
  }
  else if(val ==2){
    client.print("<h1>STEERING CENTRED</h1>");
  }
  else if(val ==3){
    client.print("<h1>LEFT TURN</h1>");
  }
  else if(val ==4){
    client.print("<h1>RIGHT TURN</h1>");
  }
  else if(val ==5){
    client.print("<h1>FAR LEFT TURN</h1>");
  }
  else if(val ==6){
    client.print("<h1>FAR RIGHT TURN</h1>");
  }
  else if(val ==7){
    client.print("<h1>DRIVE SPEED1</h1>");
  }
  else if(val ==8){
    client.print("<h1>DRIVE SPEED2</h1>");
  }
  else if(val ==9)
  {
    client.print("<h1>HAND BRAKE</h1>");
  }
  //write to client
  client.println("<br><br>");
  client.println("<form>");
  client.println("<center>");
  client.println("<svg width=\"250\" height=\"150\">");
  client.println("<a href=\"/REVERSE\">");
  client.println("<circle cx=\"125\" cy=\"75\" r=\"70\" fill=\"blue\"/>");
  client.println("<text x=\"125\" y=\"75\" fill=\"white\" text-anchor=\"middle\" alignment-baseline=\"middle\" style=\"font-size:34px\">R</text>");
  client.println("</a>");
  client.println("</svg>");
  client.println("</center>");
  client.println("<br>");
  client.println("<center>");
  client.println("<svg width=\"250\" height=\"150\">");
  client.println("<a href=\"/NEUTRAL\">");
  client.println("<circle cx=\"125\" cy=\"75\" r=\"70\" fill=\"blue\"/>");
  client.println("<text x=\"125\" y=\"75\" fill=\"white\" text-anchor=\"middle\" alignment-baseline=\"middle\" style=\"font-size:34px\">N</text>");
  client.println("</a>");
  client.println("</svg>");
  client.println("</center>");
  client.println("<br>");
  client.println("<center>");
  client.println("<svg width=\"150\" height=\"150\">");
  client.println("<a href=\"/FARLEFT\">");
  client.println("<path d=\"M  20 75 L  100  15 L  100 45 L  150 45 L  150 105 L  100 105 L  100 135 Z\" fill=\"blue\"/>");
  client.println("<text x=\"100\" y=\"80\" fill=\"white\" text-anchor=\"middle\" alignment-baseline=\"middle\" style=\"font-size:34px\"><<</text>");
  client.println("</a>");
  client.println("</svg>");
  client.println("<svg width=\"200\" height=\"150\">");
  client.println("<a href=\"/LEFT\">");
  client.println("<path d=\"M  20 75 L  100  15 L  100 45 L  150 45 L  150 105 L  100 105 L  100 135 Z\" fill=\"blue\"/>");
  client.println("<text x=\"100\" y=\"80\" fill=\"white\" text-anchor=\"middle\" alignment-baseline=\"middle\" style=\"font-size:34px\"><</text>");
  client.println("</a>");
  client.println("</svg>");
  client.println("<svg width=\"200\" height=\"150\">");
  client.println("<a href=\"/CENTRE\">");
  client.println("<circle cx=\"100\" cy=\"75\" r=\"70\" fill=\"blue\"/>");
  client.println("<text x=\"100\" y=\"75\" fill=\"white\" text-anchor=\"middle\" alignment-baseline=\"middle\" style=\"font-size:34px\">SC</text>");
  client.println("</a>");
  client.println("</svg>");
  client.println("<svg width=\"200\" height=\"150\">");
  client.println("<a href=\"/RIGHT\">");
  client.println("<path d=\"M  180 75 L  100  15 L  100 45 L  50 45 L  50 105 L  100 105 L  100 135 Z\" fill=\"blue\"/>");
  client.println("<text x=\"100\" y=\"80\" fill=\"white\" text-anchor=\"middle\" alignment-baseline=\"middle\" style=\"font-size:34px\">></text>");
  client.println("</a>");
  client.println("</svg>");
  client.println("<svg width=\"150\" height=\"150\">");
  client.println("<a href=\"/FARRIGHT\">");
  client.println("<path d=\"M  130 75 L  50  15 L  50 45 L  0 45 L  0 105 L  50 105 L  50 135 Z\" fill=\"blue\"/>");
  client.println("<text x=\"50\" y=\"80\" fill=\"white\" text-anchor=\"middle\" alignment-baseline=\"middle\" style=\"font-size:34px\">>></text>");
  client.println("</a>");
  client.println("</svg>");
  client.println("</center>");
  client.println("<br>");
  client.println("<center>");
  client.println("<svg width=\"250\" height=\"150\">");
  client.println("<a href=\"/DRIVE1\">");
  client.println("<circle cx=\"125\" cy=\"75\" r=\"70\" fill=\"blue\"/>");
  client.println("<text x=\"125\" y=\"75\" fill=\"white\" text-anchor=\"middle\" alignment-baseline=\"middle\" style=\"font-size:34px\">D1</text>");
  client.println("</a>");
  client.println("</svg>");
  client.println("</center>");
  client.println("<br>");
  client.println("<center>");
  client.println("<svg width=\"250\" height=\"150\">");
  client.println("<a href=\"/DRIVE2\">");
  client.println("<circle cx=\"125\" cy=\"75\" r=\"70\" fill=\"blue\"/>");
  client.println("<text x=\"125\" y=\"75\" fill=\"white\" text-anchor=\"middle\" alignment-baseline=\"middle\" style=\"font-size:34px\">D2</text>");
  client.println("</a>");
  client.println("</svg>");
  client.println("</center>");
  client.println("<br>");
  client.println("<center>");
  client.println("<svg width=\"250\" height=\"150\">");
  client.println("<a href=\"/HANDBRAKE\">");
  client.println("<circle cx=\"125\" cy=\"75\" r=\"70\" fill=\"blue\"/>");
  client.println("<text x=\"125\" y=\"75\" fill=\"white\" text-anchor=\"middle\" alignment-baseline=\"middle\" style=\"font-size:34px\">HB</text>");
  client.println("</a>");
  client.println("</svg>");
  client.println("</center>");
  client.println("<br><br>");
  client.println("</form>");
  client.println("</html>");
  
}
