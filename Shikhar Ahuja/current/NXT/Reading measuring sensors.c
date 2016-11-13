#include "Global.h"

float getDist(byte ar)
{
	tByteArray send;
	tByteArray recieve;
	FloatUnion ret;
	send[0] = 2;
	send[1] = ar;
	send[2] = 0x54;
	if(!writeI2C(S4,send,recieve,2))
	{
		playSound(soundBlip);
		return -1;
	}
	for(int i = 0; i < 4; i++)
		ret.arr[i] = recieve[4-i];
	delay(5);
     return recieve[0]/10;
}

void setup()
{
	splitter = S4;
	//generateColor(S1,63,151,302,1.5,&Leye);
	//generateLight(0x52,19,55,&Meye);
	//generateColor(S3,47,128,273,2,&Reye);
	splitter2 = S2;
	SensorType[splitter] =sensorI2CCustom;
	SensorType[splitter2]=sensorI2CCustom;
	SensorType[S3]=sensorI2CCustom;
	RMotor = motorC;
	LMotor = motorA;
	Sweeper = motorB;
	bMotorReflected[LMotor] = true;
	bMotorReflected[RMotor] = true;
}


task main()
{
	setup();
	while(1)
	{
		float US=getDist(LBU_ADDRESS);
		println(0,"SONAR: %f",US);
		int US1=getDistance(0x42);
		//int US1=SensorValue[S2];
			println(1,"SONAR: %d", US1);
		float US2=getDist(LFU_ADDRESS);
			println(2,"SONAR: %f", US2);
		float US3=getDist(RU_ADDRESS);
		println(3,"SONAR: %f", US3);
		wait1Msec(400);
	}

}
