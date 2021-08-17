#include "Relay.h"

Relay::Relay(byte pin) {
  this->pin = pin;
  init();
}

void Relay::init() {
  pinMode(pin, OUTPUT);
  deactivate();
}

void Relay::activate() {
  changeState(true);
}

void Relay::deactivate() {
  changeState(false);
}

void Relay::changeState(bool state) {
  if (state && !activated) {
    digitalWrite(pin, LOW);
  }
  else if (!state && activated) {
    digitalWrite(pin, HIGH);
  }
  activated = state;
}

bool Relay::isActivated() {
  return activated;
}
