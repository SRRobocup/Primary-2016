#include "Global.h"
const int LEFTBLACK = 100;
const int LEFTGREEN = 200;
const int MIDDLEBLACK = 10;
const int RIGHTBLACK = 1;
const int RIGHTGREEN = 2;
const int NONE = 0;
unsigned int lastState = 0;
unsigned int lastTurn = 0;
float integral = 0;

unsigned int getState()
{
	int x = ((int)getColor(Leye) * 100) + ((int)getBoolColor(Meye) * 10) + (int)getColor(Reye);
	return x;
}

void twoEyePID(int greenL = -1,int greenR = -1)
{
	short rgbL[4], rgbR[4];
	if(greenL == -1)
	{
		getColorSensorData(Leye.port, colorRaw, &rgbL[0]);
		greenL = rgbL[1];
	}
	if(greenR == -1)
	{
		getColorSensorData(Reye.port, colorRaw, &rgbR[0]);
		greenR = rgbR[1];
	}
	float curr = greenL - greenR;
	if(abs(curr) < 10)
	{
		writeDebugStreamLine("RESET");
		integral = 0;
	}
	float kp = 0.7;
	float ki = kp/100;
	float err = curr * kp;
	int tp = 50;
	motor[LMotor] = tp + err + integral*ki;
	motor[RMotor] = tp - err - integral*ki;
	integral += curr;
}

void twoEye()
{
	int tp = 50;
	int green = -forward; //-17
	short rgbL[4],rgbR[4];
	getColorSensorData(Leye.port,colorRaw,&rgbL[0]);
	getColorSensorData(Reye.port,colorRaw,&rgbR[0]);
	unsigned int state = 0;
	if(rgbL[0] == 0 || rgbR[0] == 0)
		return;
	float ratioL = (float) rgbL[1]/ rgbL[0];
	float ratioR = (float) rgbR[1]/ rgbR[0];
	println(1,"RGB: %d %d %.1f %.1f %.2f", rgbL[1], rgbL[0],Leye.g[0],Leye.g[1], ratioL);
	println(3,"RGB: %d %d %.1f %.1f %.2f", rgbR[1], rgbR[0],Reye.g[0],Reye.g[1], ratioR);
	state = (int)getColor(Meye)*10;
	if (rgbL[1] > Leye.g[2]) state += (int)cSilver * 100;
	else if (rgbL[1] < Leye.g[0]) state += (int)cBlack * 100;
	else if (rgbL[1] > Leye.g[1]) state += (int)cWhite * 100;
	else if (ratioL >= Leye.rgb) state += (int)cGreen * 100;
	else state += (int)cGradient * 100;
	if (rgbR[1] > Reye.g[2]) state += (int)cSilver;
	else if (rgbR[1] < Reye.g[0]) state += (int)cBlack;
	else if (rgbR[1] > Reye.g[1]) state += (int)cWhite;
	else if (ratioR >= Reye.rgb) state += (int)cGreen;
	else state += (int)cGradient;
	if(getDigit(state,100) == cGreen) state = LEFTGREEN;
	if(getDigit(state,1) == cGreen) state = RIGHTGREEN;
	println(5,"%d",state);
	writeDebugStream("STATE: %d ",state);
	switch (state)
	{
	case NONE:
		println(0, "State: GAP");
		writeDebugStreamLine("GAP");
		resetEncoders();
		int begin = nPgmTime;
		//while(seeBlackArray() && nPgmTime - begin < 2000)
		//{
		//	if(getColor(Meye) || getColor(Leye) == cGreen || getColor(Reye) == cGreen) return;
		//	twoEyePID(kp,tp);
		//}
		motor[LMotor] = straight;
		motor[RMotor] = straight;
		while(nMotorEncoder[LMotor] < 10*cm){if(seeLine()) return;}
		playSound(soundLowBuzzShort);
		stopMotors();
		delay(100);
		SensorType[Leye.port] = sensorNone;
		SensorType[Reye.port] = sensorNone;
		wait1Msec(100);
		ubyte values[8];
		int tp = 15;
		float kp = 0.003;
		repeat(5)
		{
			motor[LMotor] = -straight;
			motor[RMotor] = -straight;
			while(!seeBlackArray()){delay(50);}
			stopMotors();
			wait1Msec(100);
			goBack(5);
			stopMotors();
			begin = nPgmTime;
			int curr;
			while(seeBlackArray() && nPgmTime - begin < 2000)
			{
				MSLSAreadSensors(splitter,values);
				curr = (upperLimit - wrap(values[0])) * 500 +
				(upperLimit - wrap(values[1])) * 100 +
				(upperLimit - wrap(values[2])) * 10 +
				(upperLimit - wrap(values[3])) * 1 +
				(upperLimit - wrap(values[4])) * -1 +
				(upperLimit - wrap(values[5])) * -10 +
				(upperLimit - wrap(values[6])) * -100 +
				(upperLimit - wrap(values[7])) * -500;
				motor[motorA] = tp + curr*kp;
				motor[motorC] = tp - curr*kp;
			}
		}
		stopMotors();
		wait1Msec(100);
		SensorType[Leye.port] = sensorColorNxtFULL;
		SensorType[Reye.port] = sensorColorNxtFULL;
		wait1Msec(100);
		//turnRight(15);
		writeDebugStreamLine("GAP: FORWARD");
		motor[LMotor] = forward;
		motor[RMotor] = forward;
		while(getColor(Leye) != cWhite || getColor(Reye) != cWhite){}
		while((getColor(Leye) == cWhite || getColor(Leye) == cGradient) && (getColor(Reye) == cWhite || getColor(Reye) == cGradient)){}
		writeDebugStreamLine("%d", getState());
		break;
		case RIGHTBLACK + LEFTBLACK:
			if(lastTurn == RIGHTBLACK)
			{
				setMotorSyncEncoder(LMotor, RMotor, -100, wheelbase*PI/4*cm, forward);
				while(stillRunning(LMotor) || stillRunning(RMotor))
				{

				}
				stopMotors();
				resetSync();
			}
			break;
	case RIGHTGREEN + MIDDLEBLACK:
	case RIGHTGREEN:
		println(0, "State: RIGHT GREEN");
		writeDebugStreamLine("RIGHT GREEN");
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
		goStraight(5);
		resetEncoders();
		motor[RMotor] = green;
		motor[LMotor] = forward;
		while(getColor(Meye) != cWhite){}
		while(getColor(Meye) != cBlack && nMotorEncoder[LMotor] < wheelbase*PI/4*cm){}
		stopMotors();
		playSound(soundBlip);
		motor[RMotor] = straight;
		motor[LMotor] = straight;
		while(getColor(Reye) == cGreen){}
		break;
	case LEFTGREEN + MIDDLEBLACK:
	case LEFTGREEN:
		println(0, "State: LEFT GREEN");
		writeDebugStreamLine("LEFT GREEN");
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
		goStraight(5);
		resetEncoders();
		motor[RMotor] = forward;
		motor[LMotor] =	green;
		while(getColor(Meye) != cWhite){}
		while(getColor(Meye) != cBlack && nMotorEncoder[RMotor] < wheelbase*PI/4*cm){}
		stopMotors();
		playSound(soundBlip);
		motor[RMotor] = straight;
		motor[LMotor] = straight;
		while(getColor(Leye) == cGreen){}
		break;
		//case LEFTBLACK + RIGHTBLACK:
		//	stopMotors();
		//	wait1Msec(100);
		//	int encoder = 0;
		//	int direction = lastTurn == RIGHTBLACK ? -1 : 1;
		//	resetEncoders();
		//	motor[LMotor] = -forward * direction;
		//	motor[RMotor] = forward * direction;
		//	while(getColor(Meye) != cBlack){}
		//	stopMotors();
		//	encoder = abs(nMotorEncoder[RMotor]);
		//	motor[LMotor] = forward * direction;
		//	motor[RMotor] = -forward  * direction;
		//	while(getColor(Meye) == cBlack){}
		//	while(getColor(Meye) != cBlack){}
		//	stopMotors();
		//	if(abs(nMotorEncoder[LMotor] - encoder) > encoder)
		//	{
		//		motor[LMotor] = -forward * direction;
		//		motor[RMotor] = forward * direction;
		//		while(getColor(Meye) == cBlack){}
		//		while(getColor(Meye) != cBlack){}
		//	}
		//	writeDebugStreamLine("DIR 1: %d DIR 2: %d",encoder,abs(nMotorEncoder[LMotor] - encoder));
		//	break;
	default:
		println(0, "State: PID");
		writeDebugStreamLine("PID");
		twoEyePID();
		break;
	}
	lastState = state;
}
