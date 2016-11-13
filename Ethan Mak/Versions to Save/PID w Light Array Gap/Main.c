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
	generateColor(S1,135.5,150.5,300,1.38,&Leye); //93.5 198.5
	generateLight(0x52,20,32,&Meye);
	generateColor(S3,130.5,140,300,1.34,&Reye); //119.5 204
	arduino = S4;
	SensorType[arduino] = sensorI2CCustom;
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
