#include "Global.h"
#include "FuncLib.c"

task Wobble()
{
	//the function crossing the gap
	while (true)
	{
		goStraight(4);
		turnRight(38);
		goStraight(4);
		turnLeft(38);
	}
}

void gap(int Encoder)
{// the function which prepares for gap
	displayTextLine(0,"Check gap worked");
	playSound(soundFastUpwardTones);
	nMotorEncoder[motorA]=0;
	while(nMotorEncoder[motorA]<Encoder)
	{
		motor[RMotor]=-tpower;
		motor[LMotor]=tpower;
	}
	stopMotors();
	wait1Msec(1000);
	startTask(Wobble);
	while(SensorValue[Reye]>rthreshold)
	{
		println(7,"R: %d",SensorValue[Reye]);
	}
	stopTask(Wobble);
}

void checkgap()
{
	//the function sensing that it is a gap
	if (SensorValue[Reye]>rthreshold+5)
	{
		//the else must not be met if the first condition is met, and the second is not
		if(abs(nMotorEncoder[motorC])>2250)//2000
		{
			int EncoderVal = nMotorEncoder[motorC];
			nMotorEncoder[motorC]=0;
			stopMotors();
			wait1Msec(300);
			gap(EncoderVal);
		}
	}
	else
	{
		nMotorEncoder[motorC] = 0;
	}
}
