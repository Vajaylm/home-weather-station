#include "Relay.h"

Relay::Relay(byte pin) : pin(pin) {
  activated = COIL_ON;
  init();
}

void Relay::init() {
  pinMode(pin, OUTPUT);
  deactivate();
}

void Relay::activate() {
  changeState(COIL_ON);
}

void Relay::deactivate() {
  changeState(COIL_OFF);
}

void Relay::changeState(CoilState state) {
  if (state == COIL_ON && activated == COIL_OFF) {
    digitalWrite(pin, LOW);
  }
  else if (state == COIL_OFF && activated == COIL_ON) {
    digitalWrite(pin, HIGH);
  }
  activated = state;
}

Relay::CoilState Relay::isActivated() {
  return activated;
}
