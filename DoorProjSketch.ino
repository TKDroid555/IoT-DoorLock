#include <Arduino.h>
#include <Wire.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include "door.h"
#include "puzzle.h"

// ESP32 DevKit pin map. Change these constants to match your board/wiring.
namespace Pins {
constexpr uint8_t lcdSda = 21;
constexpr uint8_t lcdScl = 22;
constexpr uint8_t relay = 18; // Relay channel input; change to your wiring
constexpr uint8_t buzzer = 19;
constexpr uint8_t manualButton = 5; // button to GND; internal pull-up enabled
constexpr byte rowPins[4] = {13, 14, 27, 26};
constexpr byte colPins[4] = {12, 33, 32, 23}; // 12 was 25 earlier but pin 25 broke or smth
}

const char keyMap[4][4] = {
  {'1', '2', '3', '*'}, // * = BS
  {'4', '5', '6', '#'}, // # = ENT
  {'7', '8', '9', 'D'}, // D = CLSD
  {'.', '0', '^', 'c'}
};
Keypad keypad = Keypad(makeKeymap(keyMap), const_cast<byte*>(Pins::rowPins),
                       const_cast<byte*>(Pins::colPins), 4, 4);
LiquidCrystal_I2C lcd(0x27, 16, 2);
RelayDoor door(Pins::relay, true); // true = common active-LOW relay module
Puzzle puzzle(keypad, lcd, door, Pins::buzzer);

bool lastButtonReading = HIGH;
bool stableButtonState = HIGH;
unsigned long lastDebounceMs = 0;
constexpr unsigned long debounceMs = 50;

void setup() {
  Serial.begin(115200);
  pinMode(Pins::manualButton, INPUT_PULLUP);
  Wire.begin(Pins::lcdSda, Pins::lcdScl);
  lcd.init();
  lcd.backlight();
  door.begin();
  puzzle.begin();
  Serial.println("ESP32 door puzzle ready");
}

void loop() {
  puzzle.update();

  const bool reading = digitalRead(Pins::manualButton);
  if (reading != lastButtonReading) lastDebounceMs = millis();
  if ((millis() - lastDebounceMs) > debounceMs && reading != stableButtonState) {
    stableButtonState = reading;
    if (stableButtonState == LOW) {
      if (door.isOpen()) door.close();
      else door.open();
    }
  }
  lastButtonReading = reading;
}
