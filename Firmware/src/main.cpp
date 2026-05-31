#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <WiFi.h>
#include <WebServer.h>
#include <dashboard.html>

const char* ssid = "";
const char* pass = "";

Adafruit_SSD1306 display(128, 64, &Wire, -1);

int ref0, ref1;

int moisture = 0;
float percentage = map(moisture, 1500, 3800, 100 , 0);

WebServer server(80);

void serve() {
  server.send(200, "text/html", html);
}

void give() {
  server.send(200, "text/html", String(percentage));
}

void setup() {
  Serial.begin(115200);
  Serial.println(html);
  pinMode(21, OUTPUT);
  digitalWrite(21, LOW);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("ssd1306 failed :(");
    for(;;);
  }
  display.clearDisplay();
  display.display();
  display.setRotation(2);
  display.setTextColor(SSD1306_WHITE);

  WiFi.begin(ssid, pass);
  while(WiFi.status() != WL_CONNECTED) {
    delay(1067);
  }

  Serial.println("Local IP:");
  Serial.println(WiFi.localIP());

  server.on("/", serve);
  server.on("/data", give);
  server.begin();
}

void loop() {
  server.handleClient();
  if(analogRead(3) > 420) {
    moisture = analogRead(4);
    percentage = map(moisture, 1500, 3800, 100 , 0);
    // fully submerged = ~1500, dry = ~3800

    display.clearDisplay();

    display.setTextColor(1);
    display.setTextWrap(false);
    display.setCursor(4, 19);
    display.print("MOISTURE:");
    display.print(percentage);
    display.print("%");

    display.drawRect(5, 29, 117, 20, 1);
    display.drawRect(6, 30, 1, 1, 1);
    display.drawRect(6, 47, 1, 1, 1);
    display.drawRect(120, 30, 1, 1, 1);
    display.drawRect(120, 47, 1, 1, 0);
    display.drawRect(5, 29, 1, 1, 0);
    display.drawRect(5, 48, 1, 1, 0);
    display.drawRect(121, 29, 1, 1, 0);
    display.drawRect(121, 49, 1, 1, 0);
    display.drawRect(121, 48, 1, 1, 0);
    display.drawRect(120, 47, 1, 1, 1);

    display.fillRect(6, 30, ceil((percentage/100)*115), 18, 1);

    display.display();


    display.display();
    delay(500);
  }
}