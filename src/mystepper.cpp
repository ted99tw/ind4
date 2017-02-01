#include <Arduino.h>
#include "mystepper.h"

MyStepper::MyStepper(int pul, int dir){
  pinPul = pul;
  pinDir = dir;
  pinMode(pul, OUTPUT);
  pinMode(dir, OUTPUT);
}

void MyStepper::go(int sp, int dir, int steps){
  int delayTimes;
  switch (sp){
    case 1 :
      delayTimes = 35;
      break;
    case 2 :
      delayTimes = 120;
      break;
    case 3 :
      delayTimes = 250;
      break;
    case 4 :
      delayTimes = 350;
      break;
    case 5 :
      delayTimes = 500;
      break;  
    default :
      delayTimes = 250;
      break;   
  }
  for(int i = 0; i < steps; i++){
    if(dir == 1){    
      digitalWrite(pinDir, HIGH);
      digitalWrite(pinPul, HIGH);
      delayMicroseconds(delayTimes);
      digitalWrite(pinPul, LOW);
      delayMicroseconds(delayTimes);
    } else {
      digitalWrite(pinDir, LOW);
      digitalWrite(pinPul, HIGH);
      delayMicroseconds(delayTimes);
      digitalWrite(pinPul, LOW);
      delayMicroseconds(delayTimes); 
    }
  }
}
	
