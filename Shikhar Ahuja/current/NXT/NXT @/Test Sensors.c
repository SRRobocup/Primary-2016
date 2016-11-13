#include "Global.h"
#include "mindsensors-lightsensorarray.h"

task main()
{
	splitter = S2;
	SensorType[splitter] = sensorI2CCustom;
	generateColor(S1,63,151,302,1.5,&Leye);
	generateLight(0x52,19,55,&Meye);
	generateColor(S3,47,128,273,2,&Reye);
	arduino = S4;
	SensorType[arduino] = sensorI2CCustom;
	RMotor = motorC;
	LMotor = motorA;
	Sweeper = motorB;
	bMotorReflected[LMotor] = true;
	bMotorReflected[RMotor] = true;
	short temp[4];
	ubyte values[8];
	while(true)
	{
		//println(0,"LPing: %.3f",getDistance(Lsonar));
		//println(1,"MPing: %.3f",getDistance(Msonar));
		//println(2,"RPing: %.3f",getDistance(Rsonar));
		//println(3,"L2Ping: %.3f",getDistance(Lsonar2));
		//getColorSensorData(Leye.port,colorRaw,&temp[0]);
		//println(4,"L: %d %d %d %d", temp[0],temp[1],temp[2],temp[3]);
		//println(5,"M: %d", getValue(Meye));
		//getColorSensorData(Reye.port,colorRaw,&temp[0]);
		//println(6,"R: %d %d %d %d", temp[0],temp[1],temp[2],temp[3]);
		//wait1Msec(500);
		MSLSAreadSensors(splitter,values);
		eraseDisplay();
		println(0,"A: %d %d %d %d",values[0],values[1],values[2],values[3]);
		println(1,"   %d %d %d %d",values[4],values[5],values[6],values[7]);
		wait1Msec(500);
		eraseDisplay();
	}
}
