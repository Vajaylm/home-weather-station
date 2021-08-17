#include "RelayControl.h"
#include "Relay.h"

RelayControl::RelayControl(byte relayPin, float lowThreshold, float highThreshold, bool switchOnHigher) 
  : relay(relayPin) {
    this->lowThreshold = lowThreshold;
    this->highThreshold = highThreshold;
    this->switchOnHigher = switchOnHigher;
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

void RelayControl::handleSwitch(float value, bool normOpen) {
  if (switchOnHigher && checkValueHigher(value) && !checkRelayActive()) {
    switchRelay(normOpen);
  }
  else if (switchOnHigher && checkValueLower(value) && checkRelayActive()) {
    switchRelay(!normOpen);
  }
  else if (!switchOnHigher && checkValueHigher(value) && checkRelayActive()) {
    switchRelay(!normOpen);      
  }
  else if (!switchOnHigher && checkValueLower(value) && !checkRelayActive()) {
    switchRelay(normOpen);
  }
}

void RelayControl::switchRelay(bool state) {
  if (state) {
    relay.activate();  
  }
  else {
    relay.deactivate();  
  }
}

bool RelayControl::checkRelayActive() {
  return relay.isActivated();
}
