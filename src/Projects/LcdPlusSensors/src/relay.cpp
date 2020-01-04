#include <Arduino.h>
#include <nRF24L01.h>
#include <RF24.h>

#define RELAY1 3
#define RELAY2 4

#define LCD_ADDRESS 

struct relayState
{
  bool first;
  bool second;
};
relayState state = {0, 0};

void setup() {
  Serial.begin(9600);
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  digitalWrite(RELAY1, LOW);
  digitalWrite(RELAY2, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
}