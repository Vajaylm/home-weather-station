#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "AuthOTA.h"

void setup() {  
  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  ArduinoOTA.handle();
  delay(500);
}



//#include <RTClib.h>
//
//#include <Wire.h>
//#include <Adafruit_Sensor.h>
//#include <Adafruit_BME280.h>
//
//RTC_DS1307 rtc;
//Adafruit_BME280 bme;
//
//int SAMPLING_TIME = 5000; //ms
//
//void setup() {
//  Serial.begin(9600);
//  
//  rtc.begin();
//  if (!rtc.isrunning()) {
//    Serial.println("RTC is NOT running!");
//    rtc.adjust(DateTime(__DATE__, __TIME__));
//  }
//  
//  if (!bme.begin(0x76)) {
//    Serial.println("Could not find BME280 sensor!");
//    while (true);
//  }
//}
//
//void loop() {
//  long startTime = millis();
//  
//  DateTime now = rtc.now();
//  DatePrint(now);
//  TimePrint(now);
//
//  float temperature = bme.readTemperature();
//  float pressure = bme.readPressure() / 100.0F;
//  float humidity = bme.readHumidity();
//  FormattedDataPrint("Temperature", temperature, "C");
//  FormattedDataPrint("Pressure", pressure, "hPa");
//  FormattedDataPrint("Humidity", humidity, "%");
//  Serial.println("----------------------------");
//  
//  delay(startTime + SAMPLING_TIME - millis());
//}
//
//void FormattedDataPrint(String prop, float value, String valUnit) {
//  Serial.print(prop);
//  Serial.print(": ");
//  Serial.print(value);
//  Serial.println(valUnit);
//}
//
//void DatePrint(DateTime actDateTime) {
//  Serial.print(actDateTime.year(), DEC);
//  Serial.print('.');
//  Serial.print(TwoDigitFormatter(actDateTime.month()));
//  Serial.print('.');
//  Serial.println(TwoDigitFormatter(actDateTime.day()));
//}
//
//void TimePrint(DateTime actDateTime) {
//  Serial.print(TwoDigitFormatter(actDateTime.hour()));
//  Serial.print(':');
//  Serial.print(TwoDigitFormatter(actDateTime.minute()));
//  Serial.print(':');
//  Serial.print(TwoDigitFormatter(actDateTime.second()));  
//  Serial.println();
//}
//
//String TwoDigitFormatter(int value) {
//  return value < 10 ? "0" + String(value) : String(value);
//}
