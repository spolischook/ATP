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

bool buttonState = false;

void makeNoise(bool onOff) {

}

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON, INPUT);
}

void loop() {
  analogValue = analogRead(lightSensorPin);
  // tone(ZOOM, analogValue*10, 200);
  if (BuzzTone > MAX_FREQ) toneDir = -1;
  if (BuzzTone < MIN_FREQ) toneDir = 1;
  BuzzTone += 10*toneDir;
  if (isTone) tone(ZOOM, BuzzTone, 200);

  if (digitalRead(BUTTON) && !buttonState) {
    buttonState = true;
    isTone = !isTone;
  } 
  if (!digitalRead(BUTTON) && buttonState) {
    buttonState = false;
  }

}