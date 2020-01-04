#include <Arduino.h>
#include <string.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include <nRF24L01.h>
#include <RF24.h>

#define DIRR_LCD_TERMO 00001
#define DIRR_LCD_PHOTO 00002
#define DIRR_LCD_RALAY 00004
#define DIRR_TERMO_LCD 01101
#define DIRR_PHOTO_LCD 01102
#define DIRR_RALAY_LCD 01103

static const uint8_t BUTTON1 = A3;
static const uint8_t BUTTON2 = A2;
static const uint8_t BUTTON3 = A1;
static const uint8_t BUTTON4 = A0;

LiquidCrystal lcd(8,6,5,4,3,2,7,NEGATIVE);
RF24 radio(9, 10); // CE, CSN
unsigned long termoUpdateTime = 0;

struct data {
  char termo[4];
  char photo[4];
  char relay1[4];
  char relay2[4];
};
struct address {
  byte lcd[6];
  byte termo[6];
  byte photo[6];
  byte relay[6];
};
data dataState = {"n/a", "n/a", "n/a", "n/a"};
data dataLCD   = {"n/a", "n/a", "n/a", "n/a"};
address moduleAdresses = {"00001", "00002", "00003", "00004"};

void printDataToLcd(LiquidCrystal lcd, data data) {
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
bool isDataChanged(LiquidCrystal lcd, data dataState, data dataLCD) {
  if (strcmp(dataState.photo, dataLCD.photo) != 0) return true;
  if (strcmp(dataState.termo, dataLCD.termo) != 0) return true;
  return false;
}
void setupRadioAddresses(RF24 radio) {
  
  radio.openReadingPipe(1, DIRR_TERMO_LCD);
}
void setupRadio(RF24 radio) {
  radio.begin();
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  setupRadioAddresses(radio);
  radio.startListening();
  radio.setRetries(15, 15);
  radio.setChannel(120);
}

// ----------------
// Rady, stady, go!
// ----------------

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.on();
  printDataToLcd(lcd, dataState);
  setupRadio(radio);
  

  Serial.print("Is Chip connected: "); Serial.println(radio.isChipConnected());
}
void loop() {

  if (isDataChanged(lcd, dataState, dataLCD)) {
    printDataToLcd(lcd, dataState);
    dataLCD = dataState;
  }

  uint8_t n = 1;
  if (radio.available(&n)) {
    radio.read(dataState.termo, sizeof(dataState.termo));
    termoUpdateTime = millis();
  }
  if (5000 < millis()-termoUpdateTime) {
    sprintf(dataState.termo, "n/a");
  }


  // // radio.stopListening();
  // // char ping[5] = "ping";
  // // bool result = radio.write(&ping, 5);
  // // Serial.println(result);
  // // radio.startListening();

  // sprintf(dataState.photo, "555");
}