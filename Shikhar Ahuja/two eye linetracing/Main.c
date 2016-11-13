#include "Global.h"
#include "FuncLib.c"
#include "LineTrace.c"
#include "Obstacle.c"

void setup()
{
#ifdef NXT
	SensorType[Leye] = sensorLightActive;
	SensorType[Reye] = sensorLightActive;
	SensorType[Ultrasonic] = sensorSONAR;
#else
	SensorType[Leye] = sensorEV3_Color;
	SensorType[Reye] = sensorEV3_Color;
	SensorType[Ultrasonic] = sensorEV3_Ultrasonic;
#endif
	//bMotorReflected[LMotor] = true;
	//bMotorReflected[RMotor] = true;
	nMotorPIDSpeedCtrl[LMotor] = mtrSpeedReg;
	nMotorPIDSpeedCtrl[RMotor] = mtrSpeedReg;
}

task main()
{
	setup();
	while(true)
	{
		if(getDistance(Ultrasonic) <= distObstacle)
		{
			println(7, "Obstacle");
			obstacle();
		}
		else
		{
			twoEye();
		}
	}
	//evactuation
}
