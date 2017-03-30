/* MeArmIK library York Hack Space May 2014
 * A simple control library for Phenoptix' MeArmIK
 * Usage:
 *   MeArmIK arm;
 *   arm.begin(1, 10, 9, 6);
 *   arm.openGripper();
 *   arm.gotoPoint(-80, 100, 140);
 *   arm.closeGripper();
 *   arm.gotoPoint(70, 200, 10);
 *   arm.openGripper();
 */
// Modified by Ted Lien June.2015

#include <Arduino.h>
#include "ik.h"
#include "MeArmIK.h"

bool setup_servo (ServoInfo& svo, const int n_min, const int n_max,
                  const float a_min, const float a_max)
{
    float n_range = n_max - n_min;
    float a_range = a_max - a_min;

    // Must have a non-zero angle range
    if(a_range == 0) return false;

    // Calculate gain and zero
    svo.gain = n_range / a_range;
    svo.zero = n_min - svo.gain * a_min;

    // Set limits
    svo.n_min = n_min;
    svo.n_max = n_max;

    return true;
}

//Full constructor with calibration data
MeArmIK::MeArmIK(int sweepMinBase, int sweepMaxBase, float angleMinBase, float angleMaxBase,
          int sweepMinShoulder, int sweepMaxShoulder, float angleMinShoulder, float angleMaxShoulder,
          int sweepMinElbow, int sweepMaxElbow, float angleMinElbow, float angleMaxElbow,
          int sweepMinGripper, int sweepMaxGripper, float angleMinGripper, float angleMaxGripper) {
  //calroutine();
  setup_servo(_svoBase, sweepMinBase, sweepMaxBase, angleMinBase, angleMaxBase);
  setup_servo(_svoShoulder, sweepMinShoulder, sweepMaxShoulder, angleMinShoulder, angleMaxShoulder);
  setup_servo(_svoElbow, sweepMinElbow, sweepMaxElbow, angleMinElbow, angleMaxElbow);
  setup_servo(_svoGripper, sweepMinGripper, sweepMaxGripper, angleMinGripper, angleMaxGripper);
}

void MeArmIK::begin(int pinBase, int pinShoulder, int pinElbow, int pinGripper, float adj_x, float adj_y) {
  _pinBase = pinBase;
  _pinShoulder = pinShoulder;
  _pinElbow = pinElbow;
  _pinGripper = pinGripper;
  _base.attach(_pinBase);
  _shoulder.attach(_pinShoulder);
  _elbow.attach(_pinElbow);
  _gripper.attach(_pinGripper);

  x_shoulder = 0; 
  y_elbow = 0;
  servo_speed = 3;
  
  x_shoulder = adj_x; 
  y_elbow = adj_y;
  goDirectlyTo(0, 150, 136);
  delayWithRefresh(100);
  openGripper();
}

//Set servos to reach a certain point directly without caring how we get there 
void MeArmIK::goDirectlyTo(float x, float y, float z) {
  float radBase,radShoulder,radElbow;
  if (solve(x, y, z, radBase, radShoulder, radElbow)) {

    int a0 = int(servodefaultangle[0] - radBase*180/pi );
    int a1 = 180 - int(radShoulder*180/pi + servodefaultangle[1]);
    int a2 = int(radElbow*180/pi + servodefaultangle[2]);

    _base.write(a0);
    _shoulder.write(a1 + int(x_shoulder));
    _elbow.write(a2 + int(y_elbow));

    _shoulderDeltaAngle = a1 - 90;
    _elbowDeltaAngle = a2 - 135;

    _x = x; _y = y; _z = z;
  }    
}

//Travel smoothly from current point to another point
void MeArmIK::gotoPoint(float x, float y, float z, int gr, int sp, float t) {
  setSpeed(sp);
  //Starting points - current pos
  float x0 = _x; 
  float y0 = _y; 
  float z0 = _z;
  float dist = sqrt((x0-x)*(x0-x)+(y0-y)*(y0-y)+(z0-z)*(z0-z));
  int step = servo_speed;
  
  step = step > 10 ? 10 : step;
  step = step <  1 ?  1 : step;

  for (int i = 0; i<dist; i+= step) {
    goDirectlyTo(x0 + (x-x0)*i/dist, y0 + (y-y0) * i/dist, z0 + (z-z0) * i/dist);
    delay(10);
    SoftwareServo::refresh();
  }
  delayWithRefresh(500);
  if (gr == 1) openGripper();
  else closeGripper(); 
  //goDirectlyTo(x, y, z);
  delayWithRefresh(t*1000); 
}

//Check to see if possible
bool MeArmIK::isReachable(float x, float y, float z) {
  float radBase,radShoulder,radElbow;
  return (solve(x, y, z, radBase, radShoulder, radElbow));
}

//Grab something
void MeArmIK::openGripper() {
  _gripper.write(30);
  delayWithRefresh(100);
}

//Let go of something
void MeArmIK::closeGripper() {
  _gripper.write(0);
  delayWithRefresh(100);
}

//Adjust angles of shoulder/elbow servo for the sake of installation difference
void MeArmIK::adjustAngles(float x, float y){
  x_shoulder = x; 
  y_elbow = y;
  goDirectlyTo(0, 150, 136);
}

//Set speed of moving
//i : 1~9, slowest/fastest
void MeArmIK::setSpeed(int i){
  servo_speed = i;
}

//Show 2 angles for adjustAngles(float x, float y);
void MeArmIK::showAdjustmentsToSerial(){
  Serial.print("adjustAngles(");
  Serial.print(_shoulderDeltaAngle);
  Serial.print(" , ");
  Serial.print(_elbowDeltaAngle);
  Serial.println(");");

}

//
void MeArmIK::delayWithRefresh(int t){
  int lp = t/25;
  for(int i = 0; i < lp; i++){    
    SoftwareServo::refresh();
    delay(25);
  }
  SoftwareServo::refresh();
}

//Current x, y and z
float MeArmIK::getX() {
  return _x;
}
float MeArmIK::getY() {
  return _y;
}
float MeArmIK::getZ() {
  return _z;
}
