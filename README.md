# esp8266-LED-matrix-clock
A IOT enabled LED Matrix clock powered by an ESP8266 and MAX7219 LED drivers.
![The clock working](/src/other/IMG_20170121_090344.jpg?raw=true)
This project uses 4 LED matrixes powered by MAX7219, you can either wire these yourself or purchase them inexpensively online. (I purchased [mine from AliExpress](https://www.aliexpress.com/item/MAX7219-Dot-Matrix-Module-For-Arduino-Microcontroller-4-In-One-Display-with-5P-Line/32618155357.html), look for "MAX7219 Dot Matrix Module"

Any ESP8266 module should work, just make sure you are using the right SPI pins for your module. You will also need a bi-directional logic level converter since the matrix runs at 5v and the ESP8266 at 3.3v.

You will need wire the ESP8266's hardware SPI to the MAX7219. For most ESP8266 this will be as follows: 
SPI CLK = GPIO14
SPI MOSI = GPIO13
SPI MISO = GPIO12
Wire CLK to CLK and MOSI to DIN. The CS pin from the 7219 can be connected to any GPIO, just define it in the code. The default is 16.

Adjust the matrix rotation in the setup for your Matrices.

Make sure you have the [MAX72XX Panel Library](https://github.com/markruys/arduino-Max72xxPanel) and the [NTP Client Library](https://github.com/arduino-libraries/NTPClient) installed. (Note that the Max72xx library requires the Adafruit GFX library.)

Work In Progress/Future Plans
- Adafruit IO support (Graphing, events)
- RSS News Headlines on request
