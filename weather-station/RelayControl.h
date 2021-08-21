#ifndef RELAY_CONTROL_H
#define RELAY_CONTROL_H

#include "Relay.h"
#include <Arduino.h>

class RelayControl {
  public:
    enum SwitchMode {
      HIGHER = 1,
      LOWER = 2
    };

    enum Connection {
      NORMALLY_OPEN = 3,
      NORMALLY_CLOSED = 4
    };
  
  private:
    Relay relay;
    float lowThreshold;
    float highThreshold;
    SwitchMode switchMode;
    
  public:
    RelayControl(byte relayPin, float lowThreshold, float highThreshold, SwitchMode switchMode);
    float getLowThreshold();
    float getHighThreshold();
    void setLowThreshold(float lowThreshold);
    void setHighThreshold(float highThreshold);
    void handleSwitch(float value, Connection conn);
    Relay::CoilState checkRelayActive();
    
  private:
    bool checkValueHigher(float value);
    bool checkValueLower(float value);
    void switchRelay(Relay::CoilState state);
};

#endif
