/*
*   PingTimerExample.ino
*   Copyright (C) 2019 2023 David C.(deltagrobotics@gmail.com)
*
*   The Trigger and Echo pins must be connected as described
*        in the library documentation
*
*/


// Include the library
#include "PingTimer.h"

void setup() {  
  Serial.begin(9600);
  // call ping.begin() to set things up
  ping.begin();
  // call sendPing to start a measurement. 
  ping.sendPing();
}

void loop() {
  //  if the ping sensor has new data save it to distance and print it.
  if(ping.hasNewData()){
    // get distance in millimeters
    int distance = ping.getDistanceMM();
    Serial.print("Current Distance: ");
    Serial.println(distance);
    // call sendPing to start a new measurement
    ping.sendPing();   
  }   
}
