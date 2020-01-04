#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal.h>

static const uint8_t BUTTON1 = A3;
static const uint8_t BUTTON2 = A2;
static const uint8_t BUTTON3 = A1;
static const uint8_t BUTTON4 = A0;
LiquidCrystal lcd(8,6,5,4,3,2,7,NEGATIVE);

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.on();
  lcd.print("<<< Waiting >>>");
  // delay(1000);
  // lcd.off();
  // delay(1000);
  // lcd.clear();
  // lcd.on();
  // lcd.print("I'm allive!");
  // while (true)
  // {
  //   delay(400);
  //   lcd.off();
  //   delay(400);
  //   lcd.on();
  // }

}

void loop() {
  if (analogRead(BUTTON1) > 500) {
    lcd.clear();  
    lcd.print("Button 1 pressed");
  } else if (analogRead(BUTTON2) > 500) {
    lcd.clear();
    lcd.print("Button 2 pressed");
  } else if (analogRead(BUTTON3) > 500) {
    lcd.clear();
    lcd.print("Button 3 pressed");
  } else if (analogRead(BUTTON4) > 500) {
    lcd.clear();
    lcd.print("Button 4 pressed");
  } else {
    lcd.clear();
    lcd.print("<<< Waiting >>>");
  }
  // Serial.print("BUTTON_1: "); Serial.println(analogRead(BUTTON1));
  // Serial.print("BUTTON_2: "); Serial.println(analogRead(BUTTON2));
  delay(100);
}