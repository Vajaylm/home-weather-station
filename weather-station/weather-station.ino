#include <RTClib.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

RTC_DS1307 rtc;
Adafruit_BME280 bme;

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
  delay(2000);
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
  Serial.print(actDateTime.month(), DEC);
  Serial.print('.');
  Serial.println(actDateTime.day(), DEC);
}

void TimePrint(DateTime actDateTime) {
  Serial.print(actDateTime.hour(), DEC);
  Serial.print(':');
  Serial.print(actDateTime.minute(), DEC);
  Serial.print(':');
  Serial.print(actDateTime.second(), DEC);
  Serial.println();
}
