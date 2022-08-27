//All columns get connected to v+ on pins 0-30
//All rows get connected to GND on pins 31-36
//Pulling an output high pulls the ground low.

#include "Adafruit_TLC5947.h"

// How many boards do you have chained?
#define NUM_TLC5974 2

#define data D0
#define clock D1
#define latch D2
#define oe  -1  // set to -1 to not use the enable pin (its optional)

int cols[/*31*/] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30};
//int rows[6] = {9,10,2,3,4,8}; //{31,32,33,34,35,36};
#define uit 0
#define aan 4095

//int col[] = {0,2,1,X};
//int lijn2[] = {D4,D5,D6,4,8,11};

Adafruit_TLC5947 tlc = Adafruit_TLC5947(NUM_TLC5974, clock, data, latch);

void setup() {
  Serial.begin(9600);
  
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
}

void loop() {

  //imageTest();
  eyes3();
  //imageTest2();
  //daftPunk2();
  /*
    powerOn();
    delay(3000);

    eyes3();
    delay(3000);

    eyes2();
    delay(3000);

    visor();
    delay(3000);

    powerOff();
    delay(3000);*/
}

void powerOn() {
  digitalWrite(12, HIGH);
  digitalWrite(13, HIGH);
  digitalWrite(14, HIGH);
  digitalWrite(15, HIGH);
  digitalWrite(2, HIGH);
  digitalWrite(0, HIGH);

  for (int i = 30; i >= 0; i--) {
    tlc.setPWM(cols[i], aan);
    tlc.write();
    delay(40);
  }
}

void powerOff() {
  burst();
  digitalWrite(12, HIGH);
  digitalWrite(13, HIGH);
  digitalWrite(14, HIGH);
  digitalWrite(15, HIGH);
  digitalWrite(2, HIGH);
  digitalWrite(0, HIGH);

  for (int i = 0; i <= 30; i++) {
    tlc.setPWM(cols[i], uit);
    tlc.write();
    delay(40);
  }
}

void eyes() {
  cut();
  digitalWrite(0, LOW);/*1*/
  digitalWrite(2, LOW);/*2*/
  digitalWrite(14, HIGH);/*3*/
  digitalWrite(12, HIGH);/*4*/
  digitalWrite(13, LOW);/*5*/
  digitalWrite(15, LOW);/*6*/
  tlc.setPWM(10, aan);
  tlc.setPWM(11, aan);
  tlc.setPWM(20, aan);
  tlc.setPWM(21, aan);
  tlc.write();
}

void eyes2() {
  cut();
  digitalWrite(0, LOW);/*1*/
  digitalWrite(2, LOW);/*2*/
  digitalWrite(14, HIGH);/*3*/
  digitalWrite(12, HIGH);/*4*/
  digitalWrite(13, LOW);/*5*/
  digitalWrite(15, LOW);/*6*/
  tlc.setPWM(9, aan);
  tlc.setPWM(10, aan);
  tlc.setPWM(11, aan);
  tlc.setPWM(12, aan);
  tlc.setPWM(19, aan);
  tlc.setPWM(20, aan);
  tlc.setPWM(21, aan);
  tlc.setPWM(22, aan);
  tlc.write();
}

void eyes3() {
  cut();
  digitalWrite(0, LOW);/*1*/
  digitalWrite(2, LOW);/*2*/
  digitalWrite(14, HIGH);/*3*/
  digitalWrite(12, HIGH);/*4*/
  digitalWrite(13, LOW);/*5*/
  digitalWrite(15, LOW);/*6*/
  tlc.setPWM(9, aan);
  tlc.setPWM(10, aan);
  tlc.setPWM(11, aan);
  tlc.setPWM(12, aan);
  tlc.setPWM(19, aan);
  tlc.setPWM(20, aan);
  tlc.setPWM(21, aan);
  tlc.setPWM(22, aan);
  tlc.write();
}

void visor() {
  cut();
  digitalWrite(0, LOW);/*1*/
  digitalWrite(2, HIGH);/*2*/
  digitalWrite(14, HIGH);/*3*/
  digitalWrite(12, HIGH);/*4*/
  digitalWrite(13, HIGH);/*5*/
  digitalWrite(15, LOW);/*6*/
  for (int i = 30; i >= 0; i--) {
    tlc.setPWM(cols[i], aan);
  }
  tlc.write();
}

void burst() {
  digitalWrite(0, HIGH);/*1*/
  digitalWrite(2, HIGH);/*2*/
  digitalWrite(14, HIGH);/*3*/
  digitalWrite(12, HIGH);/*4*/
  digitalWrite(13, HIGH);/*5*/
  digitalWrite(15, HIGH);/*6*/

  for (int i = 0; i <= 30; i++) {
    tlc.setPWM(cols[i], aan);
    tlc.write();
  }
}

void cut() {
  digitalWrite(0, LOW);/*1*/
  digitalWrite(2, LOW);/*2*/
  digitalWrite(14, LOW);/*3*/
  digitalWrite(12, LOW);/*4*/
  digitalWrite(13, LOW);/*5*/
  digitalWrite(15, LOW);/*6*/

  for (int i = 0; i <= 30; i++) {
    tlc.setPWM(cols[i], uit);
    tlc.write();
  }
}

void imageTest() {

  int cols1[] = {13   , 15, 16, 17};
  int cols2[] = {13   , 15};
  int cols3[] = {13, 14, 15, 16, 17};
  int cols4[] = {      15,   17};
  int cols5[] = {13, 14, 15,   17};

  //////////////////////////////////////////
  for (int i = 0; i < (sizeof(cols5) / sizeof(cols5[0])); i++) {
    tlc.setPWM(cols5[i], uit);
  }
  tlc.write();
  for (int i = 0; i < (sizeof(cols5) / sizeof(cols5[0])); i++) {
    tlc.setPWM(cols1[i], aan);
  }
  digitalWrite(0, HIGH);
  digitalWrite(13, LOW);
  tlc.write();
  //////////////////////////////////////////
  for (int i = 0; i < (sizeof(cols5) / sizeof(cols5[0])); i++) {
    tlc.setPWM(cols1[i], uit);
  }
  tlc.write();
  for (int i = 0; i < (sizeof(cols2) / sizeof(cols2[0])); i++) {
    tlc.setPWM(cols2[i], aan);
  }
  digitalWrite(0, LOW);
  digitalWrite(2, HIGH);
  tlc.write();
  //////////////////////////////////////////
  for (int i = 0; i < (sizeof(cols2) / sizeof(cols2[0])); i++) {
    tlc.setPWM(cols2[i], uit);
  }
  tlc.write();
  for (int i = 0; i < (sizeof(cols3) / sizeof(cols3[0])); i++) {
    tlc.setPWM(cols3[i], aan);
  }
  digitalWrite(2, LOW);
  digitalWrite(14, HIGH);
  tlc.write();
  //////////////////////////////////////////
  for (int i = 0; i < (sizeof(cols3) / sizeof(cols3[0])); i++) {
    tlc.setPWM(cols3[i], uit);
  }
  tlc.write();
  for (int i = 0; i < (sizeof(cols4) / sizeof(cols4[0])); i++) {
    tlc.setPWM(cols4[i], aan);
  }
  digitalWrite(14, LOW);
  digitalWrite(12, HIGH);
  tlc.write();
  //////////////////////////////////////////
  for (int i = 0; i < (sizeof(cols4) / sizeof(cols4[0])); i++) {
    tlc.setPWM(cols4[i], uit);
  }
  tlc.write();
  for (int i = 0; i < (sizeof(cols5) / sizeof(cols5[0])); i++) {
    tlc.setPWM(cols5[i], aan);
  }
  digitalWrite(12, LOW);
  digitalWrite(13, HIGH);
  tlc.write();
  //////////////////////////////////////////

  //int cols[31] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,  X, 14, X, X, X, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30};
  //int cols[31] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,  X, 14, X, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30};
  //int cols[31] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,  X,  X, X,  X, X, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30};
  //int cols[31] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, X, 16, X, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30};
  //int cols[31] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,  X,  X, X, 16, X, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30};
  //int cols[31] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 16, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30};
}

void imageTest2() {

  int writes[] = {0, 2, 14, 12, 13, 15};

  int image[5][5] = { {13, 99, 15, 16, 17}, {13, 99, 15, 99, 99}, {13, 14, 15, 16, 17}, {99, 99, 15, 99, 17}, {13, 14, 15, 99, 17} };

  for (int i = 0; i <= 5; i++) {

    digitalWrite(writes[i], HIGH);

    for (int x = 0; x < 5; x++) {

      for (int j = 0; j < 5; j++) {
        tlc.setPWM(image[x][j], aan);
      }
      tlc.write();

    }


    for (int x = 0; x < 5; x++) {

      for (int j = 0; j < 5; j++) {
        tlc.setPWM(image[x][j], uit);
      }
      tlc.write();

    }

    digitalWrite(writes[i], LOW);
    delay(100);
  }
}

void strobe() {
  int writes[] = {0, 2, 14, 12, 13, 15};

  for (int i = 0; i <= 30; i++) {
    tlc.setPWM(cols[i], aan);
    tlc.write();
  }

  for (int i = 0; i < 5; i++) {
    digitalWrite(writes[i], HIGH);
    digitalWrite(writes[i + 1], HIGH);
    delay(75);
    digitalWrite(writes[i], LOW);
    digitalWrite(writes[i + 1], LOW);
  }
}

void snow() { //WIP

  int writes[] = {0, 2, 14, 12, 13, 15};
  int ran = random(6);
  int ran2 = random(31);

  digitalWrite(writes[ran], HIGH);
  tlc.setPWM(cols[ran2], aan);
  tlc.write();
  tlc.setPWM(cols[ran2], uit);
  tlc.write();
  digitalWrite(writes[ran], LOW);
}


void daftPunk2() {
  for (int i = 0; i <= 30; i++) {
    tlc.setPWM(cols[i], uit);
    tlc.write();
    delay(40);
  }
  
  int writes[] = {0, 2, 14, 12, 13, 15};

  int daft[6][31] = {
    {99, 99, 99, 99, 99, 99, 99, 99, 99,  7,  8, 99, 99, 11, 12, 13, 99, 15, 16, 17, 99, 19, 20, 21, 22, 99, 99, 99, 99, 99, 99},
    {99, 99, 99, 99, 99, 99, 99, 99, 99,  7, 99,  9, 99, 11, 99, 13, 99, 15, 16, 99, 99, 99, 99, 21, 99, 99, 99, 99, 99, 99, 99},
    {99, 99, 99, 99, 99, 99, 99, 99, 99,  7, 99,  9, 99, 11, 12, 13, 99, 15, 16, 99, 99, 99, 99, 21, 99, 99, 99, 99, 99, 99, 99},
    {99, 99, 99, 99, 99, 99, 99, 99, 99,  7, 99,  9, 99, 11, 99, 13, 99, 15, 99, 99, 99, 99, 99, 21, 99, 99, 99, 99, 99, 99, 99},
    {99, 99, 99, 99, 99, 99, 99, 99, 99,  7, 99,  9, 99, 11, 99, 13, 99, 15, 99, 99, 99, 99, 99, 21, 99, 99, 99, 99, 99, 99, 99},
    {99, 99, 99, 99, 99, 99, 99, 99, 99,  7,  8, 99, 99, 11, 99, 13, 99, 15, 99, 99, 99, 99, 99, 21, 99, 99, 99, 99, 99, 99, 99}
  };

  int punk[6][31] = {
    {99, 99, 99, 99, 99, 99, 99, 99, 99,  7,  8,  9, 99, 11, 12, 13, 99, 15, 99, 99, 18, 99, 99, 21, 99, 23, 99, 99, 99, 99, 99},
    {99, 99, 99, 99, 99, 99, 99, 99, 99,  7, 99,  9, 99, 11, 99, 13, 99, 15, 16, 99, 18, 99, 99, 21, 22, 99, 99, 99, 99, 99, 99},
    {99, 99, 99, 99, 99, 99, 99, 99, 99,  7, 9,   9, 99, 11, 99, 13, 99, 15, 99, 17, 18, 99, 99, 21, 22, 99, 99, 99, 99, 99, 99},
    {99, 99, 99, 99, 99, 99, 99, 99, 99,  7, 99, 99, 99, 11, 99, 13, 99, 15, 99, 99, 18, 99, 99, 21, 99, 23, 99, 99, 99, 99, 99},
    {99, 99, 99, 99, 99, 99, 99, 99, 99,  7, 99, 99, 99, 11, 99, 13, 99, 15, 99, 99, 18, 99, 99, 21, 99, 23, 99, 99, 99, 99, 99},
    {99, 99, 99, 99, 99, 99, 99, 99, 99,  7, 99, 99, 99, 11, 12, 13, 99, 15, 99, 99, 18, 99, 99, 21, 99, 23, 99, 99, 99, 99, 99}
  };

    for (int q = 0; q <= 5; q++) {
      digitalWrite(writes[q], HIGH);
    }
      
    for (int i = 0; i <= 5; i++) {
      for (int j = 0; j < 31; j++) {
        tlc.setPWM(daft[i][j], aan);
      }
      tlc.write();
    }  
    delay(500);
    
    for (int x = 0; x < 5; x++) {
      for (int l = 0; l < 31; l++) {
        tlc.setPWM(daft[x][l], uit);
      }
      tlc.write();
    }
    delay(500);

    for (int p = 0; p <= 5; p++) {
      digitalWrite(writes[p], LOW);
    }
}  

void H() {
  int letters[6][31] = {
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30},
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30},
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30},
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30},
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30},
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30},
  };

  int daftPunk[][31] = {
    {0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0}
  };
}
