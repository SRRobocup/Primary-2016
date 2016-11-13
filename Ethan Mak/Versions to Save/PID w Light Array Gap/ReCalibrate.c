#define display displayTextLine
#define Light
#pragma DebuggerWindows("NXTLCDScreen")
#include "Global.h"
#include "mindsensors-lightsensorarray.h"

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

#ifdef NXT
void getColorRGB(SensorPort port, long &r, long &g, long&b)
{
	short temp[4];
	getColorSensorData(port,colorRaw,&temp[0]);
	r = temp[0];
	g = temp[1];
	b = temp[2];
}

void reset(SensorPort port)
{
	short temp[4];
	getColorSensorData(port,colorRaw,&temp[0]);
}
#endif
task main()
{
	SensorPort LPort = S1, RPort = S3;
	SensorType[LPort] = sensorColorNxtFULL;
	SensorType[RPort] = sensorColorNxtFULL;
	splitter = S2;
	SensorType[splitter] = sensorI2CCustom;
	MSLSAinit(splitter);
	generateLight(0x52,19,55,&Meye);
	RMotor = motorC;
	LMotor = motorA;
	long delete;
	bMotorReflected[motorA] = true;
	bMotorReflected[motorC] = true;
	waitDisplay("Place All on White");
	long white[3];
	//reset(LPort);
	getColorRGB(LPort, delete, white[0], delete);
	//white[0] <<= 1;
	white[1] = getValue(Meye);
	//reset(RPort);
	getColorRGB(RPort, delete, white[2], delete);
	//white[2] <<= 1;
	MSLSAcalWhite(splitter);

	long black[3];
	waitDisplay("Place Left and Right on Black");
	//reset(LPort);
	getColorRGB(LPort, delete, black[0], delete);
	getColorRGB(RPort, delete, black[2], delete);
	//black[2] <<= 1;
	//black[0] <<= 1;

	waitDisplay("Place Middle on Black");
	black[1] = getValue(Meye);

	SensorType[LPort] = sensorNone;
	SensorType[RPort] = sensorNone;

	wait1Msec(100);

	waitDisplay("Place Array on Black");
	MSLSAcalBlack(splitter);

	delay(500);
	MSLSASleep(splitter);
	delay(500);
	MSLSAinit(splitter);

	SensorType[LPort] = sensorColorNxtFULL;
	SensorType[RPort] = sensorColorNxtFULL;
	wait1Msec(100);

	long silver[2];
	waitDisplay("Place Left and Right on Silver");
	//reset(LPort);
	getColorRGB(LPort,delete, silver[0], delete);
	//silver[0] <<= 1;
	reset(RPort);
	getColorRGB(RPort,delete, silver[1], delete);
	//silver[1] <<= 1;

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
		//reset(LPort);
		getColorRGB(LPort,data[0],data[1],delete);
		//data[0]<<=1;
		//data[1]<<=1;
		if (data[0] == 0)
			continue;
		temp = (float)data[1]/data[0];
		if(temp < min[0])
			min[0] = temp;
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
		//reset(RPort);
		getColorRGB(RPort,data[0],data[1],delete);
		//data[0]<<=1;
		//data[1]<<=1;
		if (data[0] == 0)
			continue;
		temp = (float)data[1]/data[0];
		if(temp < min[1])
			min[1] = temp;
		if(data[1] < green[1][0])
			green[1][0] = data[1];
		if(data[1] > green[1][1])
			green[1][1] = data[1];
	}
	//stopTask(run);
	playSound(soundBlip);

	display(2, "%.1f %.1f", (float)(black[0] + green[0][0])/2, (float)(green[0][1] + white[0])/2);
	display(3, "%.1f %.2f", (float)(white[0] + silver[0])/2, min[0]);
	display(4, "%d %d", black[1]+5, (black[1] + white[1])/2);
	display(5, "%.1f %.1f", (float)(black[2] + green[1][0])/2, (float)(green[1][1] + white[2])/2);
	display(6, "%.1f %.2f",(float)(white[2] + silver[1])/2, min[1]);
	while(true){}
}
