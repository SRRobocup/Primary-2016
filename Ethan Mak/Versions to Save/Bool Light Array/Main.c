#include "Global.h"
#include "LineTraceNewGap.h"
#include "Obstacle.h"
#include "Evac.h"

#ifdef EV3
#pragma DebuggerWindows("EV3LCDScreen")
#else
#pragma DebuggerWindows("NXTLCDScreen")
#endif

task Debug
{
	while(true)
	{
		//println(4,"%d %d %d %d",getColor(Leye),getColor(Meye),getColor(Reye),getState());
		println(6,"%d %d %d",motor[LMotor],motor[RMotor],forward, back);
	}
}

void setup()
{
	splitter = S2;
	SensorType[splitter] = sensorI2CCustom;
	MSLSAinit(splitter);
	float temp[10] = {147.0,263.0,4000.5,1.10,8,20,146.0,235.0,4000.5,1.10};
	generateColor(S1,temp[0],temp[1],temp[2],temp[3],&Leye); //93.5 198.5
	generateLight(0x52,temp[4],temp[5],&Meye);
	generateColor(S3,temp[6],temp[7],temp[8],temp[9],&Reye); //119.5 204
	arduino = S4;
	SensorType[arduino] = sensorI2CCustomFast;
	RMotor = motorC;
	LMotor = motorA;
	Sweeper = motorB;
	bMotorReflected[LMotor] = true;
	bMotorReflected[RMotor] = true;
}

task main()
{
	setup();
	startTask(Debug);
	//while(!(getColor(Leye) == cSilver && getColor(Reye) == cSilver && getColor(Meye) == cWhite))
	while(true)
	{
		//println(1, "%d" , getColor(Reye));
		//if(getDistance(Msonar) <= distObstacle)
		//{
		//	obstacle();
		//}
		//else
		{
			//println(4,"%d %d %d %d",getColor(Leye),getColor(Meye),getColor(Reye),getState());
			twoEye();
			//writeDebugStreamLine("%.2f %.2f %.2f %.2f", getDistance(Lsonar),getDistance(Msonar),getDistance(Rsonar),getDistance(Lsonar2));
		}
		wait1Msec(50);
	}
	//evactuation
	stopTask(Debug);
	playSound(soundBeepBeep);
	goStraight(length + 6);
	evac();
	stopMotors();
}
