#include "Global.h"
#include "mindsensors-lightsensorarray.h"

task main()
{
	splitter = S2;
	SensorType[splitter] = sensorI2CCustom9V;
	MSLSAinit(splitter);
	generateColor(S1,63,151,302,1.5,&Leye);
	generateLight(0x52,19,55,1000,&Meye);
	generateColor(S3,47,128,273,2,&Reye);
	arduino = S4;
	SensorType[arduino] = sensorI2CCustom;
	RMotor = motorC;
	LMotor = motorA;
	Sweeper = motorB;
	bMotorReflected[LMotor] = true;
	bMotorReflected[RMotor] = true;
	short temp[4];
	ubyte signalstr[8];
	while(true)
	{
		println(0,"LPing: %.3f",getDistance(Lsonar));
		println(1,"MPing: %.3f",getDistance(Msonar));
		println(2,"RPing: %.3f",getDistance(Rsonar));
		println(3,"L2Ping: %.3f",getDistance(Lsonar2));
		getColorSensorData(Leye.port,colorAtoD,&temp[0]);
		println(4,"L: %d %d %d %d", temp[0],temp[1],temp[2],temp[3]);
		println(5,"M: %d", getValue(Meye));
		getColorSensorData(Reye.port,colorAtoD,&temp[0]);
		println(6,"R: %d %d %d %d", temp[0],temp[1],temp[2],temp[3]);
		wait1Msec(100);
		//MSLSAreadSensors(splitter,signalstr);
		////eraseDisplay();
		//println(0,"A: %d %d %d %d",signalstr[0],signalstr[1],signalstr[2],signalstr[3]);
		//println(1,"   %d %d %d %d",signalstr[4],signalstr[5],signalstr[6],signalstr[7]);
		//wait1Msec(100);
		//eraseDisplay();
	}
}
