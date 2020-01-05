#include <Arduino.h>
#include <string.h>
#include <nRF24L01.h>
#include <RF24.h>

#define DIRR_LCD_PHOTO 00002
#define DIRR_PHOTO_LCD 01002

static const uint8_t ZOOM = 3;
static const uint8_t BUTTON = 4;
static const uint8_t lightSensorPin = A0;
RF24 radio(9, 10); // CE, CSN
int light;
unsigned long lastUpdate = 0;

void setupRadioAddresses(RF24 radio) {
  radio.openReadingPipe(1, DIRR_LCD_PHOTO);
  radio.openWritingPipe(DIRR_PHOTO_LCD);
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
bool isUpdateNeeded() {
  // if (light != analogRead(lightSensorPin)) return true;
  if (800 < lastUpdate-millis())  return true;
  return false;
}

// ----------------
// Rady, stady, go!
// ----------------

void setup() {
  Serial.begin(9600);
  setupRadio(radio);
}

void loop() {
  if (isUpdateNeeded()) {
    light = analogRead(lightSensorPin);
    radio.stopListening();
    delay(100);
    char temp[4] = "";
    sprintf(temp, "%.3d", light);
    bool result = radio.write(temp, 4);
    Serial.println(result);
    lastUpdate = millis();
    radio.startListening();
  }
}