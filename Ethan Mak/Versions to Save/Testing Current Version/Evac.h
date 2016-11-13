#include "Global.h"
short tripos;
short pos;
short origin;
float dist;
int degrees = 8093; //closing 9241
int buffer = 5;
int run = 1;
int tp = 60;
//void closeSweeper()
//{
//	nMotorEncoder[Sweeper] = 0;
//	motor[Sweeper] = -100;
//	while(nMotorEncoder[Sweeper] > -degrees){}
//	motor[Sweeper] = 0;
//}

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
void PID()
{
	playSound(soundBlip);
	//float kp = 8.1;
	float kp2 = 4.1;
	//float kd = 3.1;
	float target = 5;
	float err = 0, curr, err2, curr2;
	curr = getDistance(Lsonar);
	if(curr - prev > 2)
		curr = 2;
	curr2 = getDistance(Lsonar2);
	//err = (curr - target) * kp;
	err2 = (curr - curr2) * kp2;
	motor[LMotor] = tp - err - err2;
	motor[RMotor] = tp + err + err2;
	//writeDebugStreamLine("%.2f %.2f",err,curr);
	//writeDebugStreamLine("%d %d",tp - err,tp + err);
	//writeDebugStreamLine("%d %d",motor[LMotor],motor[RMotor]);
}

void allign(int power = 20)
{
	int dir = getDistance(Lsonar) > getDistance(Lsonar2) ? -1 : 1;
	while(abs(getDistance(Lsonar) - getDistance(Lsonar2)) > 0.9)
	{
		motor[LMotor] = power * dir;
		motor[RMotor] = -power * dir;
	}
	stopMotors();
}

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

void updatePos()
{
	pos -= 1;
	if (pos <= 0)
		pos = 4;
}

void turnSweep()
{
	nMotorEncoder[Sweeper] = 0;
	motor[Sweeper] = -100;
	turnLeft(40);
	while(nMotorEncoder[Sweeper] > -2000){}
	turnLeft(20);
	while(nMotorEncoder[Sweeper] > -4000){}
	turnLeft(20);
	while(nMotorEncoder[Sweeper] > -6000){}
	turnLeft(20);
	while(nMotorEncoder[Sweeper] > -8096){}
	motor[Sweeper] = 0;
}

void evac()
{
	eraseDisplay();
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
	while(!(getColor(Meye) == cBlack && run > 2))
	{
		writeDebugStreamLine("%.3f",getDistance(Msonar));
		if(getColor(Meye) == cBlack)
			tripos = pos;
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
	turnSweep();
	//closeSweeper();
	stopMotors();
}
