#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme;

void setup() {
  Serial.begin(9600);

  if (!bme.begin(0x76)) {
    Serial.println("Could not find BME280 sensor!");
    while (true);
  }
  else {
    Serial.println("BME280 sensor found!");
    Serial.println("");
  }
}

void loop() {
  float temperature = bme.readTemperature();
  float pressure = bme.readPressure() / 100.0F;
  float humidity = bme.readHumidity();
  float alt = bme.readAltitude(SEALEVELPRESSURE_HPA);
  
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println("C");

  Serial.print("Pressure: ");
  Serial.print(pressure);
  Serial.println("hPa");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  Serial.print("Altitude: ");
  Serial.print(alt);
  Serial.println("m");


  Serial.println("----------------------------");
  delay(2000);
}
