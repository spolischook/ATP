#include <Arduino.h>
#include <string.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include <nRF24L01.h>
#include <RF24.h>

uint8_t READ_CURRENT_PIPE;
uint8_t READ_TERMO_PIPE=1;
uint8_t READ_PHOTO_PIPE=2;
uint8_t READ_RELAY_PIPE=3;
#define DIR_LCD_TERMO 00001
#define DIR_LCD_PHOTO 00002
#define DIR_LCD_RELAY 00003
#define DIR_TERMO_LCD 01001
#define DIR_PHOTO_LCD 01002
#define DIR_RALAY_LCD 01003

static const uint8_t BUTTON1 = A3;
static const uint8_t BUTTON2 = A2;
static const uint8_t BUTTON3 = A1;
static const uint8_t BUTTON4 = A0;
bool buttonState[4] = {0, 0, 0, 0};
unsigned long buttonUpdateTime[4] = {0, 0, 0, 0};

bool relayState[2] = {false, false};

LiquidCrystal lcd(8,6,5,4,3,2,7,NEGATIVE);
RF24 radio(9, 10); // CE, CSN

unsigned long termoUpdateTime = 0;
unsigned long photoUpdateTime = 0;
unsigned long relayUpdateTime = 0;
unsigned long relayPingTime = 0;

struct data {
  char termo[4];
  char photo[4];
  char relay1[4];
  char relay2[4];
};
data dataState = {"n/a", "n/a", "n/a", "n/a"};
data dataLCD   = {"n/a", "n/a", "n/a", "n/a"};

void printDataToLcd(LiquidCrystal lcd, data data) {
  char line[16];
  sprintf(line, "T:  %.4s  Ph: %.4s", data.termo, data.photo);
  lcd.clear();
  lcd.setCursor(0,0);
  // Serial.println(line);
  lcd.print(line);

  sprintf(line, "R1: %.4s  R2: %.4s", data.relay1, data.relay2);
  lcd.setCursor(0,1);
  lcd.print(line);
}
bool isDataChanged(LiquidCrystal lcd, data dataState, data dataLCD) {
  if (strcmp(dataState.photo, dataLCD.photo) != 0) return true;
  if (strcmp(dataState.termo, dataLCD.termo) != 0) return true;
  if (strcmp(dataState.relay1, dataLCD.relay1) != 0) return true;
  if (strcmp(dataState.relay2, dataLCD.relay2) != 0) return true;
  return false;
}
void setupRadioAddresses(RF24 radio) {
  radio.openReadingPipe(READ_TERMO_PIPE, DIR_TERMO_LCD);
  radio.openReadingPipe(READ_PHOTO_PIPE, DIR_PHOTO_LCD);
  radio.openReadingPipe(READ_RELAY_PIPE, DIR_RALAY_LCD);
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
bool isButtonTurnedOn(uint8_t button) {
  if (millis() - buttonUpdateTime[button-A0] < 100) return false;
  bool isPushed = analogRead(button) > 900;
  
  if (isPushed && !buttonState[button-A0]) {
    buttonState[button-A0] = true;
    buttonUpdateTime[button-A0] = millis();

    return true;
  }

  return false;
}
bool isButtonTurnedOff(uint8_t button) {
  if (millis() - buttonUpdateTime[button-A0] < 100) return false;
  bool isPushed = analogRead(button) > 500;

  if (!isPushed && buttonState[button-A0]) {
      buttonState[button-A0] = false;
      buttonUpdateTime[button-A0] = millis();
      return true;
  }

  return false;
}
void sendComToRelay(RF24 radio, bool * relayState) {
  radio.stopListening();
  radio.openWritingPipe(DIR_LCD_RELAY);
  char com[7];
  sprintf(com, "COM%.1d%.1d", relayState[0], relayState[1]);
  radio.write(com, 7);
  radio.startListening();
}
void getRelayStatus(RF24 radio) {
  radio.stopListening();
  radio.openWritingPipe(DIR_LCD_RELAY);
  radio.write("GET", 3);
  radio.startListening();
}
void handleTermoPipe() {
  radio.read(dataState.termo, sizeof(dataState.termo));
  termoUpdateTime = millis();
}
void handleRelayPipe() {
  char message[8];
  radio.read(message, sizeof(message));
  String method = String(message).substring(0, 3);
  
  if (method.equals("COM")) {
    String State = String(message).substring(3, 5);
    relayState[0] = State.substring(0, 1).equals("1");
    relayState[1] = State.substring(1, 2).equals("1");
    if (relayState[0]) sprintf(dataState.relay1, "ON "); else sprintf(dataState.relay1, "OFF");
    if (relayState[1]) sprintf(dataState.relay2, "ON "); else sprintf(dataState.relay2, "OFF");
    relayUpdateTime = millis();
  }
  if (method.equals("GET")) {
    radio.stopListening();
    radio.openWritingPipe(DIR_LCD_RELAY);
    char com[5];
    sprintf(com, "COM%.1d%.1d", relayState[0], relayState[1]);
    bool result = radio.write(com, 5);
    radio.startListening();
  }
}
void handlePhotoPipe() {
  radio.read(dataState.photo, 4);
  photoUpdateTime = millis();
}
void updateStates() {
  if (5000 < millis()-termoUpdateTime) {
    sprintf(dataState.termo, "n/a");
  }
  if (5000 < millis()-photoUpdateTime) {
    sprintf(dataState.photo, "n/a");
  }
  if (1100 < millis()-relayPingTime) {
    getRelayStatus(radio);
    relayPingTime = millis();
  }
  if (8000 < millis()-relayUpdateTime) {
    sprintf(dataState.relay1, "n/a");
    sprintf(dataState.relay2, "n/a");
  }
}
#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__
 
int freeMemory() {
  char top;
#ifdef __arm__
  return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  return &top - __brkval;
#else  // __arm__
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}

// ----------------
// Rady, stady, go!
// ----------------

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.on();
  printDataToLcd(lcd, dataState);
  setupRadio(radio);
  getRelayStatus(radio);
  Serial.print("Is Chip connected: "); Serial.println(radio.isChipConnected());
}
void loop() {
  if (isDataChanged(lcd, dataState, dataLCD)) {
    printDataToLcd(lcd, dataState);
    dataLCD = dataState;
  }

  if (radio.available(&READ_CURRENT_PIPE)) {
    if (READ_CURRENT_PIPE == READ_TERMO_PIPE) handleTermoPipe();
    if (READ_CURRENT_PIPE == READ_RELAY_PIPE) handleRelayPipe();
    if (READ_CURRENT_PIPE == READ_PHOTO_PIPE) handlePhotoPipe();
  }

  updateStates();

  if (isButtonTurnedOn(BUTTON1)) {
    Serial.print("freeMemory()=");Serial.println(freeMemory());
  }
  if (isButtonTurnedOff(BUTTON1)) {}

  if (isButtonTurnedOn(BUTTON2)) {
  }
  if (isButtonTurnedOff(BUTTON2)) {}

  if (isButtonTurnedOn(BUTTON3)) {
    Serial.println("Button3 pressed");
    relayState[0] = !relayState[0];
    sendComToRelay(radio, relayState);
  }
  if (isButtonTurnedOff(BUTTON3)) {}

  if (isButtonTurnedOn(BUTTON4)) {
    Serial.println("Button4 pressed");
    relayState[1] = !relayState[1];
    sendComToRelay(radio, relayState);
  }
  if (isButtonTurnedOff(BUTTON4)) {}
}
