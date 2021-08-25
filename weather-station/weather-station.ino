// Over The Air upload related
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "AuthOTA.h"
#include "Animation.h"
#include "RelayControl.h"
#include "RtcModule.h"

// I2C related
#include <Wire.h>
#include <RTClib.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <LiquidCrystal_I2C.h>

// Instance creation
RtcModule rtc;
Adafruit_BME280 bme;
LiquidCrystal_I2C lcd(0x27, 20, 4); 
RelayControl fanControl(D5, 22.0, 25.0, RelayControl::HIGHER);
RelayControl humidifierControl(D6, 30.0, 60.0, RelayControl::LOWER);

// Global variables
#define SERIAL_PRINT 100000
#define I2C_PRINT 100001
bool bmeRunning = true;
int refresh_time = 1000; //ms
int samplingCycle = 5;
int actCycle = samplingCycle;
float temperature = 0.0;
float pressure = 0.0;
float humidity = 0.0;

void setup() {
  Serial.begin(115200);
  Serial.println("Booting");
  Wire.begin(D2, D1); // D1 - SCL, D2 - SDA
  
  InitWifi();
  InitOTA();
  
  InitRtc();
  InitBmeSensor();  
  InitLcd();

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  long startTime = millis();
  
  PrintDateTime(rtc.getFormattedDate(), rtc.getFormattedTime());
  
  if (bmeRunning) {
    if (actCycle == samplingCycle) {
      ReadSensorData();
    }
    PrintSensorData();
  }

  humidifierControl.handleSwitch(humidity, RelayControl::NORMALLY_OPEN);
  if (humidifierControl.checkRelayActive() == Relay::COIL_ON) {
    byte humAnimId = 0;
    LcdAnimation(humAnimId, 2, 3, humAnim, actCycle);
  }
  else {
    CleanupLcd(2, 3);
  }
  
  fanControl.handleSwitch(temperature, RelayControl::NORMALLY_OPEN);
  if (fanControl.checkRelayActive() == Relay::COIL_ON) {
    byte fanAnimId = 1;
    LcdAnimation(fanAnimId, 5, 3, fanAnim, actCycle % 3);
  }
  else {
    CleanupLcd(5, 3);
  }
    
  ArduinoOTA.handle();
  SetActCycle();
  delay(startTime + refresh_time - millis());
}

// Formatted print for showing data as "Prop: ValueUnit"
void FormattedDataPrint(String prop, float value, String valUnit, int printMode) {
  String dataString = prop + ": " + value + valUnit;
  PrintData(dataString, printMode);
}

void InitRtc() {
  rtc.connect();
}
  
void InitBmeSensor() {
  if (!bme.begin(0x76)) {
    Serial.println("Could not find BME280 sensor!");
    bmeRunning = false;
  }
}

void InitLcd() {
  lcd.init();
  lcd.backlight();
}

void InitWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
}

void InitOTA() {
  ArduinoOTA.setHostname(HOST_NAME);
  ArduinoOTA.setPassword(HOST_PASSWORD);
  
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

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
}

void PrintDateTime(String dateString, String timeString) {
  PrintData(dateString, SERIAL_PRINT);
  PrintData(timeString, SERIAL_PRINT);
  lcd.setCursor(0, 0);
  PrintData(dateString, I2C_PRINT);
  lcd.print("  ");
  PrintData(timeString, I2C_PRINT);
}

void PrintData(String dataString, int printMode) {
  if (printMode == SERIAL_PRINT) {
    Serial.println(dataString);
  }
  else if (printMode == I2C_PRINT) {
    lcd.print(dataString);
  }
}

void ReadSensorData() {
  temperature = bme.readTemperature();
  pressure = bme.readPressure() / 100.0F;
  humidity = bme.readHumidity();
}

void PrintSensorData() {
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

void LcdAnimation(byte animId, byte col, byte row, byte anim[][8], byte animIdx) {
  lcd.createChar(animId, anim[animIdx]);
  lcd.setCursor(col, row);
  lcd.write(animId);
}

void CleanupLcd(byte col, byte row) {
  lcd.setCursor(col, row);
  lcd.print(" ");
}

void SetActCycle() {
  if (actCycle == samplingCycle) {
    actCycle = 0;
  }
  else {
    actCycle++;
  }
}
