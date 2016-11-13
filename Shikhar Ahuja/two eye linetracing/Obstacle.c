#include "Global.h"
#include "FuncLib.c"

void obstacle()
{
	float obstDia=10, inner=obstDia+(distObstacle*2), outer=inner+(wheelbase*2);
	float ratio= outer/inner;
	int direction = -1; // 1 <= left, -1 => right
	stopMotors();
	playSound(soundBlip);
	wait1Msec(1000);
	//Checking which side
	//{
	//	turnLeft(90);
	//	float distL = getDistance(Ultrasonic);
	//	turnRight(180);
	//	float distR = getDistance(Ultrasonic);
	//	direction = distL < distR ? 1 : -1;
	//	turnLeft(90);
	//}

	playSound(soundBeepBeep);
	wait1Msec(100);

	if(direction == 1)
	{
		turnLeft(90);
		motor[LMotor] = tpower * ratio;
		motor[RMotor] = tpower;
		displayCenteredBigTextLine(2,"L: %d",motor[LMotor]);
		displayCenteredBigTextLine(4,"R: %d",motor[RMotor]);
		while (SensorValue[Reye] > rthreshold){}
		stopMotors();
		turnLeft(90);
	}
	else
	{
		turnRight(90);
		motor[LMotor] = tpower;
		motor[RMotor] = tpower * ratio;
		displayCenteredBigTextLine(2,"L: %d",motor[LMotor]);
		displayCenteredBigTextLine(4,"R: %d",motor[RMotor]);
		while (SensorValue[Leye] > lthreshold){}
		stopMotors();
		turnLeft(90);
	}
}
