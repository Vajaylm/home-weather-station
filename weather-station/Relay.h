#ifndef RELAY_H
#define RELAY_H

#include <Arduino.h>

// The relays for which this class is created must get LOW on input to activate
class Relay {
  
  private:
    byte pin;
    bool activated = false;
    
  public:
    Relay(byte pin);
    void activate();
    void deactivate();
    bool isActivated();

  private:
    void init();
    void changeState(bool state);
};

#endif
