#include "door.h"

RelayDoor::RelayDoor(uint8_t pin, bool activeLow)
    : pin_(pin), activeLow_(activeLow) {}

void RelayDoor::begin() {
  pinMode(pin_, OUTPUT);
  close(); // Start in the locked/closed state.
}

void RelayDoor::setRelay(bool energized) {
  const uint8_t onLevel = activeLow_ ? LOW : HIGH;
  digitalWrite(pin_, energized ? onLevel : (activeLow_ ? HIGH : LOW));
}

void RelayDoor::open() {
  setRelay(true);
  open_ = true;
  Serial.println("Door open (relay energized)");
}

void RelayDoor::close() {
  setRelay(false);
  open_ = false;
  Serial.println("Door closed (relay de-energized)");
}