#include <Arduino.h>

#define LED_RED 4
#define LED_YELLOW 5
#define LED_GREEN 8

#define BUTTON_1 3
#define BUTTON_2 6
#define BUTTON_3 7

void setup() {
  pinMode(BUTTON_1, INPUT);
  pinMode(BUTTON_2, INPUT);
  pinMode(BUTTON_3, INPUT);

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
}

void loop() {
  if (digitalRead(BUTTON_1)) digitalWrite(LED_RED, HIGH);
  else digitalWrite(LED_RED, LOW);

  if (digitalRead(BUTTON_2)) digitalWrite(LED_YELLOW, HIGH);
  else digitalWrite(LED_YELLOW, LOW);

  if (digitalRead(BUTTON_3)) digitalWrite(LED_GREEN, HIGH);
  else digitalWrite(LED_GREEN, LOW);
}