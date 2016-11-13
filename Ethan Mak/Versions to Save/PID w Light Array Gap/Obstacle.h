#include "Global.h"

void obstacle()
{
	int tp = 30;
	float err;
	float kp = 8.1;
	float curr;
	float prev = 200000;
	if(getDistance(Lsonar) > getDistance(Rsonar))
	{
		float target = wheelbase/2 - length/2 + 2;
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
		while(getColor(Meye)!=cBlack && nMotorEncoder[RMotor] < wheelbase/4 *cm){}
		stopMotors();
	}
	else
	{
		float target = wheelbase/2 - length/2 + 2;
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
		while(getColor(Meye)!=cBlack && nMotorEncoder[LMotor] < wheelbase/4 *cm){}
		stopMotors();
	}
}
