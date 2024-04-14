#include "Adafruit_TLC5947.h"

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// How many boards do you have chained?
#define NUM_TLC5974 2

#define data D0
#define clock D1
#define latch D2
#define oe -1  // set to -1 to not use the enable pin (its optional)

int cols[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30 };
int rows[] = { 0, 2, 14, 12, 13, 15 };
#define uit 0
#define aan 4095

int x = 30;

Adafruit_TLC5947 tlc = Adafruit_TLC5947(NUM_TLC5974, clock, data, latch);


/*Put your SSID & Password*/
const char* ssid = "telenet-E2B7EEB";   //REPLACE WITH PHONE HOTSPOT
const char* password = "Ks2bwcznZcxv";  //REPLACE WITH PHONE HOTSPOT

ESP8266WebServer server(80);

bool powerButton;
bool button2;
bool scanButton;

void setup() {
  Serial.begin(115200);
  delay(100);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(0, OUTPUT);

  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(16, OUTPUT);

  Serial.println("TLC5974 test");
  tlc.begin();
  if (oe >= 0) {
    pinMode(oe, OUTPUT);
    digitalWrite(oe, LOW);
  }

  Serial.println("Connecting to ");
  Serial.println(ssid);

  //connect to your local wi-fi network
  WiFi.begin(ssid, password);

  //check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handle_OnConnect);
  server.on("/powerOn", handle_powerOn);
  server.on("/powerOff", handle_powerOff);
  server.on("/eyes", handle_eyes);
  server.on("/eyes2", handle_eyes2);
  server.on("/scan", handle_scan);
  server.on("/cut", handle_cut);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
}
void loop() {
  server.handleClient();

  if (scanButton) {
    scan();
  } else {
    scanButton = 0;
  }
}

void powerOn() {

  digitalWrite(0, HIGH);  /*1*/
  digitalWrite(2, HIGH);  /*2*/
  digitalWrite(14, HIGH); /*3*/
  digitalWrite(12, HIGH); /*4*/
  digitalWrite(13, HIGH); /*5*/
  digitalWrite(15, HIGH); /*6*/

  for (int i = 30; i >= 0; i--) {
    tlc.setPWM(cols[i], aan);
    tlc.write();
    delay(40);
  }

  delay(1000);
  cut();

  powerButton = 1;
  eyes2();
}

void powerOff() {
  burst();
  delay(10);

  for (int i = 0; i <= 30; i++) {
    tlc.setPWM(cols[i], uit);
    tlc.write();
    delay(40);
  }

  powerButton = 0;
}

void eyes() {
  digitalWrite(0, LOW);   /*1*/
  digitalWrite(2, LOW);   /*2*/
  digitalWrite(14, HIGH); /*3*/
  digitalWrite(12, HIGH); /*4*/
  digitalWrite(13, LOW);  /*5*/
  digitalWrite(15, LOW);  /*6*/

  for (int i = 0; i < 9; i++) {
    tlc.setPWM(i, uit);
  }
  tlc.setPWM(9, aan);
  tlc.setPWM(10, aan);
  tlc.setPWM(11, aan);
  tlc.setPWM(12, aan);
  for (int i = 13; i < 18; i++) {
    tlc.setPWM(i, uit);
  }
  tlc.setPWM(18, aan);
  tlc.setPWM(19, aan);
  tlc.setPWM(20, aan);
  tlc.setPWM(21, aan);
  for (int i = 22; i < 31; i++) {
    tlc.setPWM(i, uit);
  }
  tlc.write();

  button2 = 0;
}

void eyes2() {
  digitalWrite(0, LOW);   /*1*/
  digitalWrite(2, HIGH);  /*2*/
  digitalWrite(14, HIGH); /*3*/
  digitalWrite(12, HIGH); /*4*/
  digitalWrite(13, HIGH); /*5*/
  digitalWrite(15, LOW);  /*6*/

  for (int i = 0; i < 10; i++) {
    tlc.setPWM(i, uit);
  }
  tlc.setPWM(10, aan);
  tlc.setPWM(11, aan);
  for (int i = 12; i < 19; i++) {
    tlc.setPWM(i, uit);
  }
  tlc.setPWM(19, aan);
  tlc.setPWM(20, aan);
  for (int i = 21; i < 31; i++) {
    tlc.setPWM(i, uit);
  }
  tlc.write();

  button2 = 1;
}

void scan() {
  Serial.print('Scan before sB = 1');
  Serial.println(scanButton);

  scanButton = 1;
  Serial.print('Scan after sB = 1');
  Serial.println(scanButton);

  digitalWrite(0, HIGH);  /*1*/
  digitalWrite(2, HIGH);  /*2*/
  digitalWrite(14, HIGH); /*3*/
  digitalWrite(12, HIGH); /*4*/
  digitalWrite(13, HIGH); /*5*/
  digitalWrite(15, HIGH); /*6*/

  while (scanButton) {

    for (int i = 30; i >= 0; i--) {
      tlc.setPWM(cols[i], aan);
      tlc.write();
      delay(40);
      tlc.setPWM(cols[i], uit);
      tlc.write();
    }
    break;
  }
}

void cut() {
  Serial.print('Cut before sB = 0');
  Serial.println(scanButton);
  scanButton = 0;
  Serial.print('Cut after sB = 0');
  Serial.println(scanButton);

  digitalWrite(0, LOW);  /*1*/
  digitalWrite(2, LOW);  /*2*/
  digitalWrite(14, LOW); /*3*/
  digitalWrite(12, LOW); /*4*/
  digitalWrite(13, LOW); /*5*/
  digitalWrite(15, LOW); /*6*/

  for (int i = 0; i <= 30; i++) {
    tlc.setPWM(cols[i], uit);
    tlc.write();
  }
}

void burst() {
  digitalWrite(0, HIGH);  /*1*/
  digitalWrite(2, HIGH);  /*2*/
  digitalWrite(14, HIGH); /*3*/
  digitalWrite(12, HIGH); /*4*/
  digitalWrite(13, HIGH); /*5*/
  digitalWrite(15, HIGH); /*6*/

  for (int i = 0; i <= 30; i++) {
    tlc.setPWM(cols[i], aan);
    tlc.write();
  }
}




void handle_OnConnect() {
  server.send(200, "text/html", SendHTML(powerButton, button2, scanButton));
}

void handle_powerOn() {
  powerOn();

  server.send(200, "text/html", SendHTML(powerButton, button2, scanButton));
}

void handle_powerOff() {
  powerOff();

  server.send(200, "text/html", SendHTML(powerButton, button2, scanButton));
}

void handle_eyes() {
  eyes();

  server.send(200, "text/html", SendHTML(powerButton, button2, scanButton));
}

void handle_eyes2() {
  eyes2();

  server.send(200, "text/html", SendHTML(powerButton, button2, scanButton));
}

void handle_cut() {
  cut();

  server.send(200, "text/html", SendHTML(powerButton, button2, scanButton));
}

void handle_scan() {
  server.send(200, "text/html", SendHTML(powerButton, button2, scanButton));
  scan();
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

String SendHTML(uint8_t powerState, uint8_t button2stat, uint8_t scanState) {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>Visor Controls</title>\n";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr += ".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr += ".button-on {background-color: #1abc9c;}\n";
  ptr += ".button-on:active {background-color: #16a085;}\n";
  ptr += ".button-off {background-color: #34495e;}\n";
  ptr += ".button-off:active {background-color: #2c3e50;}\n";
  ptr += "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<h1>Visor Controls</h1>\n";
  ptr += "<h3>Work in progress</h3>\n";

  if (powerState) {
    ptr += "<p>Visor Status: ON</p><a class=\"button button-off\" href=\"/powerOff\"> Turn OFF</a>\n";
  } else {
    ptr += "<p>Visor Status: OFF</p><a class=\"button button-on\" href=\"/powerOn\">Turn ON</a>\n";
  }

  if (button2stat) {
    ptr += "<p>Eye mode: 1</p><a class=\"button button-off\" href=\"/eyes\">2</a>\n";
  } else {
    ptr += "<p>Eye mode: 2</p><a class=\"button button-on\" href=\"/eyes2\">1</a>\n";
  }


  if (scanState) {
    ptr += "<p>Scan state:</p><a class=\"button button-off\" href=\"/scan\">OFF</a>\n";
  } else {
    ptr += "<p>Scan state:</p><a class=\"button button-on\" href=\"/cut\">ON</a>\n";
  }

  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}
