#include <Arduino.h>

#define RELAY1 3
#define RELAY2 4

void setup() {
  Serial.begin(9600);
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  digitalWrite(BUTTON, HIGH);
}

void loop() {
  digitalWrite(RELAY1, HIGH);
  delay(1000);
  digitalWrite(RELAY2, HIGH);
  delay(1000);
  digitalWrite(RELAY1, LOW);
  delay(1000);
  digitalWrite(RELAY2, LOW);
  delay(1000);
}