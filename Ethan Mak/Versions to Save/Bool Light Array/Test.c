#include "common.h"
#include "Global.h"
#include "Evac.h"
const int LEFTBLACK = 100;
const int LEFTGREEN = 200;
const int MIDDLEBLACK = 10;
const int RIGHTBLACK = 1;
const int RIGHTGREEN = 2;
const int NONE = 0;
unsigned int lastState = 0;
unsigned int lastTurn = 0;
float integral = 0;
//#pragma DebuggerWindows("EV3LCDScreen")

void twoEyePID()
{
	short rgbL[4], rgbR[4];
	getColorSensorData(Leye.port, colorRaw, &rgbL[0]);
	getColorSensorData(Reye.port, colorRaw, &rgbR[0]);
	float curr = rgbL[1] - rgbR[1];
	if(abs(curr) < 10)
	{
		writeDebugStreamLine("RESET");
		integral = 0;
	}
	float kp = 0.4;
	float ki = kp/350;
	float err = curr * kp;
	int tp = 50;
	motor[LMotor] = tp + err + integral*ki;
	motor[RMotor] = tp - err - integral*ki;
	integral += curr;
}

void twoEye()
{
	int thresh = 200;
	int fast = 40;
	int green = -forward; //-17
	short rgbL[4], rgbR[4];
	getColorSensorData(Leye.port,colorRaw,&rgbL[0]);
	getColorSensorData(Reye.port,colorRaw,&rgbR[0]);
	writeDebugStream("RGB:%d RGB2:%d ",rgbL[1],rgbR[1]);
	unsigned int state = (int)(rgbL[1]<thresh)*100 + (int)(rgbR[1]<thresh);
	float kpF = 0.3, kpB = 0.1;
	println(5,"%d",state);
	switch (state)
	{
		case LEFTBLACK + RIGHTBLACK:
		writeDebugStream("State: BOTH BLACK");
			break;
	case LEFTBLACK:
		writeDebugStream("State: LEFT BLACK ");
		motor[LMotor] = back - (thresh-rgbL[1])*kpB;
		motor[RMotor] = forward + (rgbR[1]-thresh)*kpF;
		while(rgbL[1] <= thresh)
		{
			getColorSensorData(Leye.port,colorRaw,&rgbL[0]);
			delay(10);
		}
		lastTurn = LEFTBLACK;
		break;
	case RIGHTBLACK:
		writeDebugStream("State: RIGHT BLACK");
		motor[RMotor] = back - (thresh-rgbR[1])*kpB;
		motor[LMotor] = forward + (rgbL[1]-thresh)*kpF;
		while(rgbR[1] <= thresh)
		{
			getColorSensorData(Reye.port,colorRaw,&rgbR[0]);
			delay(10);
		}
		break;
	default:
		writeDebugStream("State: STRAIGHT");
		motor[LMotor] = straight;
		motor[RMotor] = straight;
		break;
	}
	writeDebugStreamLine(" LMOTOR: %d RMOTOR: %d",motor[LMotor],motor[RMotor]);
	lastState = state;
}

task main()
{

	clearDebugStream();
	splitter = S2;
	SensorType[splitter] = sensorI2CCustom;
	MSLSAinit(splitter);
	generateColor(S1,215,442.0,6200,1.03,&Leye); //93.5 198.5
	generateLight(0x52,9,18,&Meye);
	generateColor(S3,315,508.0,6200.0,1.03,&Reye); //119.5 204
	arduino = S4;
	SensorType[arduino] = sensorI2CCustomFast;
	RMotor = motorC;
	LMotor = motorA;
	Sweeper = motorB;
	bMotorReflected[LMotor] = true;
	bMotorReflected[RMotor] = true;
	while(true)
	{
		twoEyePID();
		//delay(500);
	}
	wait1Msec(10000);
}
