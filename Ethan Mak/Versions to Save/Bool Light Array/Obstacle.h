#include "Global.h"

void obstacle()
{
	int tp = 30;
	float err;
	float kp = 10.1;
	float curr;
	float prev = 200000;
	int dir = -1; //1 right , -1 left
	int rdist = clamp((int)getDistance(Rsonar), 30,0);
	int ldist = clamp((int)getDistance(Lsonar), 30,0);
	writeDebugStreamLine("LDIST: %d RDIST: %d", ldist,rdist);
	if(rdist > ldist)
		dir = 1;
	if(ldist > rdist)
		dir = =-1;
	if(dir = -1)
	{
		float target = 3;
		turnLeft(90);
		eraseDisplay();
		while(getColor(Reye) != cBlack)
		{
			curr = getDistance(Rsonar);
			if(curr - prev > 15)
			{
				motor[LMotor] = 20;
				motor[RMotor] = 30;
			}
			displayTextLine(3,"Sensors: %.2f %.2f",curr,prev);
			err = (curr - target)*kp;
			displayTextLine(4,"Error: %f",err);
			motor[LMotor] = tp + err;
			motor[RMotor] = tp - err;
			displayTextLine(5,"Motor: %d %d",motor[LMotor],motor[RMotor]);
		}
		stopMotors();
		resetEncoders();
		goStraight(3);
		motor[LMotor] = -tp;
		motor[RMotor] = tp;
		while(getColor(Meye)!=cBlack && nMotorEncoder[RMotor] < wheelbase*PI/4 *cm){}
		stopMotors();
	}
	else
	{
		float target = 3;
		turnRight(90);
		eraseDisplay();
		while(getColor(Reye) != cBlack)
		{
			curr = getDistance(Lsonar);
			if(curr - prev > 15)
			{
				motor[LMotor] = 20;
				motor[RMotor] = 30;
			}
			displayTextLine(3,"Sensors: %.2f %.2f",curr,prev);
			err = (curr - target)*kp;
			displayTextLine(4,"Error: %f",err);
			motor[LMotor] = tp - err;
			motor[RMotor] = tp + err;
			displayTextLine(5,"Motor: %d %d",motor[LMotor],motor[RMotor]);
		}
		stopMotors();
		resetEncoders();
		goStraight(3);
		motor[LMotor] = tp;
		motor[RMotor] = -tp;
		while(getColor(Meye)!=cBlack && nMotorEncoder[LMotor] < wheelbase*PI/4 *cm){}
		stopMotors();
	}
}
