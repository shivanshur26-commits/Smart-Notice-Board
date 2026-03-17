#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const char* ssid = "DIP-XPON-2.4G";
const char* password = "12345678";

WebServer server(80);

String message = "SMART NOTICE";

void handleRoot() {

  String page = "<html><body>";
  page += "<h2>Smart Notice Board</h2>";
  page += "<form action='/msg'>";
  page += "Enter Message:<br>";
  page += "<input type='text' name='text'>";
  page += "<input type='submit' value='Send'>";
  page += "</form></body></html>";

  server.send(200, "text/html", page);
}

void handleMessage() {

  message = server.arg("text");

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,20);
  display.println(message);
  display.display();

  server.send(200,"text/html","Message Sent to OLED");
}

void setup() {

  Serial.begin(115200);

  WiFi.begin(ssid,password);

  while(WiFi.status()!=WL_CONNECTED){
    delay(500);
    Serial.println("Connecting...");
  }

  Serial.println("WiFi Connected");
  Serial.println(WiFi.localIP());

  if(!display.begin(SSD1306_SWITCHCAPVCC,0x3C)){
    Serial.println("OLED failed");
    while(true);
  }

  display.clearDisplay();

  server.on("/",handleRoot);
  server.on("/msg",handleMessage);

  server.begin();
}

void loop() {
  server.handleClient();
}