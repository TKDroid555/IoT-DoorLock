#pragma once
#include <Arduino.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include "door.h"

class Puzzle {
public:
  Puzzle(Keypad& keypad, LiquidCrystal_I2C& lcd, DoorActuator& door, uint8_t buzzerPin);
  void begin();
  void update();
private:
  struct Question { const char* prompt; const char* answer; };
  static const Question questions_[];
  static constexpr size_t questionCount_ = 99;
  Keypad& keypad_;
  LiquidCrystal_I2C& lcd_;
  DoorActuator& door_;
  uint8_t buzzerPin_;
  String answer_;
  size_t questionIndex_ = 0;
  void chooseQuestion();
  void draw(const char* status = nullptr);
  void onKey(char key);
  void submit();
  void beep(unsigned long durationMs);
};

