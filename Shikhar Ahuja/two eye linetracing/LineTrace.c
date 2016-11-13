#include "Global.h"
#include "FuncLib.c"
#define LEFT 4
#define MIDDLE 2
#define RIGHT 1
#define NONE 0

task Wobble()
{
	//the function crossing the gap
	while (true)
	{
		goStraight(4);
		turnRight(38);
		goStraight(4);
		turnLeft(38);
	}
}

unsigned byte getState()
{
	return (SensorValue[Leye] > lthreshold) << 2 | (SensorValue[Meye] > mthreshold) << 1 | (SensorValue[Reye] > rthreshold);
}

void twoEye()
{
	switch (getState())
	{
		//case LeftGreen;
	//turnright(90);
	//break;
	//case RightGreen;
	//turnleft(90);
	//break;

	case LEFT:
		motor[LMotor] = -40;
		motor[RMotor] = 60;
		break;
	case RIGHT:
		motor[LMotor] = 60;
		motor[RMotor] = -40;
		break;
	case NONE:
		startTask(Wobble);
		while(SensorValue[Meye] < mthreshold) {}
		break;
	default:
		motor[LMotor] = 40;
		motor[RMotor] = 40;
		break;
	}
}
