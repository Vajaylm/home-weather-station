#ifndef RTC_MODULE_H
#define RTC_MODULE_H

#include "I2CDevice.h"
#include <RTClib.h>

class RtcModule: public I2CDevice {  
  private:
    RTC_DS1307 rtc_ds1307;
      
  public:
    RtcModule();
    void connect();
    void setDateTime();
    String getFormattedDate();
    String getFormattedTime();
  
  private:
    DateTime getNow();
    String formatTwoDigit(int value);
};

#endif
