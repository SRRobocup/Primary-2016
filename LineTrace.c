#include "Global.h"
#include "FuncLib.c"

//float prevErr = -1;
 //The right eyed PID
void RIPID()
{
	float kp =2.1;//kp 5.25
	//float kd = 1.5;//kd 1.4
	float currErr = rthreshold-SensorValue[Reye];
	//if (prevErr == -1)
		//prevErr = currErr;
	float pGain = currErr * kp;//, dGain = (currErr - prevErr) *kd;   // tuned value

	motor[RMotor] = tpower + pGain;// - dGain;
	motor[LMotor] = tpower - pGain;// + dGain;

	println(1,"L: %d", motor[LMotor]);
	println(3,"R: %d", motor[RMotor]);
	println(5,"Current: %.2f", currErr);
	println(7,"Gain: %.2f", pGain);
	//prevErr = currErr;
}

//task main
//{
//	while (1)
//	RIPID();
//}

// The intrsections
void intersection()
{
	if (SensorValue[S3]<lthreshold-5)
	{
		playSound(soundLowBuzz);
		goStraight(6);
		wait1Msec(750);
		if (SensorValue[Reye]<rthreshold)
		{
			wait1Msec(750);
			return;
		}
		else
		{
			wait1Msec(750);
			while (SensorValue[Reye]> rblack)
			{
				motor[LMotor]=-40;
				motor[RMotor]=-40;
			}
			goBack(1);
			RIPID();
			wait1Msec(1000);
		}
	}
}
