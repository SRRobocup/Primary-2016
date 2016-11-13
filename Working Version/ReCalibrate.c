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

task runStraight()
{
	taskRunning = true;
	goStraight(0.5);
	taskRunning = false;
}

task main()
{
	//char* fileName = "sensorValue.txt";
	//long fileHandle = FileOpenWrite(fileName);
	waitDisplay("Place All on White");
	long white[3][3];
	getColorRGB(S1, white[0][0], white[0][1], white[0][2]);
	white[1][0] = SensorValue[S2];
	getColorRGB(S3, white[2][0], white[2][1], white[2][2]);

	long black[3][3];
	waitDisplay("Place Left on Black");
	getColorRGB(S1, black[0][0], black[0][1], black[0][2]);

	waitDisplay("Place Middle on Black");
	black[1][0] = SensorValue[S2];

	waitDisplay("Place Right on Black");
	getColorRGB(S3, black[2][0], black[2][1], black[2][2]);

	long green[3][2];
	float temp;
	long data[3];
	float minmax[3][2];
	for(int i = 0; i < 3; i++)
	{
		minmax[i][0] = 900;
		minmax[i][1] = -10;
		green[i][0] = 900;
		green[i][1] = -10;
	}
	waitDisplay("Roll Left on Green");
	startTask(runStraight);
	while(taskRunning)
	{
		display(4,"Task: %d", taskRunning);
		getColorRGB(S1,data[0],data[1],data[2]);
		temp = (float)data[1]/data[2];
		if(temp < minmax[0][0])
			minmax[0][0] = temp;
		if(temp > minmax[0][1])
			minmax[0][1] = temp;
		if(data[1] < green[0][0])
			green[0][0] = data[1];
		if(data[1] > green[0][1])
			green[0][1] = data[1];
	}
	stopTask(runStraight);
	waitDisplay("Roll Right on Green");
	startTask(runStraight);
	while(taskRunning)
	{
		display(4,"Task: %d", taskRunning);
		getColorRGB(S3,data[0],data[1],data[2]);
		temp = (float)data[1]/data[0];
		if(temp < minmax[2][0])
			minmax[2][0] = temp;
		if(temp > minmax[2][1])
			minmax[2][1] = temp;
		if(data[1] < green[2][0])
			green[2][0] = data[1];
		if(data[1] > green[2][1])
			green[2][1] = data[1];
	}
	stopTask(runStraight);
	display(3, "%.2f %.2f %.2f %.2f", (float)(black[0][1] + green[0][0])/2, (float)(green[0][1] + white[0][1])/2, minmax[0][0], minmax[0][1]);
	display(4, "%.2f", (float)(white[1][0] + black[1][0])/2);
	display(5, "%.2f %.2f %.2f %.2f", (float)(black[2][1] + green[2][0])/2, (float)(green[2][1] + white[2][1])/2, minmax[2][0], minmax[2][1]);
	while(true){}
}
