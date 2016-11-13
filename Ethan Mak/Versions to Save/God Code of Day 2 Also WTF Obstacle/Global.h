#ifndef __GLOBAL.H__
#define __GLOBAL.H__
#include "common.h"
#include "mindsensors-lightsensorarray.h"
#include "mindsensors-imu.h"
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
	float ratio;
	float thresh[2];
	float mthreshold;
} ColorSensor;
typedef struct {
	int address;
	float bthreshold;
	float mthreshold;
	float sthreshold;
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
const float wheelbase = 17.5;
const float width = 17;
const float length = 20;
const float diameter = 4.5;
const float cm = 360.0 / (diameter *  PI);
int straight = 35;
int forward = 40;//40 when battery is full
int back = -25; //when battery is full
const float distObstacle = 7;
const int arrThreshold = 40;
unsigned int upperLimit = 75;
unsigned int lowerLimit = 30;
ubyte zig[50];
//end

//defining all common functions

/**
* Generates a ColorSensor struct with the provided parameters
*
* @param pr Port to be assigned to struct
* @param t1 Threshold between black and green using green values
* @param t2 Threshold between green and white using green values
* @param t3 Threshold between white and black using green values
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
	res->port = pr;
	res->thresh[0] = t1;
	res->thresh[1] = t2;
	res->mthreshold = t3;
	res->ratio = rgb1;
	writeDebugStreamLine("COLOR EYE: %d T: %.1f %.1f M: %.1f R: %.2f", pr,t1,t2,t3,rgb1);
}

/**
* Generates a LightSensor struct at the given pointer with the given parameters
*
* @param pr I2C address of light sensor/adapter
* @param threshold Threshold to determine if values are truly black
* @param threshold2 Threshold between black and white
* @param threshold3 Threshold between silver and white
* @param res Pointer to resulting struct
*/
void generateLight(int pr, float threshold,float threshold2,float threshold3,LightSensor* res)
{
	res->address = pr;
	res->bthreshold = threshold;
	res->mthreshold = threshold2;
	res->sthreshold = threshold3;
	writeDebugStreamLine("LIGHT EYE: %d T: %.1f %.1f %.1f",pr,threshold,threshold2,threshold3);
}

/**
* Forces given value into a certain range
*
* @param val Number to be clamped/changed
* @param up The upper limit on result
* @param low The lower limit on result
*
* @ret Changed number
*/
int clamp(unsigned int val,unsigned int up = upperLimit,unsigned int low = lowerLimit)
{
	if(val > up)
		return up;
	if(val < low)
		return low;
	return val;
}

#ifdef NXT

/**
* Syncing to make the motors more accurate
*
* @param a Port of Motor 1
* @param b Port of Motor 2
* @param ratio Ratio between motors
*/
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

/**
* Reset the motor sync
*/
void resetSync()
{
	nSyncedMotors = synchNone;
}

/*
* Sync two motors and set motorTarget for one
*
* @param a Port of Motor 1
* @param b Port of Motor 2
* @param ratio The turn ratio
* @param target The encoder target
* @param power The power of Motor 1
*/
void setMotorSyncEncoder(MotorPort a, MotorPort b, int ratio, float target, int power)
{
	setMotorSync(a,b,ratio);
	nMotorEncoderTarget[a] = target;
	motor[a] = power;
}
#endif

/**
* Stop left and right motor
*/
void stopMotors()
{
	motor[LMotor]=0;
	motor[RMotor]=0;
	//motor[Sweeper]=0;
}

/**
* Reset Left and Right Motors
*/
void resetEncoders()
{
	nMotorEncoder[LMotor] = 0;
	nMotorEncoder[RMotor] = 0;
	//nMotorEncoder[Sweeper] = 0;
}

/**
* Checks if motor is still running
*
* @param m Motor to check
* @ret Motor is still running
*/
bool stillRunning(MotorPort m)
{
#ifdef NXT
	return nMotorRunState[m] != runStateIdle;
#else
	return getMotorRunning(m);
#endif
}

/**
* Gets Distance of certain PING
*
* @param eye I2C register of PING
* @ret The value of the PING
*/
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

/**
* Seeing which color the color sensors are seeing
*
* @param color sensor x
* @ret Color seen by sensor
*/
Color getColor(ColorSensor x)
{
	short rgbSensor[4];
	getColorSensorData(x.port,colorRaw,&rgbSensor[0]);
	if (rgbSensor[0] == 0)
		return cBlack;
	float ratio = (float) rgbSensor[1]/ (float)rgbSensor[0];
	if (rgbSensor[1] > x.thresh[1])
		return cWhite;
	if (rgbSensor[1] < x.thresh[0])
		return cBlack;
	if (ratio >= x.ratio)
		return cGreen;
	return cGradient;
	//if (rgbSensor[1] < x.mthreshold)
	//	return cBlack;
	//else
	//	return cWhite;
}

/**
* Returns any given digit of any given number
*
* @param src Number to extract digit from
* @param place Number place to extrace from
* @ret Digit extracted
*/
Color getDigit(int src, int place)
{
	return (Color) ((src / place) % 10);
}

/**
* Gets value from given light sensor
*
* @param x LightSensor to get value from
* @ret Value of light sensor
*/
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

/**
* Gets green spectrum of color sensor
*
* @param x Color sensor to extract
* @ret The green value of the Color Sensor
*/
int getValue(ColorSensor x)
{
	short col[4];
	getColorSensorData(x.port,colorRaw,&col[0]);
	return col[1];
}

/**
* Returns Color that the Light sensor is seeing
*
* @param x Lightsensor to detect
* @ret The Color that the sensor is seeing (Black, White, Silver)
*/
Color getColor(LightSensor x)
{
	int y = getValue(x);
	println(2, "%d", y);
	if(y > x.sthreshold)
		return cSilver;
	return (Color)(y < x.bthreshold);
}

/**
*  A more lenient version of above function that uses a higher threshold and does not detect silver
*
* @param x Lightsensor to detect
* @ret The Color that the sensor is seeing (Black, White)
*/
bool getBoolColor(LightSensor l)
{
	return getValue(l) < l.mthreshold;
}

/**
* Returns if any sensor definitely sees the line
*
* @ret If the sensors see the line
*/
bool seeLine()
{
	Color left = getColor(Leye);
	Color middle = getColor(Meye);
	Color right = getColor(Reye);
	return left == cBlack || left == cGreen || middle == cBlack || right == cBlack || right == cGreen;
}

/**
* A more lenient version of the above function
*
* @ret If the sensors see the line
*/
bool seeBoolLine()
{
	return getColor(Leye) != cWhite || getValue(Meye) < Meye.mthreshold || getColor(Reye) != cWhite;
}

/**
* Returns the value of one sensor on the light array
*
* @param index Index of sensor
* @ret Value of sensor
*/
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

/**
* Returns if any sensors of the light array sees the black line
*
* @param thresh Threshold of which determines if a sensor sees the line
* @ret If the array sees the line
*/
bool seeBlackArray(unsigned int thresh = arrThreshold)
{
	ubyte val[8];
	MSLSAreadSensors(splitter,val);
	//writeDebugStreamLine("%d %d %d %d %d %d %d %d",val[0],val[1],val[2],val[3],val[4],val[5],val[6],val[7]);
	for(int i = 0; i < 8; i++)
	{
		if (val[i] < thresh)
			return true;
	}
	return false;
}

/**
* Returns if one sensor on the array sees black
*
* @param index Index of sensor on array (0-7)
* @param thresh Threshold of which sensor sees black
* @ret If the sensors sees black
*/
bool getBoolArray(ubyte index, unsigned int thresh = arrThreshold)
{
	return getArray(index) < thresh;
}

/**
* Gets incline of robot based on accelerometer
*
* @ret Incline of robot
*/
float getIncline()
{
	short x_val,y_val,z_val;
	tByteArray send;
	tByteArray reply;
	send[0] = 2;
	send[1] = 0x02;
	send[2] = 0x42;
	writeI2C(splitter,send,reply,6);
	x_val = (reply[0] > 127) ? (reply[0] - 256) * 4 + reply[3] : reply[0] * 4 + reply[3];
  y_val = (reply[1] > 127) ? (reply[1] - 256) * 4 + reply[4] : reply[1] * 4 + reply[4];
  z_val = (reply[2] > 127) ? (reply[2] - 256) * 4 + reply[5] : reply[2] * 4 + reply[5];
  if (z_val == 0)
  	return 0;
	float deg = radiansToDegrees(atan((float)x_val/z_val));
	deg = deg > 180 ? deg-360 : deg;
	if(deg == -360)
		deg = 0;
	return deg;
}

/**
* Gets acceleration of x axis and z axis
*
* @param x_val Pointer to resulting variable for x axis
* @param z_val Pointer to resulting variable for z axis
*/
void getAccel(short &x_val, short &z_val)
{
	tByteArray send;
	tByteArray reply;
	send[0] = 2;
	send[1] = 0x02;
	send[2] = 0x42;
	writeI2C(splitter,send,reply,6);
	x_val = (reply[0] > 127) ? (reply[0] - 256) * 4 + reply[3] : reply[0] * 4 + reply[3];
  z_val = (reply[2] > 127) ? (reply[2] - 256) * 4 + reply[5] : reply[2] * 4 + reply[5];
}

//int getIncline()
//{
//	tByteArray send;
//	tByteArray reply;
//	send[0] = 2;
//	send[1] = 0x32;
//	send[2] = 0x54;
//	writeI2C(splitter,send,reply,2);
//	float dist = (float)((reply[1] << 8)+reply[0])/10;
//	if(dist > 5)
//		return -16;
//	if(dist < 2)
//		return 16;
//	return 0;
//}

#ifdef DEBUG
/**
* Writes values of all sensors of light array to debug stream
*/
void writeArr()
{
	ubyte val[8];
	MSLSAreadSensors(splitter,val);
	writeDebugStreamLine("%d %d %d %d %d %d %d %d",val[0],val[1],val[2],val[3],val[4],val[5],val[6],val[7]);
}
#endif

/**
* Stops motors and suspends programs
*/
void suspend()
{
	playSound(soundShortBlip);
	stopMotors();
	while(true){}
}

//end
#endif
