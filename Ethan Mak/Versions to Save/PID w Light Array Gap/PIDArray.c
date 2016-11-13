#include "mindsensors-lightsensorarray.h"

unsigned int upperLimit = 75;
unsigned int lowerLimit = 30;

int wrap(ubyte val)
{
	if(val > upperLimit)
		return upperLimit;
	if(val > lowerLimit)
		return lowerLimit;
	return val;
}
task main()
{
	tSensors MSLSA = S2;
	SensorType[MSLSA] = sensorI2CCustom;
	MSLSAinit(MSLSA);
	ubyte values[8];
	float curr;
	int tp = 60;
	float kp = 0.0035;
	bMotorReflected[motorA] = true;
	bMotorReflected[motorC] = true;
	while (true)
	{
		MSLSAreadSensors(MSLSA,values);
		curr = (upperLimit - wrap(values[0])) * 500 +
					 (upperLimit - wrap(values[1])) * 100 +
					 (upperLimit - wrap(values[2])) * 10 +
					 (upperLimit - wrap(values[3])) * 1 +
					 (upperLimit - wrap(values[4])) * -1 +
					 (upperLimit - wrap(values[5])) * -10 +
					 (upperLimit - wrap(values[6])) * -100 +
					 (upperLimit - wrap(values[7])) * -500;
		motor[motorA] = tp - curr*kp;
		motor[motorC] = tp + curr*kp;
		displayTextLine(3, "%d",curr);
		writeDebugStreamLine("%d %d %d", curr, motor[motorA],motor[motorC]);
		wait1Msec(50);
	}
}
