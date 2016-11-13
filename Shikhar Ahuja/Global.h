#ifndef __GLOBAL.H__
#define __GLOBAL.H__
#define println(x, y, z) (displayTextLine(x,y,z))

typedef enum Color {
cBlack,
cSilver,
cGreen,
cWhite
} Color;
typedef tSensors SensorPort;
typedef tMotor MotorPort;

const SensorPort Leye = S2;
const float lwhite=78 , lblack=44, lthreshold = (lwhite + lblack)/2;
const SensorPort Reye = S1;
const float rwhite=78, rblack=44, rthreshold = (rwhite + rblack)/2, silver = 100, sthreshold = (silver + rwhite)/2;
const SensorPort Ultrasonic = S2;
//const MotorPort LMotor = motorA;
//const MotorPort RMotor = motorC;
const float wheelbase = 14;
const float diameter = 3;
const float cm = 360.0 *3.0/ (diameter *  PI);
const float tpower = 30;
const float distObstacle = 20;
// defining all of the global variables
#endif
