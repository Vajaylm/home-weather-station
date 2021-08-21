#include "RelayControl.h"

RelayControl::RelayControl(byte relayPin, float lowThreshold, float highThreshold, SwitchMode switchMode) 
  : relay(relayPin) {
    this->lowThreshold = lowThreshold;
    this->highThreshold = highThreshold;
    this->switchMode = switchMode;
}

float RelayControl::getLowThreshold() {
  return lowThreshold;
}

float RelayControl::getHighThreshold() {
  return highThreshold;
}

void RelayControl::setLowThreshold(float lowThreshold) {
  this->lowThreshold = lowThreshold;
}

void RelayControl::setHighThreshold(float highThreshold) {
  this->highThreshold = highThreshold;
}

bool RelayControl::checkValueHigher(float value) {
  return value > highThreshold;
}

bool RelayControl::checkValueLower(float value) {
  return value < lowThreshold;
}

void RelayControl::handleSwitch(float value, Connection conn) {
  if (switchMode == HIGHER && checkValueHigher(value) && checkRelayActive() == Relay::COIL_OFF) {
    switchRelay(conn == NORMALLY_OPEN ? Relay::COIL_ON : Relay::COIL_OFF);
  }
  else if (switchMode == HIGHER && checkValueLower(value) && checkRelayActive() == Relay::COIL_ON) {
    switchRelay(conn == NORMALLY_CLOSED ? Relay::COIL_ON : Relay::COIL_OFF);
  }
  else if (switchMode == LOWER && checkValueHigher(value) && checkRelayActive() == Relay::COIL_ON) {
    switchRelay(conn == NORMALLY_CLOSED ? Relay::COIL_ON : Relay::COIL_OFF);
  }
  else if (switchMode == LOWER && checkValueLower(value) && checkRelayActive() == Relay::COIL_OFF) {
    switchRelay(conn == NORMALLY_OPEN ? Relay::COIL_ON : Relay::COIL_OFF);
  }
}

void RelayControl::switchRelay(Relay::CoilState state) {
  if (state == Relay::COIL_ON) {
    relay.activate();  
  }
  else {
    relay.deactivate();  
  }
}

Relay::CoilState RelayControl::checkRelayActive() {
  return relay.isActivated();
}
