#ifndef __GLOBAL.H__
#define __GLOBAL.H__
#include "common.h"
#define DEBUG
#define ARDUINO_ADDRESS 0x08
#define LBU_ADDRESS 0x52
#define LFU_ADDRESS 0x58
#define RU_ADDRESS 0x56
#define Ultra_ADDRESS 0x02
#define println displayTextLine
#define writeln writeDebugStreamLine

typedef tSensors SensorPort;
typedef tMotor MotorPort;
typedef enum Color{
	cWhite = 0,
	cBlack = 1,
	cGreen = 2,
	cGradient = 3,
	cSilver = 4
} Color;
typedef struct {
	SensorPort port;
	float rgb;
	float g[3];
} ColorSensor;
typedef struct {
	int address;
	float bthreshold;
	float mthreshold;
} LightSensor;
typedef union floatunion{ubyte arr[4]; float val;} FloatUnion;

// defining all of the global variables
ColorSensor Leye;
LightSensor Meye;
ColorSensor Reye;
SensorPort splitter2;
SensorPort splitter;
MotorPort LMotor;
MotorPort RMotor;
MotorPort Sweeper;
const byte Lsonar = 0x43;
const byte Msonar = 0x44;
const byte Rsonar = 0x45;
const byte Lsonar2 = 0x46;
const float wheelbase = 23.4;
const float width = 18.3;
const float length = 19.0;
const float diameter = 4.5;
const float cm = 360.0 / (diameter *  PI);
const int straight = 10;
const int forward = 20;
const int back = -40;
const float distObstacle = 5;
//end

//defining all common functions
void generateColor(SensorPort pr, float t1, float t2, float t3, float rgb1, ColorSensor* res)
{
	SensorType[pr] = sensorColorNxtFULL;
	res->port = pr;
	res->g[0] = t1;
	res->g[1] = t2;
	res->g[2] = t3;
	res->rgb = rgb1;
}

void generateLight(int pr, float threshold,float threshold2, LightSensor* res)
{
	res->address = pr;
	res->bthreshold = threshold;
	res->mthreshold = threshold2;

}
#ifdef NXT
void setMotorSync(MotorPort a, MotorPort b, int ratio)
{
	if (a == motorA && b == motorC)
		nSyncedMotors = synchAC;
	else if (a == motorB && b == motorC)
		nSyncedMotors = synchBC;
	else if (a == motorA && b ==motorB)
		nSyncedMotors = synchAB;
	else if (a == motorC && b == motorA)
		nSyncedMotors = synchCA;
	else if (a == motorC && b == motorB)
		nSyncedMotors = synchCB;
	else if (a == motorB && b == motorA)
		nSyncedMotors = synchBA;
	nSyncedTurnRatio = ratio;
}

void setMotorSyncEncoder(MotorPort a, MotorPort b, int ratio, float target, int power)
{
	setMotorSync(a,b,ratio);
	nMotorEncoderTarget[a] = target;
	motor[a] = power;
}
#endif
// stopping
void stopMotors()
{
	motor[LMotor]=0;
	motor[RMotor]=0;
	motor[Sweeper]=0;
}

void resetEncoders()
{
nMotorEncoder[LMotor]=0;
nMotorEncoder[RMotor]=0;
nmotorEncoder[Sweeper]=0;
}

bool stillRunning(MotorPort m)
{
#ifdef NXT
	return nMotorEncoderTarget[m] != runStateIdle;
#else
	return getMotorRunning(m);
#endif
}
int getDistance(byte eye)
{

	tByteArray send;
	tByteArray recieve;
	send[0] = 2;
	send[1] = Ultra_ADDRESS;
	send[2] = eye;
	if(!writeI2C(splitter2,send,recieve,1))
	{
		playSound(soundBlip);
		return -1;
	}
	return recieve[0];
}
//int getDist(byte ar)
//{
//	byte eye=0x54;
//	tByteArray send;
//	tByteArray recieve;
//		send[0] = 2;
//		send[1] = ar;
//		send[2] = eye;



//		if(!writeI2C(splitter,send,recieve,4))
//	{
//		playSound(soundBlip);
//		return -1;
//	}

//	return recieve[0];
//}



// turning the robot right a certain amount of degrees
void turnRight(float deg)
{
	float target = wheelbase*PI*(deg/360)*cm;
	resetEncoders();
	setMotorSyncEncoder(LMotor, RMotor, -100, target, forward);
	while(stillRunning(LMotor) || stillRunning(RMotor)){}
	stopMotors();
	wait1Msec(100);
}
//turning left a certain amount of degrees
void turnLeft(float deg)
{
	float target = wheelbase*PI*(deg/360)*cm;
	resetEncoders();
	setMotorSyncEncoder(RMotor, LMotor, 100, target, forward);
	while(stillRunning(RMotor) || stillRunning(LMotor)){}
	stopMotors();
	wait1Msec(100);
}

// going straight a certain amount of centimeters
void goStraight(float dist)
{
	resetEncoders();
	dist *= cm;
	setMotorSyncEncoder(LMotor,RMotor,0,-dist,-forward);
	while(stillRunning(LMotor)){}
	stopMotors();
}
// going back a certain amount of centimeters
void goBack(float dist)
{
	resetEncoders();
	dist *= cm;
	setMotorSyncEncoder(LMotor,RMotor,0,dist,forward);
	while(stillRunning(LMotor)){}
	stopMotors();
}



Color getColor(ColorSensor x)
{
	short rgbSensor[34];
	getColorSensorData(x.port,colorRaw,&rgbSensor[0]);
	rgbSensor[1] <<= 1;
	rgbSensor[0] <<= 1;
	if (rgbSensor[0] == 0)
		return cBlack;
	float ratio = (float) rgbSensor[1]/ rgbSensor[0];
#ifdef DEBUG
	if(x == Leye)
		println(1,"RGB: %d %d %.1f %.1f %.2f", rgbSensor[1], rgbSensor[0],x.g[0],x.g[1], ratio);
	if(x == Reye)
		println(3,"RGB: %d %d %.1f %.1f %.2f", rgbSensor[1], rgbSensor[0],x.g[0],x.g[1], ratio);
#endif
	if (rgbSensor[1] > x.g[2])
		return cSilver;
	if (rgbSensor[1] > x.g[1])
		return cWhite;
	if (rgbSensor[1] < x.g[0])
		return cBlack;
	if (ratio >= x.rgb)
		return cGreen;
	return cGradient;
}

int getDigit(int src, int place)
{
	return (src / place) % 10;
}

int getValue(LightSensor x)
{
	tByteArray send;
	tByteArray recieve;
	send[0] = 2;
	send[1] = x.address;
	send[2] = 0x54;
	writeI2C(splitter,send,recieve,3);
	return (int) recieve[0];
}

bool getColor(LightSensor x)
{
		int y = getValue(x);
	println(2, "%d", y);
	return y < x.bthreshold;
}

bool getBoolColor(LightSensor l)
{
	return getValue(l) < l.mthreshold;
}

bool seeBlack()
{
	return getColor(Leye) == cBlack || getValue(Meye) < Meye.mthreshold || getColor(Reye) == cBlack;
}
//end
#endif
