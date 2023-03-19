# ESP8266 with BME680 sensor

This is a PlatformIO project for ESP8266 chip with Node MCU V2 board.

It reads regularly the values from the BME sensor and stores the last few values in memory.

The network it tries to connect to is defined in the [WifiHandler.h](include/WifiHandler.h) file, replace the values there for your network.

A website is exposed to take a look at the logs and an endpoint to get the last values for each sensor (temperature, humidity, gas, altitude and pressure).
