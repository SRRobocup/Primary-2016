#include "Global.h"
#include "LineTraceNewGap.h"
#include "Evac.h"



task main()
{
	while(getColor(Reye)!=cRed)
	{
		while(getColor(Meye)!=cSilver)
			twoEye();

		turnLeft(90);
		while(getDistance(Msonar)>5)
			PID();

		turnRight(90);
		while(getDistance(Msonar)>10&&getColor(Reye)!=cRed)
			PID();

		if(getColor(Reye)!=cRed)
		{
			PID();
			playSound(soundBeepBeep);
			writeDebugStreamLine("Have Missed Red");
			println(5,"Have missed Red");
		}
		else {

			continue;

		}

	}
	stopMotors();



}
