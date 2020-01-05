#include <Arduino.h>
#include <nRF24L01.h>
#include <RF24.h>

#define LED_RED 3
#define LED_YELLOW 4
#define LED_GREEN 5
#define LED_BLUE 6
#define LED_TRANSPARRENT 7
#define BUTTON 8
#define TERMISTOR 0

#define DIRR_TERMO_LCD 01001
#define DIRR_LCD_TERMO 00001

RF24 radio(9, 10); // CE, CSN
int tSensorPin = A0;
int t;
unsigned long lastUpdate = 0;

struct address {
  byte lcd[6];
  byte termo[6];
  byte photo[6];
  byte relay[6];
};
void setupRadioAddresses(RF24 radio) {
  radio.openReadingPipe(1, DIRR_LCD_TERMO);
  radio.openWritingPipe(DIRR_TERMO_LCD);
}
void setupRadio(RF24 radio) {
  radio.begin();
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  setupRadioAddresses(radio);
  radio.startListening();
  radio.setRetries(15, 15);
  radio.setChannel(120);
}
bool isUpdateNeeded() {
  if (t != analogRead(tSensorPin)) return true;
  if (1000 < lastUpdate-millis())  return true;
  return false;
}

// ----------------
// Rady, stady, go!
// ----------------

void setup() {
  Serial.begin(9600);
  
  setupRadio(radio);

  Serial.print("Is Chip connected: "); Serial.println(radio.isChipConnected());
}
void loop() {
  if (isUpdateNeeded()) {
    t = analogRead(tSensorPin);
    radio.stopListening();
    delay(100);
    char temp[3] = "";
    sprintf(temp, "%.3d", t);
    bool result = radio.write(temp, 5);
    Serial.println(result);
    lastUpdate = millis();
    radio.startListening();
  }
}
