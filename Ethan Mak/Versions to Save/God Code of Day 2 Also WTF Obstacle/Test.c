#include "common.h"
#include "Global.h"
#include "Evac.h"
//#include "LineTraceNewGap.h"
//#pragma DebuggerWindows("EV3LCDScreen")

task main()
{
	splitter = S2;
	SensorType[splitter] = sensorI2CCustom;
	generateColor(S1,93.5,198.5,300,1.45,&Leye);
	generateLight(0x52,8,26,100,&Meye);
	generateColor(S3,119.5,204.0,300,1.5,&Reye);
	arduino = S4;
	SensorType[arduino] = sensorI2CCustom;
	RMotor = motorC;
	LMotor = motorA;
	Sweeper = motorB;
	bMotorReflected[LMotor] = true;
	bMotorReflected[RMotor] = true;
	displayTextLine(2,"Sensor: %d",arduino);
	displayTextLine(3,"Type: %d",sensorI2CCustom);
	displayTextLine(4,"Sensor: %d",SensorType[arduino]);
	delay(1000);
	eraseDisplay();
	//tByteArray send;
	//tByteArray recieve;
	//send[0] = 2;
	//send[1] = 0x22;
	//send[2] = 0x57;
	float val;
	clearDebugStream();
	goStraight(length + 9;
	evac();
	//while(true)
	//{
	//	//PID();
	//	RightPID();
	//}
	//closeSweeper();
}
