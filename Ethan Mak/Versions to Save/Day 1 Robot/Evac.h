#include "Global.h"
short tripos;
short pos;
short origin;
float dist;
int degrees = 9512; //closing 9241
int buffer = 5;
int run = 1;
/**
* Closes Sweeper
*/
void closeSweeper()
{
	nMotorEncoder[Sweeper] = 0;
	motor[Sweeper] = -100;
	while(nMotorEncoder[Sweeper] > -degrees){}
	motor[Sweeper] = 0;
}

/**
* Opens Sweeper
*/
void openSweeper()
{
	nMotorEncoder[Sweeper] = 0;
	motor[Sweeper] = 100;
	while(nMotorEncoder[Sweeper] < degrees){}
	motor[Sweeper] = 0;
}

/*
dy
4---3
|   | dx
|   |
1---2
*/
float prev = 1000;

/**
* Wall traces in the evac room based on two left PINGs
*/
void PID()
{
	int tp = 50;
	playSound(soundBlip);
	float kp = 3.1;
	float kp2 = 5.1;
	float kd = 3.1;
	float target = 5;
	float err = 0, curr, err2, curr2;
	curr = getDistance(Lsonar);
	if(curr - prev > 2)
		curr = 2;
	curr2 = getDistance(Lsonar2);
	err = (curr - target) * kp;
	err2 = (curr - curr2) * kp2;
	motor[LMotor] = tp - err - err2;
	motor[RMotor] = tp + err + err2;
	//writeDebugStreamLine("%.2f %.2f",err,curr);
	//writeDebugStreamLine("%d %d",tp - err,tp + err);
	//writeDebugStreamLine("%d %d",motor[LMotor],motor[RMotor]);
}

/**
* Alligns with wall based on difference between two left sensors
*/
void allign(int power = 20, float tolerance = 1)
{
	while(abs(getDistance(Lsonar) - getDistance(Lsonar2)) > tolerance)
	{
		motor[LMotor] = power;
		motor[RMotor] = -power;
	}
	stopMotors();
}

void allignFull(int power = 20,float tolerance = 1)
{
	int dir = getDistance(Lsonar) > getDistance(Lsonar2) ? -1 : 1;
	while(abs(getDistance(Lsonar) - getDistance(Lsonar2)) > tolerance)
	{
		motor[LMotor] = power * dir;
		motor[RMotor] = -power * dir;
	}
	stopMotors();
}

/**
* Gets Distance from middle PING with data filtering
*/
float getMedianDistance(byte address)
{
	float val[5];
	for(int i = 0; i < 5; i++)
	{
		val[i] = getDistance(address);
		wait1Msec(3);
	}
	float temp;
	for (int i = 1; i < 5; i++)
	{
		for(int j = i ; j > 0 ; j--)
		{
			if(val[j] < val[j-1])
			{
				temp = val[j];
				val[j] = val[j-1];
				val[j-1] = temp;
			}
		}
  }
  return val[2];
}

/**
* Updates last corner of robot in evace room
*/
void updatePos()
{
	pos -= 1;
	if (pos <= 0)
		pos = 4;
}

/**
* Sweeps the balls into the triangle while retracting the arm and turning
*/
void turnSweep()
{
	nMotorEncoder[Sweeper] = 0;
	motor[Sweeper] = -100;
	turnLeft(40);
	while(nMotorEncoder[Sweeper] > -3500){}
	turnLeft(20);
	while(nMotorEncoder[Sweeper] > -5500){}
	turnLeft(20);
	while(nMotorEncoder[Sweeper] > -7500){}
	turnLeft(20);
	while(nMotorEncoder[Sweeper] > -9512){}
	motor[Sweeper] = 0;
	//turnRight(90);
	//goStraight(10);
	//turnLeft(25);
	//turnRight(25);
	//goStraight(10);
	//turnLeft(45);
	//turnRight(45);
	//goStraight(10);
	//turnLeft(90);
}

/**
* Full Evacuation room procedure
*/
void evac()
{
	int tp = 60;
	eraseDisplay();
	if(getDistance(Lsonar) < 10)
		allignFull();
	resetEncoders();
	if(getDistance(Rsonar) > getDistance(Lsonar))
	{
		pos = 1;
		dist = getDistance(Rsonar) + width - length;
	}
	else
	{
		pos = 2;
		dist = getDistance(Msonar);
		turnLeft(90);
	}
	origin = pos;
	println(3,"POS: %d" ,pos);
	//wait1Msec(30000);
	openSweeper();
	goStraight(length + 10);
	while(!(getColor(Meye) == cBlack))//&& run > 2
	{
		writeDebugStreamLine("%.3f",getDistance(Msonar));
		if(getColor(Meye) == cBlack)
			break;
		println(3,"POS: %d" ,pos);
		PID();
		if (getDistance(Msonar) <= buffer && getMedianDistance(Msonar) <= buffer)
		{
			playSound(soundBeepBeep);
			turnRight(90);
			allign();
			updatePos();
			run++;
			if(run == 4)
			{
				allign();
				resetEncoders();
				while(nMotorEncoder[LMotor] < dist*cm)
					PID();
				turnRight(90);
				goStraight(length + 10, tp);
				allign();
			}
		}
		wait1Msec(50);
	}
	stopMotors();
	wait1Msec(100);
	goStraight(20);
	goBack(20);
	turnSweep();
	//closeSweeper();
	stopMotors();
}
