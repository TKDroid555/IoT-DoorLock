#pragma once
#include <Arduino.h>

// Door actuator controlled by one relay channel.
class DoorActuator {
public:
  virtual ~DoorActuator() = default;
  virtual void begin() = 0;
  virtual void open() = 0;
  virtual void close() = 0;
  virtual bool isOpen() const = 0;
};

class RelayDoor final : public DoorActuator {
public:
  // Set activeLow=false for relay modules whose input turns on with HIGH.
  RelayDoor(uint8_t pin, bool activeLow = true);
  void begin() override;
  void open() override;
  void close() override;
  bool isOpen() const override { return open_; }
private:
  void setRelay(bool energized);
  uint8_t pin_;
  bool activeLow_;
  bool open_ = false;
};