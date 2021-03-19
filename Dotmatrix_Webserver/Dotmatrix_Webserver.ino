#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>
#include <time.h>

const char *ssid     = "C1F3R";
const char *password = "314159265";

int pinCS = D4; 
int numberOfHorizontalDisplays = 4;
int numberOfVerticalDisplays   = 1;
char time_value[20];
String message, webpage;
    
ESP8266WebServer server(80); 
                               
Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);
int wait   = 75; // In milliseconds between scroll movements
int spacer = 1;
int width  = 5 + spacer; // The font width is 5 pixels
String SITE_WIDTH =  "1000";

void setup() {
  Serial.begin(115200); 
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  // Wait for connection
  int i = 0;
  while (WiFi.status() != WL_CONNECTED && i++ <= 10) {//wait 10 seconds
    delay(1000);
  }
  if(i == 11){
    Serial.print("Could not connect to network...");
    while(1) delay(500);
  }
  Serial.println(F("Use this URL to connect: http://")); Serial.println(WiFi.localIP().toString()+"/");// Print the IP address

  server.on("/",GetMessage);
  server.begin(); Serial.println(F("Webserver started..."));
  matrix.setIntensity(2);    
  matrix.setRotation(0, 0);  
  matrix.setRotation(1, 0);  
  matrix.setRotation(2, 0);  
  matrix.setRotation(3, 0);  
  //wait    = 50;
  //message = "Welcome";
}

void loop() {
  server.handleClient();
  display_message(message); // Display the message
}

void display_message(String message){
   for ( int i = 0 ; i < width * message.length() + matrix.width() - spacer; i++ ) {
    //matrix.fillScreen(LOW);
    int letter = i / width;
    int x = (matrix.width() - 1) - i % width;
    int y = (matrix.height() - 8) / 2; // center the text vertically
    while ( x + width - spacer >= 0 && letter >= 0 ) {
      if ( letter < message.length() ) {
        matrix.drawChar(x, y, message[letter], HIGH, LOW, 1); // HIGH LOW means foreground ON, background OFF, reverse these to invert the display!
      }
      letter--;
      x -= width;
    }
    matrix.write(); 
    delay(wait/2);
  }
}

void GetMessage() {
  webpage = ""; 
  append_page_header();
  String IPaddress = WiFi.localIP().toString();
  webpage += "<form action=\"http://"+IPaddress+"\" method=\"POST\">";
  webpage += F("Write Something !!<br><br><input type='text' size='50' name='message' value='' >");
  webpage += F("</form><br/><br/>");
  server.send(200, "text/html", webpage); 
  if (server.args() > 0 ) { 
    for ( uint8_t i = 0; i < server.args(); i++ ) {
      String Argument_Name   = server.argName(i);
      String client_response = server.arg(i);
      if (Argument_Name == "message")    message = client_response;
    }
  }
}

void append_page_header() {
  webpage  = F("<!DOCTYPE html><html><head>"); // Change lauguage (en) as required
  webpage += F("<meta http-equiv='refresh' content='60'/>"); // 60-sec refresh time
  webpage += F("<meta http-equiv='content-type' content='text/html; charset=UTF-8'/>");
  webpage += F("<title>( ͡° ͜ʖ ͡°)</title><style>");
  webpage += F("body {width:");
  webpage += SITE_WIDTH;
  webpage += F("px;margin:0 auto;font-family:arial;font-size:14px;text-align:center;color:#cc66ff;background-color:#000000;}");
  webpage += F("ul{list-style-type:none;margin:0;padding:0;overflow:hidden;background-color:#0000;font-size:14px;}");
  webpage += F("li{float:left;border-right:1px solid #bbb;}last-child {border-right:none;}");
  webpage += F("li a{display: block;padding:2px 12px;text-decoration:none;}");
  webpage += F("li a:hover{background-color:#000000;}");
  webpage += F("section {font-size:16px;}");
  webpage += F("p {background-color:#E3D1E2;font-size:16px;}");
  webpage += F("div.header,div.footer{padding:0.5em;color:white;background-color:black;clear:left;}");
  webpage += F("h1{background-color:#000000;font-size:26px;}");
  webpage += F("h2{color:#9370DB;font-size:22px;line-height:65%;}");
  webpage += F("h3{color:#9370DB;font-size:16px;line-height:55%;}");
  webpage += F("table{font-family:arial,sans-serif;font-size:16px;border-collapse:collapse;width:100%;height:100%;}");
  webpage += F("td {border:0px solid black;text-align:center;padding:2px;}");
  webpage += F("th {border:0px solid black;text-align:center;padding:2px;font-size:22px;}");
  webpage += F(".style1 {text-align:center;font-size:50px;background-color:#D8BFD8;height:57px;}");
  webpage += F(".style2 {text-align:center;font-size:16px;background-color:#ADD8E6;color:#0066ff;height:25px;}");
  webpage += F(".style3 {text-align:center;font-size:78px;background-color:#FFE4B5;height:107px;}");
  webpage += F(".style4 {text-align:center;font-size:16px;background-color:#FFE4B5;height:30px;}");
  webpage += F(".style5 {text-align:center;font-size:20px;background-color:#D9BFD9;}");
  webpage += F(".style6 td {border:0px solid black;text-align:right;padding:0px;font-size:14px;background-color:#B0C4DE;color:#0066ff;height:20px;}");
  webpage += F(".style7 {text-align:center;font-size:12px;background-color:#F7F2Fd;width:100%;}");
  webpage += F(".style8 {text-align:center;border:0px solid black;padding:2px;color:#000000;}");
  webpage += F(".style9 {text-align:center;font-size:14px;color:blue;}");
  webpage += F("img.imgdisplay {display:block;margin-left:auto;margin-right:auto;}");
  webpage += F("sup {vertical-align:super;font-size:26px;}");
  webpage += F("sup1{vertical-align:super;font-size:10px;}");
  webpage += F("</style></head><body><h1> ");
  webpage += "C1F3R </h1>";
}
