#include "Global.h"
#include "FuncLib.c"

task Wobble()
{
	//the function crossing the gap
	while (true)
	{
		displayTextLine(5,"forward");
		goStraight(4);
		displayTextLine(5,"turning right");
		turnLeft(45);
			displayTextLine(5,"forward");
		goStraight(4);
			displayTextLine(5,"turning left");
		turnRight(38);
	}
}

void gap(int Encoder)
{// the function which prepares for gap
	displayTextLine(0,"Check gap worked");
	playSound(soundFastUpwardTones);
	nMotorEncoder[motorC]=0;
	while(nMotorEncoder[Rmotor]<Encoder)
	{
		motor[RMotor]=tpower;
		motor[LMotor]=-tpower;
	}
	stopMotors();
	playSound(soundBeepBeep);
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
	if (SensorValue[Reye]>rthreshold+2)
	{

		//the else must not be met if the first condition is met, and the second is not
	if(abs(nMotorEncoder[motorA])>2200)//2000
		{	stopMotors();

		playSound(soundBeepBeep);
			int EncoderVal = nMotorEncoder[Lmotor];
			nMotorEncoder[Lmotor]=0;
			stopMotors();
			wait1Msec(300);

		displayTextLine(2,"Check Gap worked!!")

			gap(EncoderVal);
}
		else
			return;

	}
	else
	{
		nMotorEncoder[motorA]=0;
		return;
	}
}
