#include "common.h"
task main()
{
	while (true)
	{
		if (getXbuttonValue(xButtonLeft))
			motor[motorB] = 50;
		else if (getXbuttonValue(xButtonRight))
			motor[motorB] = -50;
		else
			motor[motorB] = 0;
		if (getXbuttonValue(xButtonEnter))
			nMotorEncoder[motorB] = 0;
		displayTextLine(3, "%d", nMotorEncoder[motorB]);
	}

}
