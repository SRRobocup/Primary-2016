#include "common.h"
#include "Global.h"
#include "Evac.h"
#pragma DebuggerWindows("EV3LCDScreen")

task main()
{
	splitter = S3;
	SensorType[splitter] = sensorI2CCustom;
	generateColor(S2,63,151,302,1.5,&Leye);
	generateLight(0x52,19,55,&Meye);
	generateColor(S4,47,128,273,2,&Reye);
	arduino = S1;
	SensorType[arduino] = sensorI2CCustom;
	RMotor = motorC;
	LMotor = motorA;
	Sweeper = motorD;
	bMotorReflected[LMotor] = true;
	bMotorReflected[RMotor] = true;
	displayTextLine(2,"Sensor: %d",arduino);
	displayTextLine(3,"Type: %d",sensorI2CCustom);
	displayTextLine(4," Sensor: %d",SensorType[arduino]);
	delay(1000);
	//float last = 0, curr;
	//while (true)
	//{
	//	curr = getDistance(Lsonar);
	//	if(abs(curr -last) <= 0.02)
	//		continue;
	//	writeDebugStreamLine("Sensor: %f", curr);
	//	last = curr;
	//}
	//PID();
	//turnRight(90);
	//PID();
	while(true)
	{
		println(3,"%d",getDistance(Msonar));
	}
}
