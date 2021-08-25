#ifndef I2C_DEVICE_H
#define I2C_DEVICE_H

#include <Arduino.h>

class I2CDevice {
  public:
    enum State {
      NO_STATE,
      INITIALIZED,
      NOT_CONNECTED,
      CONNECTED
    };

  protected:
    int address;
    State state = NO_STATE;
    
  public:
    virtual void connect();
    State getState();
};

#endif
