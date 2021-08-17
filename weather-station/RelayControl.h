#ifndef RELAY_CONTROL_H
#define RELAY_CONTROL_H

#include "Relay.h"
#include <Arduino.h>

class RelayControl {
  
  private:
    Relay relay;
    float lowThreshold;
    float highThreshold;
    bool switchOnHigher;
    
  public:
    RelayControl(byte relayPin, float lowThreshold, float highThreshold, bool switchOnHigher);
    float getLowThreshold();
    float getHighThreshold();
    void setLowThreshold(float lowThreshold);
    void setHighThreshold(float highThreshold);
    void handleSwitch(float value, bool normOpen);
    bool checkRelayActive();
    
  private:
    bool checkValueHigher(float value);
    bool checkValueLower(float value);
    void switchRelay(bool state);  
    
};

#endif
