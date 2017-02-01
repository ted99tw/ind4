#ifndef __MeArm__
#define __MeArm__

#include <Arduino.h>
//#include <Servo.h>
#include "SoftwareServo.h"

class MeArm
{
	public:
    MeArm(int, int, int, int);  // 4 pins
    void angleTo(int, int, int, int, int, int); // a0,a1,a2, gripper on/off, actions, delay secons
		void gripperOpen(boolean);
    void begin();
    void delayWithRefresh(int);
	private:
    void armfromto(int f[], int t[]);   
    void actions(int); 
    int pin[4];
    int org[3] = {90,60,165}; 
    int from[3];
    int to[3];
    int delayfresh = 0;
    const int OPEN = 30, CLOSE = 0;
    //Servo myservo[4];
    SoftwareServo myservo[4];
};

#endif
