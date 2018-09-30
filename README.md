# vehicle-tracking-system
A real-time vehicle tracking system with ARM7

This project is implemented with LPC2148 microcontroller. GPS module SIM28 is used to collect data from GPS satellites.
This data is in NMEA format. latitude and longitude are parsed from GPGGA token.

A SIM800 module is used to provide GPRS connectivity.A builtin HTTP stack helps in sending data to the firebase cloud.
data is sent with POST method which creates a new URI everytime it receives new data.

The web side is designed with HTML and javaScript. google maps API is used for mapping  the coodinates.

location is changed in efficient manner as the marker is only moved when new instance of data is created.
