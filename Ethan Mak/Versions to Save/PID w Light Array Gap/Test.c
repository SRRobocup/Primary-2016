#include "common.h"
#include "Global.h"
#include "Evac.h"
//#pragma DebuggerWindows("EV3LCDScreen")

task main()
{
	splitter = S2;
	SensorType[splitter] = sensorI2CCustom;
	generateColor(S1,93.5,198.5,300,1.45,&Leye);
	generateLight(0x52,8,26,&Meye);
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
	//float last = 0, curr;
	//while (true)
	//{
	//	curr = getDistance(Lsonar);
	//	if(abs(curr -last) <= 0.02)
	//		continue;
	//	writeDebugStreamLine("Sensor: %f", curr);
	//	last = curr;
	//}
	//stopMotors();
	//wait1Msec(1000);
	//PID();

	//goStraight(length + 1);
	//evac();
	//stopMotors();
	sensortype[s1] = sensorNone;
	wait1Msec(10000);
}
