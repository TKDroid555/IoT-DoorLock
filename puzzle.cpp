#include "puzzle.h"

const Puzzle::Question Puzzle::questions_[] = {  {"d/dx 5x^2", "10x^1"},
  {"d/dx 3x^2", "6x^1"},
  {"d/dx 4x^3", "12x^2"},
  {"d/dx 2x^3", "6x^2"},
  {"d/dx 5x^3", "15x^2"},
  {"d/dx 2x^4", "8x^3"},
  {"d/dx 3x^4", "12x^3"},
  {"d/dx 4x^4", "16x^3"},
  {"d/dx 6x^2", "12x^1"},
  {"d/dx 7x^2", "14x^1"},
  {"d/dx x^2", "2x^1"},
  {"d/dx x^3", "3x^2"},
  {"d/dx x^4", "4x^3"},
  {"d/dx x^5", "5x^4"},
  {"d/dx 2x^5", "10x^4"},
  {"d/dx 3x^5", "15x^4"},
  {"d/dx 4x^5", "20x^4"},
  {"d/dx 5x^5", "25x^4"},
  {"d/dx 2x^6", "12x^5"},
  {"d/dx 3x^6", "18x^5"},
  {"d/dx 4x^6", "24x^5"},
  {"d/dx 2x^7", "14x^6"},
  {"d/dx 3x^7", "21x^6"},
  {"d/dx 5x^4", "20x^3"},
  {"d/dx 6x^3", "18x^2"},
  {"d/dx 7x^3", "21x^2"},
  {"d/dx 8x^3", "24x^2"},
  {"d/dx 9x^3", "27x^2"},
  {"d/dx 2x^8", "16x^7"},
  {"d/dx 3x^8", "24x^7"},
  {"d/dx 4x^7", "28x^6"},
  {"d/dx 5x^6", "30x^5"},
  {"d/dx 6x^5", "30x^4"},
  {"d/dx 7x^4", "28x^3"},
  {"d/dx 8x^4", "32x^3"},
  {"d/dx 9x^4", "36x^3"},
  {"d/dx 10x^3", "30x^2"},
  {"d/dx 12x^2", "24x^1"},
  {"d/dx 15x^2", "30x^1"},
  {"d/dx 11x^3", "33x^2"},
  {"d/dx x^6", "6x^5"},
  {"d/dx x^7", "7x^6"},
  {"d/dx x^8", "8x^7"},
  {"d/dx x^9", "9x^8"},
  {"d/dx 2x^9", "18x^8"},
  {"d/dx 3x^9", "27x^8"},
  {"d/dx 4x^8", "32x^7"},
  {"d/dx 5x^7", "35x^6"},
  {"d/dx 6x^6", "36x^5"},
  {"d/dx 7x^5", "35x^4"},
  {"S 2x^1 dx", "x^2+c"},
  {"S 3x^2 dx", "x^3+c"},
  {"S 4x^3 dx", "x^4+c"},
  {"S 5x^4 dx", "x^5+c"},
  {"S 6x^5 dx", "x^6+c"},
  {"S 7x^6 dx", "x^7+c"},
  {"S 8x^7 dx", "x^8+c"},
  {"S 9x^8 dx", "x^9+c"},
  {"S 10x^9 dx", "x^10+c"},
  {"S 4x^1 dx", "2x^2+c"},
  {"S 6x^2 dx", "2x^3+c"},
  {"S 8x^3 dx", "2x^4+c"},
  {"S 10x^4 dx", "2x^5+c"},
  {"S 12x^5 dx", "2x^6+c"},
  {"S 14x^6 dx", "2x^7+c"},
  {"S 16x^7 dx", "2x^8+c"},
  {"S 18x^8 dx", "2x^9+c"},
  {"S 20x^9 dx", "2x^10+c"},
  {"S 6x^1 dx", "3x^2+c"},
  {"S 9x^2 dx", "3x^3+c"},
  {"S 12x^3 dx", "3x^4+c"},
  {"S 15x^4 dx", "3x^5+c"},
  {"S 18x^5 dx", "3x^6+c"},
  {"S 21x^6 dx", "3x^7+c"},
  {"S 24x^7 dx", "3x^8+c"},
  {"S 27x^8 dx", "3x^9+c"},
  {"S 30x^9 dx", "3x^10+c"},
  {"S 8x^1 dx", "4x^2+c"},
  {"S 20x^2 dx", "5x^3+c"},
  {"S 24x^3 dx", "6x^4+c"},
  {"S 28x^4 dx", "7x^5+c"},
  {"S 32x^5 dx", "8x^6+c"},
  {"S 36x^6 dx", "9x^7+c"},
  {"S 40x^7 dx", "5x^8+c"},
  {"S 45x^8 dx", "5x^9+c"},
  {"S 50x^9 dx", "5x^10+c"},
  {"S 5 dx", "5x^1+c"},
  {"S 7 dx", "7x^1+c"},
  {"S 9 dx", "9x^1+c"},
  {"S 10 dx", "10x^1+c"},
  {"S 12 dx", "12x^1+c"},
  {"S 15x^2 dx", "5x^3+c"},
  {"S 24x^3 dx", "6x^4+c"},
  {"S 35x^4 dx", "7x^5+c"},
  {"S 14x^1 dx", "7x^2+c"},
  {"S 18x^2 dx", "6x^3+c"},
  {"S 25x^4 dx", "5x^5+c"},
  {"S 42x^6 dx", "6x^7+c"},
  {"S 56x^7 dx", "7x^8+c"},};

Puzzle::Puzzle(Keypad& keypad, LiquidCrystal_I2C& lcd, DoorActuator& door, uint8_t buzzerPin)
    : keypad_(keypad), lcd_(lcd), door_(door), buzzerPin_(buzzerPin) {}

void Puzzle::begin() {
  pinMode(buzzerPin_, OUTPUT);
  digitalWrite(buzzerPin_, LOW);
  randomSeed(esp_random());
  chooseQuestion();
  draw();
}

void Puzzle::update() {
  const char key = keypad_.getKey();
  if (key != NO_KEY) onKey(key);
}

void Puzzle::chooseQuestion() {
  questionIndex_ = static_cast<size_t>(random(questionCount_));
  answer_ = "";
}

void Puzzle::draw(const char* status) {
  lcd_.clear();
  lcd_.setCursor(0, 0);
  String first = status ? String(status) : String(questions_[questionIndex_].prompt);
  lcd_.print(first.substring(0, 16));
  lcd_.setCursor(0, 1);
  String second = ">> " + answer_;
  lcd_.print(second.substring(0, 16));
}

void Puzzle::beep(unsigned long durationMs) {
  digitalWrite(buzzerPin_, HIGH);
  delay(durationMs);
  digitalWrite(buzzerPin_, LOW);
}

void Puzzle::submit() {
  if (answer_ == questions_[questionIndex_].answer) {
    draw("Correct!");
    for (int i = 0; i < 3; ++i) { beep(150); delay(100); }
    chooseQuestion();
    draw();
    door_.open();
  } else {
    draw("Incorrect!");
    beep(1000);
    answer_ = "";
    draw();
  }
}

void Puzzle::onKey(char key) {
  // Keypad's custom keys are mapped in main.cpp: BS='*', ENT='#', CLSD='D'.
  if (key == 'D') { door_.close(); return; }
  if (key == '#') { submit(); return; }
  if (key == '*') {
    if (answer_.length()) answer_.remove(answer_.length() - 1);
  } else if (answer_.length() < 13) {
    if (key == '^') answer_ += 'x';
    if (key == 'c') answer_ += '+';
    answer_ += key;
  }
  draw();
}
