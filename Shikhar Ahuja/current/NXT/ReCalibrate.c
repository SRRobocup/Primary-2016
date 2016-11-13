#define display displayTextLine
#define Light

#include "Global.h"

bool taskRunning = false;

void waitDisplay(char* mes)
{
	displayTextLine(4, mes);
	while(!getXbuttonValue(xButtonEnter)){}
	wait1Msec(500);
}

task run()
{
	taskRunning = true;
	goStraight(1);
	taskRunning = false;
	wait1Msec(100);
}

task main()
{
	RMotor = motorC;
	LMotor = motorA;
	long delete;
	bMotorReflected[motorA] = true;
	bMotorReflected[motorC] = true;
	waitDisplay("Place All on White");
	short white[3];
	getColorSensorData(S1,colorRaw, &white[0]);
	white[0] <<= 1;
	white[1] = SensorValue[Meye.mthreshold];
	getColorSensorData(S3,colorRaw, &white[0]);
	white[2] <<= 1;

	short black[3];
	waitDisplay("Place Left on Black");
	getColorSensorData(S1, colorRaw, &black[0]);
	black[0] <<= 1;

	waitDisplay("Place Middle on Black");
	black[1] = SensorValue[S2];

	waitDisplay("Place Right on Black");
	getColorSensorData(S3, colorRaw, &black[2]);
	black[2] <<= 1;

	short silver[2];
	waitDisplay("Place Left and Right on Silver");
	getColorSensorData(S1,colorRaw, &silver[2]);
	silver[0] <<= 1;
	getColorSensorData(S3,colorRaw, &silver[1]);
	silver[1] <<= 1;

	long green[2][2] = {{900,-10},{900,-10}};
	float temp;
	short data[3];
	float min[] = {900, 900};
	waitDisplay("Roll Left on Green");
	playSound(soundBlip);
	//startTask(run);
	//wait1Msec(50);
	//while(taskRunning)
	for(int i = 0; i < 500; i++)
	{
		getColorSensorData(S1,colorRaw,&data[0]);
		getColorSensorData(S1,colorRaw,&data[1]);
		data[0]<<=1;
		data[1]<<=1;
		temp = (float)data[1]/data[0];
		if(temp < min[0])
			min[0] = temp;
		if (data[0] == 0)
			continue;
		if(data[1] < green[0][0])
			green[0][0] = data[1];
		if(data[1] > green[0][1])
			green[0][1] = data[1];
	}
	//stopTask(run);
	playSound(soundBlip);

	waitDisplay("Roll Right on Green");
	playSound(soundBlip);
	//startTask(run);
	//wait1Msec(50);
	//while(taskRunning)
	for(int i = 0; i < 500; i++)
	{
		getColorSensorData(S3,colorRaw,&data[0]);
		data[0]<<=1;
		data[1]<<=1;
		temp = (float)data[1]/data[0];
		if (data[0] == 0)
			continue;
		if(temp < min[1])
			min[1] = temp;
		if(data[1] < green[1][0])
			green[1][0] = data[1];
		if(data[1] > green[1][1])
			green[1][1] = data[1];
	}
	//stopTask(run);
	playSound(soundBlip);

	display(3, "%.2f %.2f %.2f %.2f", (float)(black[0] + green[0][0])/2, (float)(green[0][1] + white[0])/2,(float)(white[0] + silver[0])/2, min[0]);
	display(4, "%d %d", black[1]+5, (black[1] + white[1])/2);
	display(5, "%.2f %.2f %.2f %.2f", (float)(black[1] + green[1][0])/2, (float)(green[1][1] + white[2])/2,(float)(white[2] + silver[1])/2, min[1]);
	while(true){}
}
