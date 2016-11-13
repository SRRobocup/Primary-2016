#include "Global.h"
#include "LineTrace.h"
#pragma DebuggerWindows("EV3LCDScreen")
#ifndef DEBUG
#define DEBUG
#endif

task main()
{
	splitter = S2;
	SensorType[splitter] = sensorI2CCustom;
	MSLSAinit(splitter);
	generateColor(S1,185.5,489.0,592,1.25,&Leye); //93.5 198.5
	generateLight(0x52,10,29,&Meye);
	generateColor(S3,332,432.0,592.5,1.25,&Reye); //119.5 204
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
