#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

void setup()
{
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);
  
  if (!myDFPlayer.begin(mySoftwareSerial)) {
    Serial.println("Failed to initialize MP3 player.");
    
    while (true) {
      delay(0);
    }
  }
  Serial.println("MP3 player initialized.");
  
  myDFPlayer.volume(30);  // 0-30
  myDFPlayer.play(1);     // Play "mp3/00001.mp3" on SD card.
}

void loop() {

}
