#include "Global.h"
const int LEFTBLACK = 100;
const int LEFTGREEN = 200;
const int MIDDLEBLACK = 10;
const int RIGHTBLACK = 1;
const int RIGHTGREEN = 2;
const int NONE = 0;
unsigned int lastState = 0;

task Wobble()
{
	goStraight(5);
	turnRight(90);
	int result = wheelbase * PI * cm * 3;
	while (true)
	{
		resetEncoders();
		motor[LMotor] = 0;
		motor[RMotor] = forward;
		while (nMotorEncoder[RMotor] < result){}
		resetEncoders();
		motor[LMotor] = forward;
		motor[RMotor] = 0;
		while (nMotorEncoder[LMotor] < result){}
	}
}

unsigned int getState()
{
	int x = ((int)getColor(Leye) * 100) + ((int)getColor(Meye) * 10) + (int)getColor(Reye);
	return x;
}
bool twoEye()
{
	int straight = 30;
	int green = -17;
	unsigned int state = getState();
	if(getDigit(state,1) == RIGHTGREEN)
		state = RIGHTGREEN;
	if(getDigit(state,100) == LEFTGREEN)
		state = LEFTGREEN;
	if(state != 303){
		if(getDigit(state,1) == 3)
			if (lastState == RIGHTBLACK)
				state = RIGHTBLACK;
			else
				state = getDigit(state,100) * 100 + getDigit(state,10) * 10;
		if(getDigit(state,100) == 3)
			if (lastState == LEFTBLACK)
				state = LEFTBLACK;
			else
				state = getDigit(state,10) * 10 + getDigit(state,1);
	}
	switch (state)
	{
	case NONE:
		println(0, "State: GAP");
		startTask(Wobble);
		while (getColor(Leye) == cWhite && getColor(Meye) == cWhite && getColor(Reye) == cWhite){}
		stopTask(Wobble);
		break;
	case RIGHTGREEN:
		println(0, "State: RIGHT GREEN");
		playSound(soundBlip);
		motor[RMotor] = green;
		motor[LMotor] = forward;
		while(getColor(Meye)){}
		while(!getColor(Meye)){}
		motor[RMotor] = straight;
		motor[LMotor] = straight;
		while(getColor(Reye) == cGreen){}
		break;
	case LEFTGREEN:
		println(0, "State: LEFT GREEN");
		motor[RMotor] = forward;
		motor[LMotor] = green;
		while(getColor(Meye)){}
		while(!getColor(Meye)){}
		motor[RMotor] = straight;
		motor[LMotor] = straight;
		while(getColor(Leye) == cGreen){}
		break;
	case LEFTBLACK:
		println(0, "State: LEFT BLACK");
		motor[LMotor] = back;
		motor[RMotor] = forward;
		break;
	case RIGHTBLACK:
		println(0, "State: RIGHT BLACK");
		motor[LMotor] = forward;
		motor[RMotor] = back;
		break;
	default:
		println(0, "State: STRAIGHT");
		motor[LMotor] = straight;
		motor[RMotor] = straight;
		break;
	}
	lastState = state;
	return false;
}
