#include "Global.h"

void obstacle()
{
	int direction = 1; // 1 <= left, -1 => right
	stopMotors();
	float obstDia=7, inner=obstDia+(distObstacle*2), outer=inner+(wheelbase*2);
	float ratio= outer/inner;

	{
		float distL = getDistance(Lsonar);
		float distR = getDistance(Rsonar);
	direction = distL > distR ? 1 : -1;
	}
	if(direction == -1)
	{
		motor[RMotor] = straight * ratio;
		motor[LMotor] = straight;
		wait1Msec(500);
		while (getColor(Leye) == cBlack || getColor(Meye) == cBlack || getColor(Reye) == cBlack){}
	while (getColor(Leye) == cWhite){}

	motor[LMotor] = forward;
	motor[RMotor] = -forward;
	while(getColor(Meye) != cBlack){}
	}
	else
	{
	turnLeft(180);
	motor[LMotor] = straight * ratio;
	motor[RMotor] = straight;
	wait1Msec(500);
	while (getColor(Leye) == cBlack || getColor(Meye) == cBlack || getColor(Reye) == cBlack){}
	while (getColor(Leye) == cWhite){}

	motor[LMotor] = -forward;
	motor[RMotor] = forward;
	while(getColor(Meye) != cBlack){}
	}
}
