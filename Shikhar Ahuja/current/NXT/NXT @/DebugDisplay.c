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
	generateColor(S1,93.5,198.5,300,1.45,&Leye);
	generateLight(0x52,8,26,&Meye);
	generateColor(S3,119.5,204.0,300,1.5,&Reye);
	while (true)
	{
		println(0,"%d",getState());
		writeDebugStreamLine("%d",getState());
		if(getButtonPress(2))
			break;
		println(6,"Condition: %d",getState() != MIDDLEBLACK);
	}

}
