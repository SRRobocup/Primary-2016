#include "Global.h"
#include "mindsensors-lightsensorarray.h"
const int LEFTBLACK = 100;
const int LEFTGREEN = 200;
const int MIDDLEBLACK = 10;
const int RIGHTBLACK = 1;
const int RIGHTGREEN = 2;
const int NONE = 0;
unsigned int lastState = 0;
unsigned int lastTurn = 0;

unsigned int getState()
{
	int x = ((int)getColor(Leye) * 100) + ((int)getColor(Meye) * 10) + (int)getColor(Reye);
	return x;
}

void twoEye()
{
	int green = -forward; //-17
	unsigned int state = getState();
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
		if(getDigit(state,10) == cGradient)
		{
			temp = getDigit(state,100) * 100 + getDigit(state,1);
			state = temp;
		}
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
		while(nMotorEncoder[LMotor] > -10*cm){ if(seeBlack()){playSound(soundBlip); return;}}
		playSound(soundBeepBeep);
		stopMotors();
		motor[LMotor] = -straight;
		motor[RMotor] = -straight;
		while(nMotorEncoder[LMotor] < 0){}
		stopMotors();
		float currErr;
		float kp = 0.5;
		int tp = 10;
		long lastTime;
		//repeat(3)
		//{
		//	goBack(5);
		//	lastTime = nSysTime;
		//	while(nSysTime-lastTime < 1500))
		//	{
		//		currErr = (Meye.mthreshold - SensorValue[Meye.port])*kp;
		//		motor[LMotor] = tp - currErr;
		//		motor[RMotor] = tp + currErr;
		//	}
		//}
		short arr[8];
		for (short i = 0; i < 7; i+=1) {
			//displayTextLine(i/2+3, "S: %3d S: %3d", (short)signalstr[i], (short)signalstr[i+1]);
			if ((short) &signalstr[i]<50)
			arr[i]=1;


	}





		stopMotors();
		motor[LMotor] = straight;
		motor[RMotor] = straight;
		while(getColor(Leye) != cWhite || getColor(Reye) != cWhite){}
		while(getColor(Leye) == cWhite && getColor(Reye) == cWhite){}
		break;
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
		goStraight(3);
		resetEncoders();
		motor[RMotor] = green;
		motor[LMotor] = forward;
		while(getColor(Meye) != cWhite){}
		while(getColor(Meye) == cWhite && nMotorEncoder[RMotor] < 23.4*PI/4*cm){}
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
		goStraight(3);
		resetEncoders();
		motor[RMotor] = forward;
		motor[LMotor] =	green;
		while(getColor(Meye) != cWhite){}
		while(getColor(Meye) == cWhite && nMotorEncoder[LMotor] < 23.4*PI/4*cm){}
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
