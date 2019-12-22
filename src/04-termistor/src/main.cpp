#include <Arduino.h>

#define LED_RED 3
#define LED_YELLOW 4
#define LED_GREEN 5
#define LED_BLUE 6
#define LED_TRANSPARRENT 7
#define BUTTON 8
#define TERMISTOR 0

long int ledTimer = 0;
bool buttonState = false;
int tSensorPin = A0;

void turnLightOff() {
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_BLUE, LOW);
  digitalWrite(LED_TRANSPARRENT, LOW);
}

void turnLightOn() {
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_YELLOW, HIGH);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_BLUE, HIGH);
  digitalWrite(LED_TRANSPARRENT, HIGH);
}

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON, INPUT);

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_TRANSPARRENT, OUTPUT);
  
  turnLightOff();
}

void loop() {
  ledTimer++;
  if (ledTimer > 1000000) {
    turnLightOff();
    ledTimer = 0;
  }
  if (digitalRead(BUTTON)) {
    buttonState = true;
    turnLightOn();
  } else if(buttonState) {
    buttonState = false;
    turnLightOff();
    ledTimer = 100000;
  }

  if (ledTimer == 100000) digitalWrite(LED_RED, HIGH);
  if (ledTimer == 300000) digitalWrite(LED_YELLOW, HIGH);
  if (ledTimer == 500000) digitalWrite(LED_GREEN, HIGH);
  if (ledTimer == 700000) digitalWrite(LED_BLUE, HIGH);
  if (ledTimer == 900000) digitalWrite(LED_TRANSPARRENT, HIGH);
  // Serial.println(analogRead(tSensorPin));
}