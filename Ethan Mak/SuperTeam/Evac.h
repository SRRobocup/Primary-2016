#include "Global.h"
short tripos;
short pos;
short origin;
float dist;
int buffer = 5;
int run = 1;
float thresh[] = {201.000000,294.666656,3.2};

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
	curr2 = getDistance(Lsonar2);
	err = (curr - target) * kp;
	err2 = (curr - curr2) * kp2;
	if(curr-prev > 2)
	{
		err = 0;
		err2 = 0;
	}
	else
		prev = curr;
	motor[LMotor] = tp - err - err2;
	motor[RMotor] = tp + err + err2;
	//writeDebugStreamLine("%.2f %.2f",err,curr);
	//writeDebugStreamLine("%d %d",tp - err,tp + err);
	//writeDebugStreamLine("%d %d",motor[LMotor],motor[RMotor]);
}

bool seeRed()
{
	int offset = Leye.port * 3;
	short data[4];
	getColorSensorData(Leye.port,colorRaw,&data[0]);
	if(data[0] > thresh[0] && data[0] < thresh[1])
	{
		float ratio = data[0]/data[1];
		if (ratio >= thresh[2])
			return true;
	}
	return false;
}

/**
* Alligns with wall based on difference between two left sensors
*/
void allign(int power = 20, float tolerance = 1)
{
	int begin = nPgmTime;
	while(abs(getDistance(Lsonar) - getDistance(Lsonar2)) > tolerance && nPgmTime - begin < 3000)
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
* Full Evacuation room procedure
*/
void evac()
{
	int tp = 60;
	eraseDisplay();
	if(getDistance(Lsonar) < 10)
		allignFull();
	//else
	//{
	//	int begin = nPgmTime;
	//	while(nPgmTime - begin < 2000)
	//		RightPID();
	//	motor[LMotor] = -straight;
	//	motor[RMotor] = -straight;
	//	while(getColor(Meye) != cSilver){}
	//	goStraight(length+9);
	//}
	resetEncoders();
	int rdist = getDistance(Rsonar);
	int ldist = getDistance(Lsonar);
	if (rdist + ldist + width < 54)
	{
		rdist = getDistance(Rsonar);
		ldist = getDistance(Lsonar);
	}
	if(abs(ldist-rdist) <= 5)
	{
		turnLeft(90);
		motor[LMotor] = straight;
		motor[RMotor] = straight;
		while(getDistance(Msonar) > 5){}
		if(getColor(Meye) == cBlack)
			suspend();
		turnRight(90);
		allign();
		pos = 1;
		dist = getDistance(Msonar);
	}
	else if(rdist > ldist)
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
	//float temp = getDistance(Msonar);
	goStraight(length+10);
	//int check = abs(90-temp-length) < 5 ? 120 : 90;
	//bool obstFound = false;
	//float obst = 1000;
	//while(getDistance(Msonar) > 5)
	//{
	//	PID();
	//	if (getDistance(Rsonar) < obst)
	//		obst = getDistance(Rsonar);
	//}
	//obst += width;
	//if(abs(obst-check) >= 8)
	//{
	//	writeDebugStreamLine("OBSTACLE IN EVAC WTF PLS NO %f", obst);
	//	obstFound = true;
	//}
	//suspend();
	//goStraight(length + 10);
	while(!seeRed())//&& run > 2
	{
		writeDebugStreamLine("%.3f",getDistance(Msonar));
		if(getColor(Meye) == cRed)
			break;
		println(3,"POS: %d" ,pos);
		PID();
		if (getDistance(Msonar) <= buffer && getMedianDistance(Msonar) <= buffer)
		{
			playSound(soundBeepBeep);
			turnRight(90,60);
			goStraight(16);
			if(getDistance(Lsonar) > 15)
			{
				turnLeft(90);
				goStraight(42);
				turnLeft(90);
				goStraight(16);
				turnRight(90);
			}
			run++;
		}
		wait1Msec(50);
	}
	stopMotors();
	wait1Msec(100);
	goStraight(15);
	resetEncoders();
	ldist = getDistance(Lsonar);
	int mdist = getDistance(Msonar);
	if (mdist > ldist)
	{
		turnLeft(90);
		resetEncoders();
		motor[LMotor] = 100;
		motor[RMotor] = 100;
		int begin = nPgmTime;
		while(nPgmTime-begin < 2500){}
		motor[LMotor] = -straight;
		motor[RMotor] = -straight;
		while(!seeRed()){}
		stopMotors();
		goBack(10);
		turnRight(90);
	}
	else
	{
		motor[LMotor] = 100;
		motor[RMotor] = 100;
		int begin = nPgmTime;
		while(nPgmTime-begin < 2500){}
		motor[LMotor] = -straight;
		motor[RMotor] = -straight;
		while(!seeRed()){}
		stopMotors();
		goBack(10);
		turnLeft(90);
	}
	goStraight(35);
	stopMotors();
}
