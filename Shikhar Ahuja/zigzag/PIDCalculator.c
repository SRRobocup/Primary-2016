#include "Global.h"

void waitDisplay(char* mes)
{
	displayTextLine(4, mes);
	while(!getXbuttonValue(xButtonEnter)){}
	wait1Msec(500);
}

task main()
{
	float temp[11] = {154.5,273.0,184.5,1.27,9,22,55,159.0,246.0,159.0,1.25};
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
	short col[4];

	waitDisplay("L on blk|R on Wht");

	getColorSensorData(Leye.port,colorRaw,&col[0]);
	int leftBlack = col[1];

	getColorSensorData(Reye.port,colorRaw,&col[0]);
	int rightWhite = col[1];

	waitDisplay("Ready?");
	int tp = 50;
	motor[LMotor] = tp;
	motor[RMotor] = -tp;

	while (true)
	{
		getColorSensorData(Leye.port,colorRaw,&col[0]);
		if (abs(col[1] - leftBlack) < 10)
			break;
	}
	//while(getColor(Leye) != cBlack){}

	int tempT = nPgmTime;
	writeDebugStream("BEFORE: %d ",tempT);
	motor[LMotor] = -tp;
	motor[RMotor] = tp;

	short tempCol[4];
	while (true)
	{
		getColorSensorData(Leye.port,colorRaw,&col[0]);
		getColorSensorData(Reye.port,colorRaw,&tempCol[0]);
		if (abs(tempCol[1]-col[1]) < 20)
			break;
	}
	int end = nPgmTime;
	stopMotors();
	writeDebugStream("AFTER: %d ",end);
	writeDebugStreamLine("KP: %f",(float)(end-tempT)/(rightWhite-leftBlack)/2;
}
