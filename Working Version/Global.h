#ifndef __GLOBAL.H__
#define __GLOBAL.H__
#define DEBUG
#define println displayTextLine
#define writeln writeDebugStreamLine

typedef tSensors SensorPort;
typedef tMotor MotorPort;
typedef enum Color{
cWhite = 0,
cBlack = 1,
cGreen = 2,
cGradient = 3
} Color;
typedef struct {
	SensorPort port;
	float rgb[2];
	float g[2];
} ColorSensor;
typedef struct {
	SensorPort port;
	float threshold;
	float sthreshold;
} LightSensor;

// defining all of the global variables
ColorSensor Leye;
LightSensor Meye;
ColorSensor Reye;
SensorPort Ultrasonic;
MotorPort LMotor;
MotorPort RMotor;
MotorPort UMotor;
MotorPort Sweeper;
const float wheelbase = 20;
const float diameter = 4;
const float cm = 360.0 / (diameter *  PI);
const int forward = 30;
const int back = -39;//-17
const float distObstacle = 12;
//end

//defining all common functions
void generateColor(SensorPort pr, float t1, float t2, float rgb1, float rgb2, ColorSensor* res)
{
	SensorType[pr] = sensorEV3_Color;
	res->port = pr;
	res->g[0] = t1;
	res->g[1] = t2;
	res->rgb[0] = rgb1;
	res->rgb[1] = rgb2;
}

void generateLight(SensorPort pr, float threshold, LightSensor* res)
{
	SensorType[pr] = sensorEV3_Color;
	res->port = pr;
	res->threshold = threshold;
}

// stopping
void stopMotors()
{
	motor[LMotor]=0;
	motor[RMotor]=0;
}

void resetEncoders()
{
	resetMotorEncoder(LMotor);
	resetMotorEncoder(RMotor);
}

bool stillRunning(MotorPort m)
{
#ifdef NXT
	return nMotorEncoderTarget[m] != runStateIdle;
#else
	return getMotorRunning(m);
#endif
}

void setTarget(MotorPort m, int target, int mtrpower)
{
#ifdef NXT
	nMotorEncoderTarget[m] = target;
	motor[m] = mtrpower;
#else
	setMotorTarget(m, target, mtrpower);
#endif
}
// turning the robot right a certain amount of degrees
void turnRight(float deg)
{
	//	float dege=Wb/diameter*deg;
	float Cmperturn=wheelbase*PI*(deg/360) * cm * 3;
	resetEncoders();
	setTarget(LMotor, Cmperturn, forward);
	setTarget(RMotor, Cmperturn, -forward);
	while(stillRunning(LMotor)){}
	stopMotors();
}
//turning left a certain amount of degrees
void turnLeft(float deg)
{
	float Cmperturn=wheelbase*PI*(deg/360) * cm * 3;
	resetEncoders();
	setTarget(LMotor, Cmperturn, -forward);
	setTarget(RMotor, Cmperturn, forward);
	while(stillRunning(RMotor)){}
	stopMotors();
}

// going straight a certain amount of centimeters
void goStraight(float dist)
{
	resetEncoders();
	dist *= cm  * 3.15;
	setMotorTarget(LMotor, dist, forward);
	setMotorTarget(RMotor, dist, forward);
	while(stillRunning(LMotor)){}
	stopMotors();
}
// going back a certain amount of centimeters
void goBack(float dist)
{
	resetEncoders();
	dist *= cm  * 3.15;
	setTarget(LMotor, dist, -forward);
	setTarget(RMotor, dist, -forward);
	while(stillRunning(LMotor)){}
	stopMotors();
}

void rotateLeft(float degrees)
{
	nMotorEncoder[UMotor] = 0;
	setMotorTarget(UMotor, 9072 * (degrees/360), 40);
	while(stillRunning(UMotor)){}
}

void rotateRight(float degrees)
{
	nMotorEncoder[UMotor] = 0;
	setMotorTarget(UMotor, 9072 * (degrees/360), -40);
	while(stillRunning(UMotor)){}
}

Color getColor(ColorSensor x)
{
	int rgbSensor[3];
	getColorRGB(x.port,rgbSensor[0],rgbSensor[1],rgbSensor[2]);
	if (rgbSensor[2] == 0)
		return cBlack;
	float ratio = (float) rgbSensor[1]/ rgbSensor[0];
#ifdef DEBUG
	if(x == Leye)
		println(1,"RGB: %d %.1f %.1f %.2f", rgbSensor[1],x.g[0],x.g[1], ratio);
	if(x == Reye)
		println(3,"RGB: %d %.1f %.1f %.2f", rgbSensor[1],x.g[0],x.g[1], ratio);
#endif
	if (rgbSensor[1] > x.g[1])
		return cWhite;
	if (rgbSensor[1] < x.g[0])
		return cBlack;
	if (ratio >= x.rgb[0])
		return cGreen;
	return cGradient;
}

int getDigit(int src, int place)
{
	return (src / place) % 10;
}

bool getColor(LightSensor x)
{
	int y = SensorValue[x.port];
	println(2,"%d",y);
	return y < x.threshold;
}

void allign()
{
	while (getColor(Reye) ==  cBlack)
	{
		motor[LMotor] = back;
		motor[RMotor] = forward;
	}
	while (getColor(Leye) ==  cBlack)
	{
		motor[LMotor] = forward;
		motor[RMotor] = back;
	}
}
//end
#endif
