#include <Arduino.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <string.h>

// R => temp
// 320 => 3,5
// 250 => 1

RF24 radio(9, 10); // CE, CSN
const byte addresses[][6] = {"00001", "00002"};
int tSensorPin = A0;

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(addresses[0]); // 00001
  radio.openReadingPipe(1, addresses[1]); // 00002
  radio.startListening();
  radio.setRetries(15, 15);
  radio.setChannel(120);

  Serial.print("Is Chip connected: "); Serial.println(radio.isChipConnected());
}

void loop() {
  if (radio.available()) {
    char text[4] = "";
    radio.read(&text, sizeof(text));
    Serial.println(text);

    if (strcmp("get", text) == 0) {
      radio.stopListening();
      delay(100);
      int t = analogRead(tSensorPin);
      char temp[3] = "";
      sprintf(temp, "%.3d", t);
      bool result = radio.write(temp, 5);
      Serial.print("Temp - "); Serial.println(temp);
      Serial.print("Result - "); Serial.println(result);

      radio.startListening();
    }
  }
}