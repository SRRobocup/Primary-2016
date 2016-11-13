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
	while (true)
	{
		println(0,"%d",getState());
		writeDebugStreamLine("%d",getState());
		if(getXButtonValue(xButtonEnter))
			break;
		println(6,"Condition: %d",getState() != MIDDLEBLACK);
	}

}
