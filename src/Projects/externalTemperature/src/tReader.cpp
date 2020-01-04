#include <Arduino.h>
#include <Wire.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <string.h>
#include <LiquidCrystal.h>

RF24 radio(9, 10); // CE, CSN
const byte addresses[][6] = {"00001", "00002"};
LiquidCrystal lcd(8,6,5,4,3,2,7,NEGATIVE);

int BUTTON1 = A3;
int BUTTON2 = A2;
int BUTTON3 = A1;
int BUTTON4 = A0;


byte buttonState[20];
bool isButtonPressed(int button) {
    bool isHigh = analogRead(button) > 500;
    if (isHigh && !buttonState[button]) {
        buttonState[button] = true;
        return true;
    }
    if (!isHigh && buttonState[button]) {
        buttonState[button] = false;
    }

    return false;
}

void setup() {
    Serial.begin(9600);

    radio.begin();
    radio.setPALevel(RF24_PA_MIN);
    radio.setDataRate(RF24_250KBPS);
    radio.openWritingPipe(addresses[1]); // 00001
    radio.openReadingPipe(1, addresses[0]); // 00002
    radio.startListening();
    radio.setRetries(15, 15);
    radio.setChannel(120);

    Serial.print("Is Chip connected: "); Serial.println(radio.isChipConnected());
  
    buttonState[BUTTON1] = false;
    buttonState[BUTTON2] = false;
    buttonState[BUTTON3] = false;
    buttonState[BUTTON4] = false;
  
  lcd.begin(16, 2);
  lcd.on();

  lcd.print("<<< Waiting >>>");
}

void loop() {
    if (radio.available()) {
        char text[9] = "";
        radio.read(&text, sizeof(text));
        lcd.clear();
        lcd.print("Temp: "); lcd.print(text);
    }

    if (isButtonPressed(BUTTON4)) {
        radio.stopListening();
        delay(100);
        bool result = radio.write("get", 3);
        Serial.print("Result - "); Serial.println(result);

        radio.startListening();
    }
}
