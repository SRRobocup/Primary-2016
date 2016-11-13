#include "Global.h"

void obstacle()
{
	//float obstDia=8.5, inner=obstDia+(distObstacle*2), outer=inner+(wheelbase*2);
	//float ratio= outer/inner;
	int direction = 1; // 1 <= left, -1 => right
	stopMotors();
	playSound(soundBlip);
	wait1Msec(1000);
	//Checking which side
	{
		rotateLeft(90);
		float distL = getUSDistance(Ultrasonic);
		wait1Msec(100);
		rotateRight(180);
		float distR = getUSDistance(Ultrasonic);
		wait1Msec(100);
	direction = distL > distR ? 1 : -1;
	}
	float error;
	float target;
	float buffer = 4, kp = 4.1;
	if(direction == 1)
	{
		playSound(soundBeepBeep);
		rotateLeft(45);
		turnLeft(90);
		goStraight(3);
		target = 5 + buffer;
		setLEDColor(ledRedFlash);
		while (getColor(Reye) == cWhite)
		{
#ifdef DEBUG
			displayTextLine(1, "Error: %f", error);
			displayTextLine(2, "LMotor: %f", forward - error);
			displayTextLine(3, "RMotor: %f", forward + error);
#endif
			error = (target - getUSDistance(Ultrasonic)) * kp;
			motor[LMotor] = forward - error;
			motor[RMotor] = forward + error;
		}
		rotateLeft(45);
		setLEDColor(ledOrangeFlash);
		while(!getColor(Meye))
		{
			motor[LMotor] = -forward;
			motor[RMotor] = forward;
		}
	}
	else
	{
		playSound(soundDownwardTones);
		turnRight(90);
		goStraight(3);
		rotateLeft(135);
		target = 3.5 + buffer;
		setLEDColor(ledRedFlash);
		while (getColor(Leye) == cWhite)
		{
#ifdef DEBUG
			displayTextLine(2, "Error: %f", error);
			displayTextLine(3, "LMotor: %f", forward + error);
			displayTextLine(4, "RMotor: %f", forward - error);
#endif
			error = (target - getUSDistance(Ultrasonic)) * kp;
			motor[LMotor] = forward + error;
			motor[RMotor] = forward - error;
		}

		rotateRight(45);
		setLEDColor(ledOrangeFlash);
		while(!getColor(Meye))
		{
			motor[LMotor] = forward;
			motor[RMotor] = -forward;
		}
	}
	playSound(soundDownwardTones);
}


void evacuation()
{
	resetEncoders();
}
