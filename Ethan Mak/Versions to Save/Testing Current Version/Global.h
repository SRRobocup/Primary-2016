#ifndef __GLOBAL.H__
#define __GLOBAL.H__
#include "common.h"
#include "mindsensors-lightsensorarray.h"
#define DEBUG
#define ARDUINO_ADDRESS 0x08
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
typedef union FloatUnion{ubyte arr[4]; float val;} FloatUnion;

// defining all of the global variables
ColorSensor Leye;
LightSensor Meye;
ColorSensor Reye;
SensorPort arduino;
SensorPort splitter;
MotorPort LMotor;
MotorPort RMotor;
MotorPort Sweeper;
const byte Lsonar = 0x43;
const byte Msonar = 0x44;
const byte Rsonar = 0x45;
const byte Lsonar2 = 0x46;
const float wheelbase = 17.2;
const float width = 16.8;
const float length = 18.5;
const float diameter = 4.5;
const float cm = 360.0 / (diameter *  PI);
const int straight = 30;
const int forward = 40;//40 when battery is full
const int back = -15;//-25 when battery is full
const float distObstacle = 3;
const int arrThreshold = 40;
unsigned int upperLimit = 75;
unsigned int lowerLimit = 30;
//end

//defining all common functions

/**
* Generates a ColorSensor struct with the provided parameters
*
* @param pr Port to be assigned to struct
* @param t1 Threshold between black and green using green values
* @param t2 Threshold between green and white using green values
* @param t3 Threshold between white and silver using green values
* @param rgb1 Ratio between R and G values to determine if color is green or gradient
* @param res Pointer to resulting struct
*/
void generateColor(SensorPort pr, float t1, float t2, float t3, float rgb1, ColorSensor* res)
{
#ifdef NXT
	SensorType[pr] = sensorColorNxtFULL;
#else
	SensorType[pr] = sensorEV3_Color;
#endif
	writeDebugStreamLine("EYE: %d PORT: %d T: %f %f %f R: %f",pr+1,pr,t1,t2,t3,rgb1);
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

int wrap(ubyte val)
{
	if(val > upperLimit)
		return upperLimit;
	if(val < lowerLimit)
		return lowerLimit;
	return val;
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

void resetSync()
{
	nSyncedMotors = synchNone;
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
	//motor[Sweeper]=0;
}

void resetEncoders()
{
	nMotorEncoder[LMotor] = 0;
	nMotorEncoder[RMotor] = 0;
	//nMotorEncoder[Sweeper] = 0;
}

bool stillRunning(MotorPort m)
{
#ifdef NXT
	return nMotorRunState[m] != runStateIdle;
#else
	return getMotorRunning(m);
#endif
}

float getDistance(byte eye)
{
	tByteArray send;
	tByteArray recieve;
	FloatUnion ret;
	send[0] = 2;
	send[1] = ARDUINO_ADDRESS;
	send[2] = eye;
	if(!writeI2C(arduino,send,recieve,4))
	{
		playSound(soundBlip);
		return -1;
	}
	for(int i = 0; i < 4; i++)
		ret.arr[i] = recieve[i];
	delay(5);
	return ret.val;
}

// turning the robot right a certain amount of degrees
void turnRight(float deg, int power = forward)
{
	float target = wheelbase*PI*(deg/360)*cm;
	resetEncoders();
	//setMotorSyncEncoder(LMotor, RMotor, -100, target, forward);
	setMotorSyncEncoder(LMotor, RMotor, -100, target, power);
	while(stillRunning(LMotor) || stillRunning(RMotor)){}
	stopMotors();
	resetSync();
	wait1Msec(100);
}
//turning left a certain amount of degrees
void turnLeft(float deg, int power = forward)
{
	float target = wheelbase*PI*(deg/360)*cm;
	resetEncoders();
	//setMotorSyncEncoder(RMotor, LMotor, 100, target, forward);
	setMotorSyncEncoder(RMotor, LMotor, -100, target, power);
	while(stillRunning(RMotor) || stillRunning(LMotor)){}
	stopMotors();
	resetSync();
	wait1Msec(100);
}

void turn(float deg, int power = forward)
{
	if (abs(deg) < 0.1) return;
	if(deg < 0)
		turnLeft(deg, power);
	else
		turnRight(deg, power);
}

// going straight a certain amount of centimeters
void goStraight(float dist, int power = forward)
{
	resetEncoders();
	dist *= cm;
	setMotorSyncEncoder(LMotor,RMotor,100,dist,power);
	while(stillRunning(LMotor)){}
	stopMotors();
	resetSync();
}
// going back a certain amount of centimeters
void goBack(float dist, int power = forward)
{
	resetEncoders();
	dist *= cm;
	setMotorSyncEncoder(LMotor,RMotor,100,-dist,-forward);
	while(stillRunning(LMotor)){}
	stopMotors();
	resetSync();
}

Color getColor(ColorSensor x)
{
	short rgbSensor[4];
	getColorSensorData(x.port,colorRaw,&rgbSensor[0]);
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
	writeI2C(splitter,send,recieve,1);
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

ubyte getArray(ubyte index)
{
	tByteArray send;
	tByteArray receive;
	send[0] = 2;
	send[1] = MSLSA_I2C_ADDR;
	send[2] = 0x42 + index;
	writeI2C(splitter, send, receive, 1);
	return receive[0];
}

bool getBoolArray(ubyte index)
{
	return getArray(index) < arrThreshold;
}

bool seeBlackArray()
{
	ubyte val[8];
	MSLSAreadSensors(splitter,val);
	//writeDebugStreamLine("%d %d %d %d %d %d %d %d",val[0],val[1],val[2],val[3],val[4],val[5],val[6],val[7]);
	for(int i = 0; i < 8; i++)
	{
		if (val[i] < arrThreshold)
			return true;
	}
	return false;
}
#ifdef DEBUG
void writeArr()
{
	ubyte val[8];
	MSLSAreadSensors(splitter,val);
	writeDebugStreamLine("%d %d %d %d %d %d %d %d",val[0],val[1],val[2],val[3],val[4],val[5],val[6],val[7]);
}
#endif

//end
#endif
