#include <RTClib.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

RTC_DS1307 rtc;
Adafruit_BME280 bme;

int SAMPLING_TIME = 5000; //ms

void setup() {
  Serial.begin(9600);
  
  rtc.begin();
  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    rtc.adjust(DateTime(__DATE__, __TIME__));
  }
  
  if (!bme.begin(0x76)) {
    Serial.println("Could not find BME280 sensor!");
    while (true);
  }
}

void loop() {
  long startTime = millis();
  
  DateTime now = rtc.now();
  DatePrint(now);
  TimePrint(now);

  float temperature = bme.readTemperature();
  float pressure = bme.readPressure() / 100.0F;
  float humidity = bme.readHumidity();
  FormattedDataPrint("Temperature", temperature, "C");
  FormattedDataPrint("Pressure", pressure, "hPa");
  FormattedDataPrint("Humidity", humidity, "%");
  Serial.println("----------------------------");
  
  delay(startTime + SAMPLING_TIME - millis());
}

void FormattedDataPrint(String prop, float value, String valUnit) {
  Serial.print(prop);
  Serial.print(": ");
  Serial.print(value);
  Serial.println(valUnit);
}

void DatePrint(DateTime actDateTime) {
  Serial.print(actDateTime.year(), DEC);
  Serial.print('.');
  Serial.print(TwoDigitFormatter(actDateTime.month()));
  Serial.print('.');
  Serial.println(TwoDigitFormatter(actDateTime.day()));
}

void TimePrint(DateTime actDateTime) {
  Serial.print(TwoDigitFormatter(actDateTime.hour()));
  Serial.print(':');
  Serial.print(TwoDigitFormatter(actDateTime.minute()));
  Serial.print(':');
  Serial.print(TwoDigitFormatter(actDateTime.second()));  
  Serial.println();
}

String TwoDigitFormatter(int value) {
  return value < 10 ? "0" + String(value) : String(value);
}
