#include <Arduino.h>
#include <string.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include <nRF24L01.h>
#include <RF24.h>

LiquidCrystal lcd(8,6,5,4,3,2,7,NEGATIVE);
RF24 radio(9, 10); // CE, CSN

struct data {
  char termo[4];
  char photo[4];
  char relay1[4];
  char relay2[4];
};
data dataState = {"n/a", "n/a", "n/a", "n/a"};
data dataLCD   = {"n/a", "n/a", "n/a", "n/a"};
struct address {
  byte lcd[6];
  byte termo[6];
  byte photo[6];
  byte relay[6];
};

void printDataToLcd(data data, LiquidCrystal lcd) {
  char line[16];
  sprintf(line, "T:  %.4s  Ph: %.4s", data.termo, data.photo);
  lcd.clear();
  lcd.setCursor(0,0);
  Serial.println(line);
  lcd.print(line);

  sprintf(line, "R1: %.4s  R2: %.4s", data.relay1, data.relay2);
  // delay(1000);
  lcd.setCursor(0,1);
  lcd.print(line);
  // delay(1000);
}

bool isDataChanged(data dataState, data dataLCD) {
  if (strcmp(dataState.photo, dataLCD.photo) != 0) return true;
  return false;
}

void setup() {
  Serial.begin(9600);
  address address = {"00001", "00002", "00003", "00004"};
  lcd.begin(16, 2);
  lcd.on();
  printDataToLcd(dataState, lcd);

  radio.begin();
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(address.termo); // 00001
  radio.openReadingPipe(1, address.lcd); // 00002
  radio.startListening();
  radio.setRetries(15, 15);
  radio.setChannel(120);
}

void loop() {
  if (isDataChanged(dataState, dataLCD)) {
    printDataToLcd(dataState, lcd);
    dataLCD = dataState;
  }

  sprintf(dataState.photo, "555");
}