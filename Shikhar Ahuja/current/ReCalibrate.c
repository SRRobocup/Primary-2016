#define display displayTextLine
#define Light
#pragma DebuggerWindows("EV3LCDScreen")
#include "Global.h"

bool taskRunning = false;

void waitDisplay(char* mes)
{
	displayTextLine(4, mes);
	while(!getButtonPress(2)){}
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
	long white[3];
	getColorRGB(S1, delete, white[0], delete);
	white[0] <<= 1;
	white[1] = SensorValue[Meye.port]
	getColorRGB(S3, delete, white[1], delete);
	white[2] <<= 1;

	long black[3];
	waitDisplay("Place Left on Black");
	getColorRGB(S1, delete, black[0], delete);
	black[0] <<= 1;

	waitDisplay("Place Middle on Black");
	black[1] = SensorValue[S2];

	waitDisplay("Place Right on Black");
	getColorRGB(S3, delete, black[2], delete);
	black[2] <<= 1;

	long silver[2];
	waitDisplay("Place Left and Right on Silver");
	getColorRGB(S1,delete, silver[0], delete);
	silver[0] <<= 1;
	getColorRGB(S3,delete, silver[1], delete);
	silver[1] <<= 1;

	long green[2][2] = {{900,-10},{900,-10}};
	float temp;
	long data[3];
	float min[] = {900, 900};
	waitDisplay("Roll Left on Green");
	playSound(soundBlip);
	//startTask(run);
	//wait1Msec(50);
	//while(taskRunning)
	for(int i = 0; i < 500; i++)
	{
		getColorRGB(S1,data[0],data[1],delete);
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
		getColorRGB(S3,data[0],data[1],delete);
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
