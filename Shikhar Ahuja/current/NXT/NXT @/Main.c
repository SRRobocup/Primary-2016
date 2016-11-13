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
	generateColor(S1,93.5,198.5,300,1.45,&Leye); //93.5 198.5
	generateLight(0x52,8,26,&Meye);
	generateColor(S3,119.5,204.0,300,1.5,&Reye); //119.5 204
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
