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

const SensorPort Leye = S3;
const float lwhite=3 , lblack=40, lthreshold = (lwhite + lblack)/2;
const SensorPort Reye = S1;
const float rwhite=3 , rblack=40, rthreshold = (rwhite + rblack)/2, silver = 100, sthreshold = (silver + rwhite)/2;
const SensorPort Meye = S2;
const float mwhite=3 , mblack=40, mthreshold = (mwhite + mblack)/2;
const SensorPort Ultrasonic = S4;
const MotorPort LMotor = motorC;
const MotorPort RMotor = motorA;
const float wheelbase = 14;
const float diameter = 3;
const float cm = 360.0 / (diameter *  PI);
const float tpower = 20;
const float distObstacle = 20;
const float green;
const float (green+black)/2
// defining all of the global variables
#endif
