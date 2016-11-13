#include "Global.h"
#include "LineTrace.h"
#pragma DebuggerWindows("EV3LCDScreen")
#ifndef DEBUG
#define DEBUG
#endif

task main()
{
	//writeDebugStream("ASdasfdahsfoasjd");
	generateColor(S1,56,148,283,1.96,&Leye);
	generateLight(S2,11,&Meye);
	generateColor(S3,48,128,268,2.53,&Reye);
	while (true)
	{
		println(0,"%d",getState());
		writeDebugStreamLine("%d",getState());
		if(getButtonPress(2))
			break;
		println(6,"Condition: %d",getState() != MIDDLEBLACK);
	}

}
