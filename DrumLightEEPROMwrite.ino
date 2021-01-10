/*
 * Tool to write initial values to EEPROM for DrumLight program
*/

#include <EEPROM.h>
#include "drums.h"

drumLight myDrumLight = {Bass1, 65523, 75, 80, 20, 0};

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
