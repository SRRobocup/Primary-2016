#include "Global.h"
task main()
{
	float deg = degreesToRadians(1);
	displayTextLine(3,"%f",radiansToDegrees(atan(sin(deg)/cos(deg))));
	while(true){}

}
