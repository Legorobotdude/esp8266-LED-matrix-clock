#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>

int pinCS = 16; // Attach CS to this pin, DIN to MOSI and CLK to SCK (cf http://arduino.cc/en/Reference/SPI )
int numberOfHorizontalDisplays = 4;
int numberOfVerticalDisplays = 1;

Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);


const int wait = 100; // In milliseconds
const int length = 8;
const char *ssid     = "SSID";
const char *password = "PASSWORD";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP,-21600);


void setup() {


  WiFi.begin(ssid, password);
   while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    //Serial.print ( "." );
  }

  timeClient.begin();


  // put your setup code here, to run once:
matrix.setIntensity(4); // Set brightness between 0 and 15

 matrix.setRotation(0, 1);
  matrix.setRotation(1,1);
   matrix.setRotation(2,1);
    matrix.setRotation(3,1);
    matrix.setTextSize(1);
  matrix.setTextWrap(false);  // we dont want text to wrap so it scrolls nicely
  matrix.setTextColor(HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
timeClient.update();

  
    matrix.fillScreen(LOW);
    matrix.setCursor(0,0);
    matrix.print(timeClient.getFormattedTime());
    matrix.write();
    delay(5000);


//matrix.write();
}
