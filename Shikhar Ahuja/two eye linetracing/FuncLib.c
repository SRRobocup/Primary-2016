#include "Global.h"

bool stillRunning(MotorPort m)
{
#ifdef NXT
	return nMotorEncoderTarget[m] != runStateIdle;
#else
	return getMotorRunning(m);
#endif
}
// syncing the motors to make the turns more accurate
void syncMotors(MotorPort master, MotorPort slave, float turnRatio)
{
#ifdef NXT
	if (master == motorA && slave == motorC)
		nSyncedMotors = synchAC;
	else if (master == motorC && slave == motorA)
		nSyncedMotors = synchCA;
	else if (master == motorB && slave == motorC)
		nSyncedMotors = synchBC;
	else if (master == motorC && slave == motorB)
		nSyncedMotors = synchCB;
	else if (master == motorA && slave == motorB)
		nSyncedMotors = synchAB;
	else if (master == motorB && slave == motorA)
		nSyncedMotors = synchBA;
	nSyncedTurnRatio = turnRatio;
#else
	setMotorSync(master, slave, turnRatio, 0);
#endif
}
//resetting the sync of the motors
void resetSync()
{
#ifdef NXT
	nSyncedMotors = synchNone;
#else
	setMotorSync(motorA, motorA, 100, 0);
	setMotorSync(motorB, motorB, 100, 0);
	setMotorSync(motorC, motorC, 100, 0);
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
	float Cmperturn=wheelbase*PI*(deg/360);
	nMotorEncoder[LMotor]=0;
	setTarget(LMotor, Cmperturn*cm, tpower);
	setTarget(RMotor, Cmperturn*cm, -tpower);
	while(stillRunning(LMotor))
	{
		println(0,"L Encoder: %d",nMotorEncoder[LMotor]);
	}
}
//turning left a certain amount of degrees
void turnLeft(float deg)
{
	//	float dege=Wb/diameter*deg;
	float Cmperturn=wheelbase*PI*(deg/360);
	nMotorEncoder[RMotor]=0;
	setTarget(LMotor, Cmperturn*cm, -tpower);
	setTarget(RMotor, Cmperturn*cm, tpower);
	motor[RMotor] = tpower;
	while(stillRunning(RMotor))
	{
		println(0,"R Encoder: %d",nMotorEncoder[RMotor]);
	}
}

// going straight a certain amount of centimeters
void goStraight(float dist)
{
	nMotorEncoder[LMotor]=0;
	setTarget(LMotor, dist*cm, tpower);
	setTarget(RMotor, dist*cm, tpower);
	while(stillRunning(LMotor)){}
}
// going back a certain amount of centimeters
void goBack(float dist)
{
	nMotorEncoder[LMotor]=0;
	setTarget(LMotor, dist*cm, -tpower);
	setTarget(RMotor, dist*cm, -tpower);
	motor[LMotor] = -tpower;
	while(stillRunning(LMotor)){}
}
// stopping
void stopMotors()
{
	motor[LMotor]=0;
	motor[RMotor]=0;
}

void roundFloat(float &x, int places)
{
	int power = pow(10, places);
	x *= power;
	x = (int) x;
	x /= power;
}

float getDistance(SensorPort x)
{
#ifdef NXT
	return SensorValue[x];
#else
	return getUSDistance(x);
#endif
}

//Color getColor(LightSensor x)
//{
//	float value = SensorValue[x.port];
//}
