#include "Global.h"
#include "FuncLib.c"
float degreest=0;;
float getdy;
float getdx;

void Probing()
{
	getdx=getDistance(Ultrasonic);
goStraight(5);
	turnleft(90);
	getdy=getDistance(Ultrasonic);
turnRight(90);0
}
void Evac()
{

 Probing();
TurnSweep(degreest);
goStraight(getdx-8);

turnLeft(90);
gostraight(115-wheelbase);

degreest/=2;
goBack(5);
TurnSweep(-degreest);
turnLeft(22);
TurnSweep(-degreest);
turnleft(22);


}
