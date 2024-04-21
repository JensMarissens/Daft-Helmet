#include "alpha_numeric_library.h"
#include <Arduino.h>
#include <user_interface.h>

#include "Adafruit_TLC5947.h"

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define NUM_TLC5974 2

#define data D0
#define clock D1
#define latch D2
#define oe -1

#define uit 0
#define aan 4095

Adafruit_TLC5947 tlc = Adafruit_TLC5947(NUM_TLC5974, clock, data, latch);


/* NETWORK CREDENTIALS */
const char* ssid = " ";   //REPLACE WITH PHONE HOTSPOT
const char* password = "";  //REPLACE WITH PHONE HOTSPOT

ESP8266WebServer server(80);


/*------VARIABLES------*/
int cols[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30 };
int rows[] = { 0, 2, 14, 12, 13, 15 };

bool powerButton = 0;
bool eyeStyleButton;
bool scanButton;
bool textButton = 0;
String textInput = "";

unsigned long timeVal = 600000;  //(10 minutes)  OR you could use LONG_MAX;

enum Mode { POWER_ON,
            POWER_DOWN,
            EYES1,
            EYES2,
            EYES3,
            HALF_SCAN,
            SCAN,
            SHOW_TEXT,
            BLANK };

Mode mode;

/*------SETUP------*/
void setup() {
  Serial.begin(115200);

  pinMode(rows[0], OUTPUT);
  pinMode(rows[1], OUTPUT);
  pinMode(rows[2], OUTPUT);
  pinMode(rows[3], OUTPUT);
  pinMode(rows[4], OUTPUT);
  pinMode(rows[5], OUTPUT);

  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(16, OUTPUT);

  /*------INITIATE POWER_ON ANIMATION------*/
  rst_info* resetInfo = ESP.getResetInfoPtr();

  if (resetInfo->reason == 6) {
    mode = POWER_ON;
  }
  /*---------------------------------------*/


  /*----------SETUP WEB INTERFACE----------*/
  Serial.println("TLC5974 test");
  tlc.begin();
  // if (oe >= 0) {
  pinMode(oe, OUTPUT);
  // digitalWrite(oe, LOW);
  digitalWrite(oe, HIGH);
  //}

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
  server.on("/powerOff", handle_powerOff);
  server.on("/eyes=0", handle_eyes);
  server.on("/eyes=1", handle_eyes2);
  server.on("/scan", handle_scan);
  server.on("/halfScan", handle_halfScan);
  server.on("/cut", handle_cut);
  server.on("/updateText", handle_text);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
  /*---------------------------------------*/
}
/*---------------------------------------*/

/*------LOOP------*/
void loop() {
  server.handleClient();

  switch (mode) {
    case POWER_ON:
      if (!powerButton) {
        powerOn();
      }
      break;
    case EYES3:
      if (scanButton) {
        eyes3();
      }
      break;
    case SCAN:
      if (scanButton) {
        scan();
      }
      break;
    case HALF_SCAN:
      if (scanButton) {
        halfScan();
      }
      break;
    case SHOW_TEXT:
      if (textButton) {
        textInputFunction();
      }
      break;
    case POWER_DOWN:
      if (timeVal + 1000 <= millis()) {
        ESP.deepSleep(0);
      }
      break;
    default: //add deepsleep after timeVal (10 minutes). Maybe consider a hard 'force on' parameter for wearing. like a 'Display mode'. When not battery powered as well maybe?
      break;
  }
}
/*------END LOOP------*/


/*-----POWER MODES-----*/
void powerOn() {
  digitalWrite(oe, LOW);

  setRows(1, 1, 1, 1, 1, 1);

  for (int i = 30; i >= 0; i--) {
    tlc.setPWM(cols[i], aan);
    tlc.write();
    delay(40);
  }

  delay(500); //shorten
  cut();

  eyes2(eyeStyleButton);
  powerButton = 1;
}

void powerOff() {
  mode = POWER_DOWN;
  digitalWrite(oe, HIGH);

  burst();
  delay(10);

  for (int i = 0; i <= 30; i++) {
    tlc.setPWM(cols[i], uit);
    tlc.write();
    delay(40);
  }

  powerButton = 0;
  timeVal = millis();
}
/*----END POWER MODES----*/


/*----EYE STYLES (static)----*/
void eyes(int eyeStyleButton) {
  cut();

  setRows(0, 0, 1, 1, 0, 0);

  for (int i = 0; i < 9; i++) {
    tlc.setPWM(i, uit);
  }

  for (int i = 10; i < 14; i++) {
    tlc.setPWM(cols[i], aan);
  }

  for (int i = 13; i < 18; i++) {
    tlc.setPWM(i, uit);
  }

  for (int i = 19; i < 23; i++) {
    tlc.setPWM(cols[i], aan);
  }

  for (int i = 22; i < 31; i++) {
    tlc.setPWM(i, uit);
  }
  tlc.write();
}


void eyes2(int eyeStyleButton) {
  cut();

  setRows(0, 1, 1, 1, 1, 0);

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
}


void eyes3() {
  cut();

  scanButton = 1;
  mode = EYES3;

  while (scanButton) {
    digitalWrite(0, HIGH); /*Line 1*/
    tlc.setPWM(10, aan);
    tlc.setPWM(11, aan);
    tlc.write();

    delay(2); /*EoL*/

    digitalWrite(0, LOW); /*Line 2*/
    digitalWrite(2, HIGH);
    delay(2); /*EoL*/

    digitalWrite(2, LOW); /*Line 3*/
    digitalWrite(14, HIGH);
    tlc.setPWM(8, aan);
    tlc.setPWM(9, aan);
    tlc.setPWM(12, aan);
    tlc.setPWM(13, aan);
    tlc.write();
    delay(2); /*EoL*/

    digitalWrite(14, LOW); /*Line 4*/
    digitalWrite(12, HIGH);
    delay(2); /*EoL*/

    break;
  }
}

/*----END EYE STYLES (static)----*/


/*------ANIMATIONS------*/
void scan() {
  cut();
  mode = SCAN;
  scanButton = 1;

  setRows(1, 1, 1, 1, 1, 1);

  for (int i = 30; i >= 1; i--) {
    tlc.setPWM(cols[i], aan);
    tlc.setPWM(cols[i - 1], aan);
    tlc.write();
    delay(40);
    tlc.setPWM(cols[i], uit);
    tlc.setPWM(cols[i - 1], uit);
    tlc.write();
  }
  for (int i = 0; i <= 29; i++) {
    tlc.setPWM(cols[i], aan);
    tlc.setPWM(cols[i + 1], aan);
    tlc.write();
    delay(40);
    tlc.setPWM(cols[i], uit);
    tlc.setPWM(cols[i + 1], uit);
    tlc.write();
  }
}

void halfScan() {
  cut();
  mode = HALF_SCAN;
  scanButton = 1;

  setRows(1, 1, 1, 1, 1, 1);

  for (int i = 30; i >= 1; i--) {
    tlc.setPWM(cols[i], aan);
    tlc.setPWM(cols[i - 1], aan);
    tlc.write();
    delay(60);
    tlc.setPWM(cols[i], uit);
    tlc.setPWM(cols[i - 1], uit);
    tlc.write();
  }
}

String textInputFunction() {
  cut();
  mode = SHOW_TEXT;
  textButton = 1;

  byte element;

  String output = text_input(textInput);


/*-----------THE SHITSHOW-----------*/
  for (int row = 0; row < 5; row++) {
    cut();
    for (int col = 0; col < 5; col++) {
      element = getMatrixLetter(row, col);
      Serial.print(element);
      Serial.print(" ");

      if(element == 1){
        tlc.setPWM(cols[col + 10], aan);
      }


    }
    if(element == 1){
        digitalWrite(rows[row], 1);
      }
    tlc.write();
    delay(5);

    Serial.println();
  }
/*-----------THE SHITSHOW-----------*/


  Serial.println(output);
  //return output;
}
/*----END ANIMATIONS----*/


/*----ESSENTIAL FUNCTIONS----*/
void setRows(bool x, bool y, bool z, bool a, bool b, bool c) {  //write similar function for rows??
  digitalWrite(rows[0], x);                                     /*1*/
  digitalWrite(rows[1], y);                                     /*2*/
  digitalWrite(rows[2], z);                                     /*3*/
  digitalWrite(rows[3], a);                                     /*4*/
  digitalWrite(rows[4], b);                                     /*5*/
  digitalWrite(rows[5], c);                                     /*6*/
}

void cut() {
  scanButton = 0;

  setRows(0, 0, 0, 0, 0, 0);

  for (int i = 0; i <= 30; i++) {
    tlc.setPWM(cols[i], uit);
    tlc.write();
  }
}

void burst() {
  setRows(1, 1, 1, 1, 1, 1);

  for (int i = 0; i <= 30; i++) {
    tlc.setPWM(cols[i], aan);
    tlc.write();
  }
}
/*----END ESSENTIAL FUNCTIONS----*/


/*------SERVER COMMANDS------*/
void handle_OnConnect() {
  server.send(200, "text/html", SendHTML(powerButton, eyeStyleButton, scanButton, textInput));
}

void handle_powerOff() {
  powerOff();
  server.send(200, "text/html", SendHTML(powerButton, eyeStyleButton, scanButton, textInput));
}

void handle_eyes() {
  eyes(eyeStyleButton);
  server.send(200, "text/html", SendHTML(powerButton, eyeStyleButton, scanButton, textInput));
}

void handle_eyes2() {
  eyes2(eyeStyleButton);
  server.send(200, "text/html", SendHTML(powerButton, eyeStyleButton, scanButton, textInput));
}

void handle_cut() {
  cut();
  server.send(200, "text/html", SendHTML(powerButton, eyeStyleButton, scanButton, textInput));
}

void handle_scan() {
  scan();
  server.send(200, "text/html", SendHTML(powerButton, eyeStyleButton, scanButton, textInput));
}

void handle_halfScan() {
  halfScan();
  server.send(200, "text/html", SendHTML(powerButton, eyeStyleButton, scanButton, textInput));
}

void handle_text() {

  if (server.hasArg("textValue")) {
    textInput = server.arg("textValue");
    textInputFunction();
  }
  server.send(200, "text/html", SendHTML(powerButton, eyeStyleButton, scanButton, textInput));
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

/*------END SERVER COMMANDS------*/

String SendHTML(uint8_t powerState, bool eyeStyleButtonState, uint8_t scanMode, String textInputValue) {  //store last value and compare?
  String ptr = "<!DOCTYPE html> <html>\n"
               "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n"
               "<title>Visor Controls</title>\n"
               "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n"
               "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n"
               ".button {display: inline-block;width: 170px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px 5px 35px 5px;cursor: pointer;border-radius: 4px;}\n"
               ".button-on {background-color: #1abc9c;}\n"
               ".button-on:active {background-color: #16a085;}\n"
               ".button-off {background-color: #34495e;}\n"
               ".button-off:active {background-color: #2c3e50;}\n"
               "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n"
               "input[type=text] {width: 200px;padding: 12px 20px;margin: 8px 0;box-sizing: border-box;border: 2px solid #ccc;border-radius: 4px;background-color: #f8f8f8;}\n"
               "</style></head>\n<body>\n<h1>Visor Controls</h1>\n<h3>Work in progress</h3>\n";


  if (powerState) {
    ptr += "<p>Visor Status: ON</p><a class=\"button button-off\" href=\"/powerOff\">Turn OFF</a>\n";
  } else {
    ptr += "<p>Visor Status: OFF</p><a class=\"button button-off\" href=\"/\">HW Reset</a>\n";
  }

  if (eyeStyleButtonState) {
    ptr += "<p>Eye style: 2</p><a class=\"button button-off\" href=\"/eyes=" + String(eyeStyleButtonState) + "\">1</a>\n";
    eyeStyleButton = 0;

  } else {
    ptr += "<p>Eye style: 1</p><a class=\"button button-on\" href=\"/eyes=" + String(eyeStyleButtonState) + "\">2</a>\n";
    eyeStyleButton = 1;
  }


  if (scanMode && mode == HALF_SCAN) {
    ptr += "<p>Scan mode: FULL</p><a class=\"button button-off\" href=\"/scan\">HALF</a>\n";
  } else {
    ptr += "<p>Scan mode: HALF</p><a class=\"button button-on\" href=\"/halfScan\">FULL</a>\n";
  }

  if (mode == SHOW_TEXT) {
    ptr += "<p>Display text:</p><input type=\"text\" name=\"textValue\" value=\"" + String(textInputValue) + "\"></input><p><a class=\"button button-off\" href=\"/#\">Turn OFF</a>\n";
    mode = BLANK;
  } else {
    ptr += "<p>Display text:</p><form action=\"/updateText\" method=\"GET\"><input type=\"text\" name=\"textValue\" value=\"" + String(textInputValue) + "\"></input><p><button class=\"button button-off\" type=\"submit\">Update</button></form></p>";
    mode = SHOW_TEXT;
  }



  ptr += "</body>\n</html>\n";
  return ptr;
}
  ptr += "</body>\n</html>\n";
  return ptr;
}
