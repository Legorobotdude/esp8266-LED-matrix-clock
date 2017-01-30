#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* WiFi Access Point *********************************/
const char *ssid     = "SSID";
const char *password = "PASSWORD";


/************************* Adafruit.io Setup *********************************/
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  8883
#define AIO_USERNAME    "user"
#define AIO_KEY         "key"

WiFiClientSecure client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_USERNAME, AIO_KEY);

Adafruit_MQTT_Subscribe timefeed = Adafruit_MQTT_Subscribe(&mqtt, "time/seconds");

// set timezone offset from UTC
int timeZone = -4; // UTC - 4 eastern daylight time (nyc)
int interval = 4; // trigger every X hours
int hour = 0; // current hour

void timecallback(uint32_t current) {

  // stash previous hour
  int previous = hour;

  // adjust to local time zone
  current += (timeZone * 60 * 60);

  // calculate current hour
  hour = (current / 60 / 60) % 24;

  // only trigger on interval
  if((hour != previous) && (hour % interval) == 0) {
    Serial.println("Run your code here");
  }

}

/************************* LED MATRIX SETUP *********************************/
int pinCS = 16; // Attach CS to this pin, DIN to MOSI and CLK to SCK (cf http://arduino.cc/en/Reference/SPI )
int numberOfHorizontalDisplays = 4;//adjust this to your setup
int numberOfVerticalDisplays = 1;

Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);


const int wait = 100; 
const int length = 8;


WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP,-21600);


void setup() {


  WiFi.begin(ssid, password);
   while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    
  }

  timefeed.setCallback(timecallback);
  mqtt.subscribe(&timefeed);
  mqtt.subscribe(&test);

  
  timeClient.begin();//depreciated soon


 
  matrix.setIntensity(4); // Set brightness between 0 and 15

  matrix.setRotation(0, 1);//you may have to change this section depending on your LED matrix setup
  matrix.setRotation(1,1);
  matrix.setRotation(2,1);
  matrix.setRotation(3,1);
  matrix.setTextSize(1);
  matrix.setTextWrap(false); 
  matrix.setTextColor(HIGH);
}

void loop() {

MQTT_connect();
timeClient.update();//updates time

  
matrix.fillScreen(LOW);//Empty the screen
matrix.setCursor(0,0);//Move the cursor to the end of the screen
matrix.print(timeClient.getFormattedTime());//Write the time
matrix.write();
delay(5000);//adjust to how often you want the clock to update

mqtt.ping();

}



// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }

  Serial.println("MQTT Connected!");
}
