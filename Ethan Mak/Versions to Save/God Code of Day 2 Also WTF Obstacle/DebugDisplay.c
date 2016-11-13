#include "Global.h"
#include "LineTrace.h"
#pragma DebuggerWindows("EV3LCDScreen")
#ifndef DEBUG
#define DEBUG
#endif

task main()
{
	clearDebugStream();
	splitter = S2;
	SensorType[splitter] = sensorI2CCustom;
	MSLSAinit(splitter);
	float temp[11] = {176.0,263.7,188.5,1.31,10,29,75,176.0,215.0,156.5,1.29};
	generateColor(S1,temp[0],temp[1],temp[2],temp[3],&Leye); //93.5 198.5
	generateLight(0x52,temp[4],temp[5],temp[6],&Meye);
	generateColor(S3,temp[7],temp[8],temp[9],temp[10],&Reye); //119.5 204
	arduino = S4;
	SensorType[arduino] = sensorI2CCustomFast;
	RMotor = motorC;
	LMotor = motorA;
	Sweeper = motorB;
	bMotorReflected[LMotor] = true;
	bMotorReflected[RMotor] = true;
	while (true)
	{
		println(0,"%d",getState());
		writeDebugStreamLine("%d",getState());
		if(getXButtonValue(xButtonEnter))
			break;
		println(6,"Condition: %d",getState() != MIDDLEBLACK);
	}

}
