#include <Arduino.h>

int ZOOM = 3;
int BUTTON = 4;
int MIN_FREQ = 60;
int MAX_FREQ = 2000;

int freq;
int lightSensorPin = A0;
int analogValue = 0;
int BuzzTone = MIN_FREQ;
int toneDir = 1;
bool isTone = 1;

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON, INPUT);
}

void loop() {
  analogValue = analogRead(lightSensorPin);
  // tone(ZOOM, analogValue*10, 200);
  if (BuzzTone > MAX_FREQ) toneDir = -1;
  if (BuzzTone < MIN_FREQ)   toneDir = 1;
  BuzzTone += 10*toneDir;
  if (isTone) tone(ZOOM, BuzzTone, 200);
  
  if (digitalRead(BUTTON)) {
    isTone = !isTone;
  }
  // if (analogValue > 650) {
  //   noTone(ZOOM);
  //   digitalWrite(GREEN_LED, LOW);
  //   digitalWrite(BLUE_LED, LOW);
  //   digitalWrite(RED_LED, LOW);
  // }
  // if (analogValue < 650) {
  //   int freq = 10000 - (analogValue -350) * 10000/250;
  //   Serial.println(freq);
  //   tone(ZOOM, freq, 500);
  // }
  // if (analogValue < 580) {
  //   digitalWrite(GREEN_LED, HIGH);
  //   digitalWrite(BLUE_LED, LOW);
  //   digitalWrite(RED_LED, LOW);
  // }
  // if (analogValue < 510) {
  //   digitalWrite(GREEN_LED, HIGH);
  //   digitalWrite(BLUE_LED, HIGH);
  //   digitalWrite(RED_LED, LOW);
  // }
  // if (analogValue < 400) {
  //   digitalWrite(GREEN_LED, HIGH);
  //   digitalWrite(BLUE_LED, HIGH);
  //   digitalWrite(RED_LED, HIGH);
  // }
  Serial.println(analogValue);
  delay(10);
}