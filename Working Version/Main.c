 #include "Global.h"
#include "LineTrace.h"
#include "Obstacle.h"
#pragma DebuggerWindows("EV3LCDScreen")

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
	generateColor(S1,22.5,64.5,1.60,1.90,&Leye);
	generateLight(S2,56,&Meye);
	generateColor(S3,26.5,66.5,1.68,1.91,&Reye);
	Ultrasonic = S4;
	SensorType[Ultrasonic] = sensorEV3_Ultrasonic;
	LMotor = motorA;
	RMotor = motorC;
	UMotor = motorB;
	Sweeper = motorD;
}

task main()
{
	setup();
	startTask(Debug);
	while(true)
	{
		//println(1, "%d" , getColor(Reye));
		//if(getUSDistance(Ultrasonic) <= distObstacle)
		//{
		//	println(7, "Obstacle");
		//	obstacle();
		//}
		//else
		//{
	println(4,"%d %d %d %d",getColor(Leye),getColor(Meye),getColor(Reye),getState());
#ifdef PID
		if(!twoEye())
			RIPID();
#else
		twoEye();
#endif
		//}
//		wait1Msec(50);
	}
	//evactuation
	stopTask(Debug);
	evacuation();
}
