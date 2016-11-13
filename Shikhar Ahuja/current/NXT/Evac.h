#include "Global.h"
float dx;
float dy;
byte tripos;
byte pos;
byte origin;
int dist;
const int lpower = 20;
const int rpower = 20;
const int buffer = 6;
int degrees=8175;
void openSweeper()
{
	nMotorEncoder[Sweeper] = 0;
	motor[Sweeper] = -100;
	while(nMotorEncoder[Sweeper] > -degrees){}
	motor[Sweeper] = 0;
}

/*
dy
4---3
|   | dx
|   |
1---2
*/

void PID()
{
	float kp = 14.1;
	float target = 6;
	int tp = 60;
	float err;
	while (getDistance(Msonar) > 6)
	{
		err = (getDistance(Lsonar) - target) * kp;
		motor[LMotor] = tp - err;
		motor[RMotor] = tp + err;
	}
}

void turnSweep()
{
	motor[Sweeper] = 100;
	turnLeft(20);
	turnLeft(25);
	turnLeft(10);
	goStraight(5);
}

void evac()
{
	openSweeper();
	resetEncoders();
	motor[LMotor] = 20;
	motor[RMotor] = 20;
	while(nMotorEncoder[LMotor] > (dist-7) * cm * -1){}
	if(getBoolColor(Meye) != cBlack)
	{
		turnRight(90);
		motor[LMotor] = 45;
		motor[RMotor] = 46;
		while(getBoolColor(Meye) != cBlack){}
	}
	stopMotors();
	goBack(5);
	turnSweep();
}
