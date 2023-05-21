
#include "PingTimer.h"

int distance;

void setup() {
  
  Serial.begin(9600);
  ping.begin();
  ping.sendPing();

}

void loop() {

  //  if the ping sensor has new data save it to distance and print it.
  if(ping.hasNewData()){
    distance = ping.getDistanceMM();
    Serial.print("Current Distance: ");
    Serial.println(distance);
    ping.sendPing();   
  }   

  //  Do other things not blocked by ping sensor

}
