#include "MeArm.h"
MeArm::MeArm(int p0, int p1, int p2, int p3){ 
  pin[0] = p0;
  pin[1] = p1;
  pin[2] = p2;
  pin[3] = p3;  
}

void MeArm::begin(){
  for (int i = 0; i < 4; i++) myservo[i].attach(pin[i]);  
  for (int i = 0; i < 3; i++){
    myservo[i].write(org[i]); 
    from[i] = org[i];
    to[i] = org[i];
  }
  myservo[3].write(25); 
}

//smoothly move arm
void MeArm::armfromto(int *arrf, int *arrt){
  int maxAngles = 0;
  float lp[4];
      
  maxAngles = max(max(abs(arrt[0] - arrf[0]), abs(arrt[1] - arrf[1])), abs(arrt[2] - arrf[2]));
  maxAngles = maxAngles < 1 ? 1 : maxAngles;  

  for (int i = 0; i < 3; i++){    
    lp[i] = float(arrt[i] - arrf[i])/float(maxAngles);
  }
  
  for (int j = 1; j <= maxAngles; j++){
    for (int i = 0; i < 3; i++){      
      myservo[i].write(arrf[i]+j*lp[i]);      
    }
    delay(20);   
    SoftwareServo::refresh();    
  } 

  // store to value as from
  for (int i = 0; i < 3; i++){ 
    from[i] = to[i];
    myservo[i].write(to[i]);
  }
  delay(20);   
  SoftwareServo::refresh();  
}

void MeArm::gripperOpen(boolean b){
  int d = 25;
  if (b) {
    myservo[3].write(OPEN);
    delay(d);
    SoftwareServo::refresh();
    myservo[3].write(OPEN);
    delay(d);
    SoftwareServo::refresh();
  } else {
    myservo[3].write(CLOSE);
    delay(d);
    SoftwareServo::refresh();
    myservo[3].write(CLOSE);
    delay(d);
    SoftwareServo::refresh();
  }
}

void MeArm::angleTo(int a0, int a1, int a2, int b, int p, int d) {
  int delayTime = 200;

  delayfresh = d; // delay seconds after move finished

  to[0] = a0;
  armfromto(from, to);  
  delayWithRefresh(delayTime);

  to[1] = (a1+org[1])/2;
  to[2] = (a2+org[2])/2;
  armfromto(from, to); 
  delayWithRefresh(delayTime);	

  to[2] = a2;
  armfromto(from, to); 
  delayWithRefresh(delayTime);

  to[1] = a1;
  armfromto(from, to); 
  delayWithRefresh(delayTime);

  // gripper open/close
  if (b) gripperOpen(true);
  else gripperOpen(false);

  delayWithRefresh(delayTime);  

  to[1] = (a1+org[1])/2;
  armfromto(from, to); 
  delayWithRefresh(delayTime);	
  
  to[1] = org[1];
  to[2] = org[2];
  armfromto(from, to);   
  delayWithRefresh(delayTime);

  to[0] = org[0];
  armfromto(from, to);  

  //actions
  actions(p);

  delayWithRefresh(delayfresh*1000);
}

void MeArm::delayWithRefresh(int t){
  int lp = t/25;
  for(int i = 0; i < lp; i++){    
    SoftwareServo::refresh();
    delay(25);
  }
  SoftwareServo::refresh();
}

// 1/2/3 : shake motor 11/10/9
void MeArm::actions(int a){
  int a1, a2,s ;
  s = a - 1;

  if (a != 1 && a != 2 && a != 3 ) return;
  a1 = myservo[s].read();
  a2 = a1 - 15;
  for (int i = 0; i < 5; i++){
    myservo[s].write(a2);
    delayWithRefresh(200);
    myservo[s].write(a1);
    delayWithRefresh(200);
  }
}

