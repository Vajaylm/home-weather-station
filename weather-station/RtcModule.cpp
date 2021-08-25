#include "RtcModule.h"

RtcModule::RtcModule() {
  address = 0x68;
  state = INITIALIZED;
}

void RtcModule::connect() {
  rtc_ds1307.begin();
  if (!rtc_ds1307.isrunning()) {
    state = NOT_CONNECTED;
  }
  else {
    state = CONNECTED;
  }
}

void RtcModule::setDateTime() {
  if (state == CONNECTED) {
    rtc_ds1307.adjust(DateTime(__DATE__, __TIME__));
  }
}

String RtcModule::getFormattedDate() {
  String dateString = "";
  
  if (state == CONNECTED) {
    DateTime now = getNow();
    dateString = String(now.year(), DEC) + "." + formatTwoDigit(now.month()) + "." + formatTwoDigit(now.day());
  }

  return dateString;
}

String RtcModule::getFormattedTime() {
  String timeString = "";
  
  if (state == CONNECTED) {
    DateTime now = getNow();
    timeString = formatTwoDigit(now.hour()) + ":" + formatTwoDigit(now.minute()) + ":" + formatTwoDigit(now.second());
  }

  return timeString;
}

DateTime RtcModule::getNow() {
  return rtc_ds1307.now();
}

String RtcModule::formatTwoDigit(int value) {
  return value < 10 ? "0" + String(value) : String(value);
}
