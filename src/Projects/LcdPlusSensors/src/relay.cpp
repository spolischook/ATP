#include <Arduino.h>
#include <string.h>
#include <nRF24L01.h>
#include <RF24.h>

bool relayState[2];
#define RELAY1 3
#define RELAY2 4

uint8_t LCD_READ_PIPE=1;
#define DIRR_LCD_RALAY 00003
#define DIRR_RALAY_LCD 01003

RF24 radio(9, 10); // CE, CSN

void setupRadioAddresses(RF24 radio) {
  radio.openReadingPipe(LCD_READ_PIPE, DIRR_LCD_RALAY);
  radio.openWritingPipe(DIRR_RALAY_LCD);
}
void setupRadio(RF24 radio) {
  radio.begin();
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  setupRadioAddresses(radio);
  radio.setRetries(15, 15);
  radio.setChannel(120);
  radio.startListening();
}
void getRelayStatus(RF24 radio) {
  radio.stopListening();
  radio.openWritingPipe(DIRR_RALAY_LCD);
  radio.write("GET", 3);
  radio.startListening();
}

// ----------------
// Rady, stady, go!
// ----------------

void setup() {
  Serial.begin(9600);
  setupRadio(radio);
  getRelayStatus(radio);
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  digitalWrite(RELAY1, LOW);
  digitalWrite(RELAY2, LOW);
}
void loop() {
  if (radio.available()) {
    char message[8];
    radio.read(message, sizeof(message));

    String method = String(message).substring(0, 3);

    if (method.equals("COM")) {
      String State = String(message).substring(3, 5);
      relayState[0] = State.substring(0, 1).equals("1");
      relayState[1] = State.substring(1, 2).equals("1");

      if (digitalRead(RELAY1) != relayState[0]) digitalWrite(RELAY1, relayState[0]);
      if (digitalRead(RELAY2) != relayState[1]) digitalWrite(RELAY2, relayState[1]);
    }

    if (method.equals("GET")) {
      radio.stopListening();
      delay(30);
      char com[5];
      sprintf(com, "COM%.1d%.1d", relayState[0], relayState[1]);
      bool result = radio.write(com, 5);
      Serial.print("Result: ");Serial.println(result);
      radio.startListening();
      delay(30);
    }
  }
}
