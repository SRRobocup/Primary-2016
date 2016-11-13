
#include "common.h"
void waitDisplay(char* mes)
{
	displayTextLine(4, mes);
	while(!getXbuttonValue(xButtonEnter)){}
	wait1Msec(500);
}

float determineThresh(float x, float y)
{
	if (x > y)
		return x-(abs(x-y)*2/3);
	else
		return x+(abs(x-y)*2/3);
}

task main()
{
	SensorType[S1] = sensorColorNxtFULL;
	short temp[4];
	float temp1[4] = {9000,9000,-9000,9000};
	float ratio = 40000;
	waitDisplay("Place on white");
	getColorSensorData(S1,colorRaw,&temp[0]);
	temp1[3] = temp[0];
	writeDebugStreamLine("%f",temp[0]);
	waitDisplay("Place on black");
	getColorSensorData(S1,colorRaw,&temp[0]);
	writeDebugStreamLine("%f",temp[0]);
	temp1[0] = temp[0];
	waitDisplay("Place on red");
	float curr;
	for(int i = 0; i < 500; i++)
	{
		//reset(LPort);
		getColorSensorData(S1,colorRaw,&temp[0]);
		if (temp[1] == 0)
			continue;
		curr = (float)temp[0]/temp[1];
		if(curr < ratio)
			ratio = curr;
		if(temp[0] < temp1[1])
			temp1[1] = temp[0];
		if(temp[0] > temp1[2])
			temp1[2] = temp[0];
	}
	writeDebugStreamLine("%f %f %f %f",temp1[0],temp1[1],temp1[2],temp1[3]);
	writeDebugStreamLine("{%f,%f,%f}",determineThresh(temp1[0],temp1[1]),determineThresh(temp1[3],temp1[2]),ratio);
}
