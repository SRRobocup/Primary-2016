#include "Global.h"
#include "FuncLib.c"

void obstacle()
{
	float obstDia=10, inner=obstDia+(distObstacle*2), outer=inner+(wheelbase*2);
	float ratio= outer/inner;
	int direction = -1; // 1 <= left, -1 => right
	// this is the initializing
	stopMotors();
	playSound(soundBlip);
	wait1Msec(1000);
	//Checking which side
	//{
	//	turnLeft(45);
	//	float distL = getDistance(Ultrasonic);
	//	turnRight(90);
	//	float distR = getDistance(Ultrasonic);
	//	direction = distL < distR ? 1 : -1;
	//	turnLeft(45);
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

	//if (SensorValue[Leye]>10)
	//{
	//	wait10Msec(10);
	//	motor[RMotor]=outerpow;
	//	motor[LMotor]=innerpow;
	//	while(SensorValue[Reye]>rthreshold)
	//	{
	//		displayCenteredBigTextLine(2,"L: %d",motor[LMotor]);
	//		displayCenteredBigTextLine(4,"R: %d",motor[RMotor]);
	//	}
	//	stopMotors();
	//	wait10Msec(10);
	//}
	//else
	//{
	//	playSound(soundFastUpwardTones);
	//	turnRight(180);
	//	wait1Msec(100);
	//	motor[LMotor]=outerpow;
	//	motor[RMotor]=innerpow;

	//	while(SensorValue[Reye]>rthreshold-10)
	//	{
	//		displayCenteredBigTextLine(2,"L: %d",motor[LMotor]);
	//		displayCenteredBigTextLine(4,"R: %d",motor[RMotor]);
	//	}

	//	stopMotors();
	//	wait1Msec(100);

	//	motor[LMotor]=0;
	//	motor[RMotor]=40;

	//	while(SensorValue[Reye]>rthreshold+8)
	//		wait1Msec(30);

	//	motor[LMotor]=0;
	//	motor[RMotor]=40;

	//	while(SensorValue[Reye]<rthreshold-8)
	//		wait1Msec(30);
	//}
}
