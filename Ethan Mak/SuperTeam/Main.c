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
	clearDebugStream();
	memset(zig,0,sizeof(zig));
	splitter = S2;
	SensorType[splitter] = sensorI2CCustom;
	MSLSAinit(splitter);
	float temp[11] = {154.7,247.3,193.0,1.3,15,40,90,157.0,239.3,188.5,1.3};
	generateColor(S1,temp[0],temp[1],temp[2],temp[3],&Leye); //93.5 198.5
	generateLight(0x52,temp[4],temp[5],temp[6],&Meye);
	generateColor(S3,temp[7],temp[8],temp[9],temp[10],&Reye); //119.5 204
	arduino = S4;
	SensorType[arduino] = sensorI2CCustomFast;
	RMotor = motorC;
	LMotor = motorA;
	Sweeper = motorB;
	bMotorReflected[LMotor] = true;
	bMotorReflected[RMotor] = true;
}

void endProgram()
{
	stopMotors();
	stopTask(Debug);
}

task main()
{
	clearDebugStream();
	eraseDisplay();
	setup();
	startTask(Debug);
	//writeDebugStreamLine("%d %d",getValue(Meye),Meye.sthreshold);
	bPlaySounds = true;
	nVolume = 4;
	wait1Msec(100);
	while(true)
	{
		if(getColor(Meye) == cSilver)
		{
			writeDebugStreamLine("SAW SILVER");
			stopMotors();
			delay(100);
			break;
		}
		if(getDistance(Msonar) <= distObstacle)
		{
			writeDebugStreamLine("Obstacle");
			obstacle();
			eraseDisplay();
		}
		else
		{
			//println(4,"%d %d %d %d",getColor(Leye),getColor(Meye),getColor(Reye),getState());
			twoEye();
			//writeDebugStreamLine("PINGS: %.2f %.2f %.2f %.2f", getDistance(Lsonar),getDistance(Msonar),getDistance(Rsonar),getDistance(Lsonar2));
			//writeDebugStreamLine("----------------END OF ITERATION-------------------");
		}
		wait1Msec(50);
		EndTimeSlice();
	}
	playSound(soundBeepBeep);
	eraseDisplay();
	//evactuation
	playSound(soundBeepBeep);
	goStraight(length + 8);
	evac();
	endProgram();
}
