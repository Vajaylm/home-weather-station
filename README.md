# Home weather station

The goal of this project is to create a weather station to get up to date information about temperature, pressure and humidity in a room. According to the data gathered from this station a humidifier, a fan and a heater will be controlled. This will be the first step to dive into smart home solutions.

## Planned tasks:
- Read data from BME280 sensor via I2C and print on the serial monitor
- Read data from RTC module via I2C and print on the serial monitor
- Display data on an LCD screen
- According to the humidity data, control a humidifier
- According to the temperature data, control a fan or a heater
- Add possibility to set thresholds

## Components:
- NodeMcu ESP8266 V3 -> platform change, as Arduino UNO ran out of memory 
- BME280 sensor
- Tiny RTC real time clock module
- 2004 LCD screen
- Raspberry Pi 3B+
- 2 channel relay module
