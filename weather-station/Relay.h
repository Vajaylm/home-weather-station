#ifndef RELAY_H
#define RELAY_H

#include <Arduino.h>

// The relays for which this class is created must get LOW on input to activate
class Relay {
  public:
    enum CoilState {
      COIL_ON,
      COIL_OFF
    };
  
  private:
    const byte pin;
    CoilState activated;
    
  public:
    Relay(byte pin);
    void activate();
    void deactivate();
    CoilState isActivated();

  private:
    void init();
    void changeState(CoilState state);
};

#endif
