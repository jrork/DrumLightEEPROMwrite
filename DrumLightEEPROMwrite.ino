/*
 * Tool to write initial values to EEPROM for DrumLight program
*/

#include <EEPROM.h>
#include "drums.h"

#define COLOR 65523
#define BRIGHTNESS 100
#define THRESHOLD 80
#define DELAY 20
#define MODE 0
#define PIXEL_COUNT 13

drumLight myDrumLight = {Bass1, COLOR, BRIGHTNESS, THRESHOLD, DELAY, MODE, PIXEL_COUNT};

void setup()
{
    char* myDrumLightBytes = reinterpret_cast<char*>(&myDrumLight);
    const uint32_t myDrumLightSize = sizeof(myDrumLight);

  EEPROM.begin(myDrumLightSize);  //Initialize EEPROM

  for(int index = 0; index < myDrumLightSize; index++){
    EEPROM.write(index, myDrumLightBytes[index]);
  }
  
  EEPROM.commit();    //Store data to EEPROM
}

void loop()
{
  delay(1000);
}
