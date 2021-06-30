// Over The Air upload related
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "AuthOTA.h"
#include "Animation.h"

// I2C related
#include <Wire.h>
#include <RTClib.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <LiquidCrystal_I2C.h>

// Instance creation
RTC_DS1307 rtc;
Adafruit_BME280 bme;
LiquidCrystal_I2C lcd(0x27, 20, 4); 

// Global variables
#define SERIAL_PRINT 100000
#define I2C_PRINT 100001
bool rtcRunning = true;
bool bmeRunning = true;
int refresh_time = 1000; //ms
int samplingCycle = 5;
int actCycle = samplingCycle;
float temperature = 0.0;
float pressure = 0.0;
float humidity = 0.0;
byte humidifierRelayPin = D6;
float humidityThresholdLow = 30.0;
float humidityThresholdHigh = 60.0;
byte fanRelayPin = D5;
float temperatureThresholdLow = 22.0;
float temperatureThresholdHigh = 25.0;


void setup() {
  pinMode(humidifierRelayPin, OUTPUT);
  digitalWrite(humidifierRelayPin, HIGH);
  pinMode(fanRelayPin, OUTPUT);
  digitalWrite(fanRelayPin, HIGH);
  
  Serial.begin(115200);
  Serial.println("Booting");
  Wire.begin(D2, D1); // D1 - SCL, D2 - SDA

  rtc.begin();
  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    rtcRunning = false;
    rtc.adjust(DateTime(__DATE__, __TIME__));
  }
  
  if (!bme.begin(0x76)) {
    Serial.println("Could not find BME280 sensor!");
    bmeRunning = false;
  }

  lcd.init();
  lcd.backlight();
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  ArduinoOTA.setHostname(HOST_NAME);
  ArduinoOTA.setPassword(HOST_PASSWORD);
  
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
  long startTime = millis();
  
  if (rtcRunning) {
    DateTime now = rtc.now();
    DatePrint(now, SERIAL_PRINT);
    TimePrint(now, SERIAL_PRINT);
    lcd.setCursor(0, 0);
    DatePrint(now, I2C_PRINT);
    lcd.print("  ");
    TimePrint(now, I2C_PRINT);
  }
  
  if (bmeRunning) {
    if (actCycle == samplingCycle) {
      //actCycle = 0;
      temperature = bme.readTemperature();
      pressure = bme.readPressure() / 100.0F;
      humidity = bme.readHumidity();
    }
    FormattedDataPrint("Temperature", temperature, "C", SERIAL_PRINT);
    FormattedDataPrint("Pressure", pressure, "hPa", SERIAL_PRINT);
    FormattedDataPrint("Humidity", humidity, "%", SERIAL_PRINT);
    Serial.println("----------------------------");
    lcd.setCursor(0, 1);
    FormattedDataPrint("T", temperature, "C", I2C_PRINT);
    lcd.print("  ");
    FormattedDataPrint("h", humidity, "%", I2C_PRINT);
    lcd.setCursor(0, 2);
    FormattedDataPrint("p", pressure, "hPa", I2C_PRINT);
    lcd.print(" ");
  }

  if (humidity <= humidityThresholdLow) {
    // Turn on humidifier
    if (digitalRead(humidifierRelayPin) == HIGH) {
      digitalWrite(humidifierRelayPin, LOW);  
    }

    // Animation for humidifier
    byte humAnimId = 0;
    lcd.createChar(humAnimId, humAnim[actCycle]);
    lcd.setCursor(2, 3);
    lcd.write(humAnimId);
  }
  else if (humidity >= humidityThresholdHigh && digitalRead(humidifierRelayPin) == LOW) {
    // Turn off humidifier, clear animation position on LCD
    digitalWrite(humidifierRelayPin, HIGH);
    lcd.setCursor(2, 3);
    lcd.print(" ");
  }

  if (temperature >= temperatureThresholdHigh) {
    // Turn on fan
    if (digitalRead(fanRelayPin) == HIGH) {
      digitalWrite(fanRelayPin, LOW);  
    }
    
    // Animation for fan
    byte fanAnimId = 1;
    lcd.createChar(fanAnimId, fanAnim[actCycle % 3]);
    lcd.setCursor(5, 3);
    lcd.write(fanAnimId);
  }
  else if (temperature <= temperatureThresholdLow && digitalRead(fanRelayPin) == LOW) {
    // Turn off fan, clear animation position on LCD
    digitalWrite(fanRelayPin, HIGH);
    lcd.setCursor(5, 3);
    lcd.print(" ");
  }
  
  ArduinoOTA.handle();
  if (actCycle == samplingCycle) {
    actCycle = 0;
  }
  else {
    actCycle++;
  }
  delay(startTime + refresh_time - millis());
  
}

// Formatted print for showing data as "Prop: ValueUnit"
void FormattedDataPrint(String prop, float value, String valUnit, int printMode) {
  if (printMode == SERIAL_PRINT) {
    Serial.print(prop);
    Serial.print(": ");
    Serial.print(value);
    Serial.println(valUnit);
  }
  else if (printMode == I2C_PRINT) {
    lcd.print(prop);
    lcd.print(": ");
    lcd.print(value);
    lcd.print(valUnit);
  }
}

// Formatted print for showing date as "YYYY.MM.DD"
void DatePrint(DateTime actDateTime, int printMode) {
  if (printMode == SERIAL_PRINT) {
    Serial.print(actDateTime.year(), DEC);
    Serial.print('.');
    Serial.print(TwoDigitFormatter(actDateTime.month()));
    Serial.print('.');
    Serial.println(TwoDigitFormatter(actDateTime.day()));
  }
  else if (printMode == I2C_PRINT) {
    lcd.print(actDateTime.year(), DEC);
    lcd.print('.');
    lcd.print(TwoDigitFormatter(actDateTime.month()));
    lcd.print('.');
    lcd.print(TwoDigitFormatter(actDateTime.day()));
  }
}

// Formatted print for showing time as "HH:MM:SS"
void TimePrint(DateTime actDateTime, int printMode) {
  if (printMode == SERIAL_PRINT) {
    Serial.print(TwoDigitFormatter(actDateTime.hour()));
    Serial.print(':');
    Serial.print(TwoDigitFormatter(actDateTime.minute()));
    Serial.print(':');
    Serial.print(TwoDigitFormatter(actDateTime.second()));  
    Serial.println();
  }
  else if (printMode == I2C_PRINT) {
    lcd.print(TwoDigitFormatter(actDateTime.hour()));
    lcd.print(':');
    lcd.print(TwoDigitFormatter(actDateTime.minute()));
    lcd.print(':');
    lcd.print(TwoDigitFormatter(actDateTime.second()));
  }
}

String TwoDigitFormatter(int value) {
  return value < 10 ? "0" + String(value) : String(value);
}
