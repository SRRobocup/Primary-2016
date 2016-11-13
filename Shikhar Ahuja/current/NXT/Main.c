#include "Global.h"
#include "LineTraceNewGap.h"
//#include "Obstacle.h"
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
		println(4,"%d %d %d %d",getColor(Leye),getColor(Meye),getColor(Reye),getState());
		println(6,"%d %d %d",motor[LMotor],motor[RMotor],forward, back);
	}
}

void setup()
{
	splitter = S2;
	generateColor(S1,63,151,302,1.5,&Leye);
	generateLight(0x52,19,55,&Meye);
	generateColor(S3,47,128,273,2,&Reye);
	splitter2 = S4;
	SensorType[splitter2] = sensorI2CCustom;
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
	while(!(getColor(Leye) == cSilver && getColor(Reye) == cSilver && getColor(Meye) == cWhite))
	{
		//println(1, "%d" , getColor(Reye));
		//if(getDistance(Msonar) <= distObstacle)
		//{
		//	obstacle();
		//}
		//else

			println(4,"%d %d %d %d",getColor(Leye),getColor(Meye),getColor(Reye),getState());
			twoEye();

		wait1Msec(50);
	}
	//evactuation
	stopTask(Debug);
	playSound(soundBeepBeep);
	goStraight(length);
	//evac();
	stopMotors();
}
