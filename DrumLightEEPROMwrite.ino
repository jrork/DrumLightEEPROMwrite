/*
 * Tool to write initial values to EEPROM for DrumLight program
*/

#include <EEPROM.h>
#include <WiFiManager.h>        // For managing the Wifi Connection by TZAPU
#include <ESP8266WiFi.h>        // For running the Web Server
#include <ESP8266WebServer.h>   // For running the Web Server
#include <ESP8266mDNS.h>        // For running OTA and Web Server
#include <WiFiUdp.h>            // For running OTA
#include <ArduinoOTA.h>         // For running OTA
#include "drums.h"
#include "modes.h"

// Device Info
const char* devicename = "DrumTest";
const char* devicepassword = "onairadmin";

#define COLOR 65523
#define DRUM_ID bass3       // See drums.h
#define BRIGHTNESS 100      // Valid values are 0-100
#define THRESHOLD 80        // Valid values are 0-100
#define DELAY 20            // ms delay for on and off
#define MODE lightOnHitMode // check modes.h
#define PIXEL_COUNT 13      // this overrides the pixel count set in the code
// Bass drum has 87 pixels
// Snare drum has 21 pixels

struct drumLight {
  drumID drumId;
  uint32_t color;
  uint8_t brightness;
  uint32_t threshold;
  uint8_t delayValue;
  modeID triggerMode;
  uint8_t pixelCount;
};

drumLight myDrumLight = {DRUM_ID, COLOR, BRIGHTNESS, THRESHOLD, DELAY, MODE, PIXEL_COUNT};

void setup()
{
    char* myDrumLightBytes = reinterpret_cast<char*>(&myDrumLight);
    const uint32_t myDrumLightSize = sizeof(myDrumLight);

  EEPROM.begin(myDrumLightSize);  //Initialize EEPROM

  for(int index = 0; index < myDrumLightSize; index++){
    EEPROM.write(index, myDrumLightBytes[index]);
  }
  
  EEPROM.commit();    //Store data to EEPROM
    //
  // Set up the Wifi Connection
  //
  WiFi.hostname(devicename);
  WiFi.mode(WIFI_STA);      // explicitly set mode, esp defaults to STA+AP
  WiFiManager wm;
  // wm.resetSettings();    // reset settings - for testing

  // Set static IP to see if it fixes my problem - joe
  IPAddress _ip = IPAddress(192, 168, 1, 13);
  IPAddress _gw = IPAddress(192, 168, 1, 1);
  IPAddress _sn = IPAddress(255, 255, 255, 0);
  wm.setSTAStaticIPConfig(_ip, _gw, _sn);
  
  wm.setAPCallback(configModeCallback); //set callback that gets called when connecting to previous WiFi fails, and enters Access Point mode
  //if it does not connect it starts an access point with the specified name here  "AutoConnectAP"
  if (!wm.autoConnect()) {
    //Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(1000);
  }
    //
  // Set up OTA
  //
  // ArduinoOTA.setPort(8266);
  ArduinoOTA.setHostname(devicename);
  ArduinoOTA.setPassword(devicepassword);
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }
    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    //Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    //Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    //Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    //Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      //Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      //Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      //Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      //Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      //Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  Serial.println("Write complete.");
}

void loop()
{
  delay(1000);
}

/*
 * gets called when WiFiManager enters configuration mode
 */
void configModeCallback (WiFiManager *myWiFiManager) {

  //entered config mode, make led toggle faster
  //ticker.attach(0.2, tick);
}
