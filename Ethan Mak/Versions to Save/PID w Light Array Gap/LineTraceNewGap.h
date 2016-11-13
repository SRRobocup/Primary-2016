#include "Global.h"
const int LEFTBLACK = 100;
const int LEFTGREEN = 200;
const int MIDDLEBLACK = 10;
const int RIGHTBLACK = 1;
const int RIGHTGREEN = 2;
const int NONE = 0;
unsigned int lastState = 0;
unsigned int lastTurn = 0;
const unsigned int upperLimit = 60;
const unsigned int lowerLimit = 30;

unsigned int getState()
{
	int x = ((int)getColor(Leye) * 100) + ((int)getColor(Meye) * 10) + (int)getColor(Reye);
	return x;
}

int wrap(ubyte val)
{
	if(val > upperLimit)
		return upperLimit;
	if(val > lowerLimit)
		return lowerLimit;
	return val;
}

void twoEye()
{
	int fast = 40;
	int green = -forward; //-17
	int state = getState();
	println(5,"%d",state);
	int temp;
	if (state != 303 && state != 313)
	{
		if(getDigit(state,1) == cGradient)
			if (lastState == RIGHTBLACK)
			state = RIGHTBLACK;
		else
		{
			temp = getDigit(state,100) * 100 + getDigit(state,10) * 10;
			state = temp == 0 ? getDigit(state,100) * 100 + getDigit(state,10) * 10 + RIGHTBLACK: temp;
		}
		//if(getDigit(state,10) == cGradient)
		//{
		//	temp = getDigit(state,100) * 100 + getDigit(state,1);
		//	state = temp;
		//}
		if(getDigit(state,100) == cGradient)
			if (lastState == LEFTBLACK)
			state = LEFTBLACK;
		else
		{
			temp = getDigit(state,10) * 10 + getDigit(state,1);
			state = temp == 0 ? LEFTBLACK + getDigit(state,10) * 10 + getDigit(state,1) : temp;
		}
	}
	switch (state)
	{
	case NONE:
		println(0, "State: GAP");
		resetEncoders();
		motor[LMotor] = straight;
		motor[RMotor] = straight;
		while(nMotorEncoder[LMotor] < 10*cm){ if(seeBlack()){playSound(soundBlip); return;}}
		playSound(soundBeepBeep);
		stopMotors();
		SensorType[Leye.port] = sensorNone;
		SensorType[Reye.port] = sensorNone;
		wait1Msec(100);
		motor[LMotor] = -straight;
		motor[RMotor] = -straight;
		while(!seeBlackArray()){writeDebugStreamLine("loop");}
		stopMotors();
		motor[LMotor] = straight;
		motor[RMotor] = straight;
		while(seeBlackArray()){writeDebugStreamLine("loop");}
		stopMotors();
		float curr;
		int tp = 40;
		float kp = 0.015;
		long lastTime;
		ubyte values[8];
		repeat(1)
		{
			//goBack(4);
			lastTime = nSysTime;
			while(seeBlackArray())
			{
				MSLSAreadSensors(splitter,values);
				curr = (upperLimit - wrap(values[0])) * 100 +
					 (upperLimit - wrap(values[1])) * 50 +
					 (upperLimit - wrap(values[2])) * 10 +
					 (upperLimit - wrap(values[3])) * 1 +
					 (upperLimit - wrap(values[4])) * -1 +
					 (upperLimit - wrap(values[5])) * -10 +
					 (upperLimit - wrap(values[6])) * -50 +
					 (upperLimit - wrap(values[7])) * -100;
				motor[LMotor] = tp + curr*kp;
				motor[RMotor] = tp - curr*kp;
				wait1Msec(50);
			}
		}
		stopMotors();
		SensorType[Leye.port] = sensorColorNxtFULL;
		SensorType[Reye.port] = sensorColorNxtFULL;
		wait1Msec(100);
		motor[LMotor] = straight;
		motor[RMotor] = straight;
		while(seeBlackArray()){}
		while(getColor(Leye) != cWhite || getColor(Reye) != cWhite){}
		while(getColor(Leye) == cWhite && getColor(Reye) == cWhite){}
		break;
	//case RIGHTBLACK + LEFTBLACK:
	//	if(lastTurn == RIGHTBLACK)
	//	{
	//		setMotorSyncEncoder(LMotor, RMotor, -100, wheelbase*PI/4*cm, forward);
	//		while(stillRunning(LMotor) || stillRunning(RMotor))
	//		{

	//		}
	//		stopMotors();
	//		resetSync();
	//	}
	//	break;
	case RIGHTGREEN + MIDDLEBLACK:
	case RIGHTGREEN:
		println(0, "State: RIGHT GREEN");
		playSound(soundBlip);
		motor[LMotor] = straight;
		motor[RMotor] = straight;
		while(getColor(Reye) != cWhite && getColor(Reye) != cBlack){}
		playSound(soundBlip);
		if (getColor(Reye) == cWhite)
		{
			playSound(soundBeepBeep);
			return;
		}
		goStraight(6.3);
		resetEncoders();
		motor[RMotor] = green;
		motor[LMotor] = forward;
		while(getColor(Meye) != cWhite){}
		while(getColor(Meye) == cWhite && nMotorEncoder[RMotor] < wheelbase*PI/4*cm){}
		stopMotors();
		playSound(soundBlip);
		motor[RMotor] = straight;
		motor[LMotor] = straight;
		while(getColor(Reye) == cGreen){}
		break;
	case LEFTGREEN + MIDDLEBLACK:
	case LEFTGREEN:
		println(0, "State: LEFT GREEN");
		playSound(soundBlip);
		motor[LMotor] = straight;
		motor[RMotor] = straight;
		while(getColor(Leye) != cWhite && getColor(Leye) != cBlack){}
		playSound(soundBlip);
		if (getColor(Leye) == cWhite)
		{
			playSound(soundBeepBeep);
			return;
		}
		goStraight(6.3);
		resetEncoders();
		motor[RMotor] = forward;
		motor[LMotor] =	green;
		while(getColor(Meye) != cWhite){}
		while(getColor(Meye) == cWhite && nMotorEncoder[LMotor] < wheelbase*PI/4*cm){}
		stopMotors();
		playSound(soundBlip);
		motor[RMotor] = straight;
		motor[LMotor] = straight;
		while(getColor(Leye) == cGreen){}
		break;
	case LEFTBLACK:
		println(0, "State: LEFT BLACK");
		motor[LMotor] = back;
		motor[RMotor] = forward;
		lastTurn = LEFTBLACK;
		break;
	case RIGHTBLACK:
		println(0, "State: RIGHT BLACK");
		playSound(soundBlip);
		motor[LMotor] = forward;
		motor[RMotor] = back;
		lastTurn = RIGHTBLACK;
		break;
	default:
		println(0, "State: STRAIGHT");
		motor[LMotor] = straight;
		motor[RMotor] = straight;
		break;
	}
	lastState = state;
}
