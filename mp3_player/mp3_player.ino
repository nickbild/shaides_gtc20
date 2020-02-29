////
// Switch programmer for Trinket Pro to: USBtinyISP
// Other boards: AVRISP mkll
////

#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

int musicPin = 4;

void setup()
{
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);

  pinMode(musicPin, INPUT);
  
  if (!myDFPlayer.begin(mySoftwareSerial)) {
    Serial.println("Failed to initialize MP3 player.");
    
    while (true) {
      delay(0);
    }
  }
  Serial.println("MP3 player initialized.");
  
  myDFPlayer.volume(30);  // 0-30
  myDFPlayer.EQ(DFPLAYER_EQ_ROCK);
}

bool playing = 0;

void loop() {
    // Toggle music on/off.
    if (digitalRead(musicPin) == HIGH) {
      if (playing) {
        myDFPlayer.pause();
        playing = 0;
      } else {
        myDFPlayer.play(1);     // Play "mp3/0001.mp3" on SD card.
        //delay(198000);          // Remove when GPIO connected.
        playing = 1;
      }

      while (digitalRead(musicPin) == HIGH) {
        delay(10);
      }
    }
}
