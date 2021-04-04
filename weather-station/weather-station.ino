#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

Adafruit_BME280 bme;

void setup() {
  Serial.begin(9600);

  if (!bme.begin(0x76)) {
    Serial.println("Could not find BME280 sensor!");
    while (true);
  }
  else {
    Serial.println("BME280 sensor found!");
  }

}

void loop() {
  

}
