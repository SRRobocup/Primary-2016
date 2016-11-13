#include "Global.h"
const int LEFTBLACK = 100;
const int LEFTGREEN = 200;
const int MIDDLEBLACK = 10;
const int RIGHTBLACK = 1;
const int RIGHTGREEN = 2;
const int NONE = 0;
unsigned int lastState = 0;
unsigned int lastTurn = NONE;
int timeSinceSetTurn = 0;
float integral = 0;
int timeTurned = 0;

unsigned int getState()
{
	int x = ((int)getColor(Leye) * 100) + ((int)getBoolColor(Meye) * 10) + (int)getColor(Reye);
	return x;
}

/**
* A function for two eye PID based on the difference between the two sensors
*
* @param kp The knob proportion
* @param tp The target power
* @param greenL The value of green to be used in the function for the left eye
* @param greenR The value of green to be used in the function for the right eye
*/
void twoEyePID(float kp = 0.7, int tp = straight, int greenL = -1,int greenR = -1)
{
	short rgbL[4], rgbR[4];
	if(greenL == -1)
	{
		getColorSensorData(Leye.port, colorRaw, &rgbL[0]);
		greenL = rgbL[1];
	}
	if(greenR == -1)
	{
		getColorSensorData(Reye.port, colorRaw, &rgbR[0]);
		greenR = rgbR[1];
	}
	float curr = (Reye.mthreshold-greenR)-(Leye.mthreshold-greenL);
	if(abs(curr) < 10)
	{
		writeDebugStreamLine("RESET");
		integral = 0;
	}
	float ki = kp/10;
	float err = curr * kp;
	motor[LMotor] = tp + err; //+ integral*ki;
	motor[RMotor] = tp - err; //- integral*ki;
	integral += curr;
}

void twoEyeSharp(unsigned int state = 1000)
{
	float kp = 0.7;
	static float lerr = 0;
	static float rerr = 0;
	if (state == 1000)
	{
		state = getState();
		writeDebugStream("ONLY BASIC, STATE: %d ",state);
	}
	switch (state)
	{
	case LEFTBLACK:
		rerr = 0;
		println(0, "State: LEFT BLACK");
		writeDebugStreamLine("LEFT BLACK!!!!!!!!!!%d", lerr);
		twoEyePID();
		//lerr += (Leye.mthreshold - getValue(Leye));
		//motor[LMotor] = back-(lerr * kp);
		//motor[RMotor] = forward;
		break;
	case RIGHTBLACK:
		lerr = 0;
		println(0, "State: RIGHT BLACK");
		writeDebugStreamLine("RIGHT BLACK!!!!!%d", rerr);
		twoEyePID();
		//rerr += (Reye.mthreshold - getValue(Reye));

		//motor[LMotor] = forward;
		//motor[RMotor] = back - (rerr * kp);
		//if(!inhibitSet)
		//{
		//	lastTurn = RIGHTBLACK;
		//	timeSinceSetTurn = nPgmTime;
		//}
		//case LEFTBLACK:
		//case RIGHTBLACK:
		//	twoEyePID(kp,tp);
		break;
	default:
		lerr = 0;
		rerr = 0;
		println(0, "State: STRAIGHT");
		writeDebugStreamLine("STRAIGHT");
		motor[LMotor] = straight;
		motor[RMotor] = straight;
		break;
	}
	lastState = state;
}

void arrayPID()
{
	ubyte values[8];
	float kp = 0.004;
	int tp = 30;
	int curr;
	MSLSAreadSensors(splitter,values);
	curr = (upperLimit - clamp(values[0])) * 500 +
	(upperLimit - clamp(values[1])) * 100 +
	(upperLimit - clamp(values[2])) * 10 +
	(upperLimit - clamp(values[3])) * 1 +
	(upperLimit - clamp(values[4])) * -1 +
	(upperLimit - clamp(values[5])) * -10 +
	(upperLimit - clamp(values[6])) * -100 +
	(upperLimit - clamp(values[7])) * -500;
	motor[motorA] = tp + curr*kp;
	motor[motorC] = tp - curr*kp;
}

void twoEyeSimple(unsigned int state = 1000,int sideDisabled = 0)
{
	int tp = 50;
	int spd = 0;
	float kp = 0.7;
	static int rtimeTurned = 0;
	static int ltimeTurned = 0;

	if (state == 1000)
		state = getState();
	if (state == 300 && lastTurn == LEFTBLACK)
		state = LEFTBLACK;
	if (state == 003 && lastTurn == RIGHTBLACK)
		state = RIGHTBLACK;
	if (state == 103)// || state == 300)
		state = LEFTBLACK;
	if (state == 301)// || state == 003)
		state = RIGHTBLACK;
	if (sideDisabled == 1 && state == LEFTBLACK)
		state = (state % 100);
	if (sideDisabled == 2 && state == RIGHTBLACK)
		state = (state/10)*10;

	switch (state)
	{
	case LEFTBLACK:
		println(0, "State: LEFT BLACK");
		writeDebugStreamLine("LEFT BLACK!!!!!!!!!!%d", ltimeTurned);
		++ltimeTurned;

		spd = back + ltimeTurned*(-40);
		spd = (spd < -100 ? -100 : spd) ;
		motor[LMotor] = spd;
		motor[RMotor] = forward;
		//if(!inhibitSet)
		//{
		//	lastTurn = LEFTBLACK;
		//	timeSinceSetTurn = nPgmTime;
		rtimeTurned=0;
		break;
	case RIGHTBLACK:
		ltimeTurned = 0;

		println(0, "State: RIGHT BLACK");
		writeDebugStreamLine("RIGHT BLACK!!!!!%d", rtimeTurned);
		++rtimeTurned;

		spd = back + rtimeTurned*(-25);
		spd = (spd < -100 ? -100 : spd) ;

		motor[LMotor] = forward;
		motor[RMotor] = spd;
		//if(!inhibitSet)
		//{
		//	lastTurn = RIGHTBLACK;
		//	timeSinceSetTurn = nPgmTime;
		//}
		//case LEFTBLACK:
		//case RIGHTBLACK:
		//	twoEyePID(kp,tp);
		break;
	default  :
		ltimeTurned = 0;
		rtimeTurned = 0;
		if  (state /100 == 0  && state%10 ==0)
			// both white
		{
			println(0, "State: STRAIGHT");
			writeDebugStreamLine("STRAIGHT");
			motor[LMotor] = straight;
			motor[RMotor] = straight;
		}
		else
		{
			motor[LMotor] = ( motor[LMotor] < 0  ? back : forward);
			motor[RMotor] = ( motor[RMotor] < 0  ? back : forward);
			writeDebugStreamLine("UGH!!!!!! %d", state);
		}
		break;
	}
	lastState = state;
}

/**
* The Basic Two Eye Ad-hoc method of Linetracing
*/
void twoEyeBasic(int sideDisabled = 0, unsigned int state = 1000)
{
	int tp = 50;
	float kp = 0.7;
	bool inhibitSet = false;
	if (state == 1000)
	{
		state = getState();
		writeDebugStream("ONLY BASIC, STATE: %d",state);
	}
	//if (state == 300 && lastTurn == LEFTBLACK)
	//	state = LEFTBLACK;
	//if (state == 003 && lastTurn == RIGHTBLACK)
	//	state = RIGHTBLACK;
	if (state == 103|| state == 300)
		state = LEFTBLACK;
	if (state == 301|| state == 003)
		state = RIGHTBLACK;
	if (sideDisabled == 1 && state == LEFTBLACK)
		state = (state % 100);
	if (sideDisabled == 2 && state == RIGHTBLACK)
		state = (state/10)*10;
	writeDebugStream(" AFTER: %d ",state);
	//writeDebugStreamLine("%d",state);
	switch (state)
	{
	case LEFTBLACK:
		println(0, "State: LEFT BLACK");
		writeDebugStreamLine("LEFT BLACK");
		if(lastState != state)
			timeTurned = nPgmTime;
		else if(nPgmTime - timeTurned >= 500)
		{
			//playSound(soundFastUpwardTones);
		}
		motor[LMotor] = back;
		motor[RMotor] = forward;
		if(!inhibitSet)
		{
			lastTurn = LEFTBLACK;
			timeSinceSetTurn = nPgmTime;
		}
		break;
	case RIGHTBLACK:
		println(0, "State: RIGHT BLACK");
		writeDebugStreamLine("RIGHT BLACK");
		if(lastState != state)
			timeTurned = nPgmTime;
		else if(nPgmTime - timeTurned >= 500)
		{
			//playSound(soundFastUpwardTones);
		}
		motor[LMotor] = forward;
		motor[RMotor] = back;
		if(!inhibitSet)
		{
			lastTurn = RIGHTBLACK;
			timeSinceSetTurn = nPgmTime;
		}
		//case LEFTBLACK:
		//case RIGHTBLACK:
		//	twoEyePID(kp,tp);
		break;
	default:
		println(0, "State: STRAIGHT");
		writeDebugStreamLine("STRAIGHT");
		motor[LMotor] = straight;
		motor[RMotor] = straight;
		//twoEyePID(0.3,50);
		break;
	}
	lastState = state;
}

/**
* The final form of line tracing with exceptions
*/
void twoEye()
{
	static int lastIncline = 0;
	static int statesSinceInclineChange = 8;
	int tp = 50;
	float kp = 0.566038;
	short deg = 20;
	if(getIncline() > deg && (lastState != RIGHTBLACK || lastState != LEFTBLACK))
	{
		writeDebugStream("UP? ");
		if (getIncline() <= deg || getIncline() <= deg || getIncline() <= deg)
		{
			writeDebugStreamLine("NO RAMP");
			return;
		}
		println(7,"UP RAMP");
		writeDebugStreamLine("UP RAMP!!!!!!!!!!!");
		lastIncline = 1;
		statesSinceInclineChange = 6;
		playSound(soundUpwardTones);
		straight = 30;
		forward = 20;
		back = 0;
		while(getIncline() > deg)
		{
			twoEyeBasic();
			delay(50);
		}
		resetEncoders();
		if (!seeLine())
			while(!seeBlackArray())
			{
				motor[LMotor] = -straight;
				motor[RMotor] = -straight;
			}
		stopMotors();
		while(!seeLine())
			arrayPID();
	}
	else if(getIncline() < -deg && (lastState != RIGHTBLACK || lastState != LEFTBLACK))
	{
		writeDebugStream("DOWN? ");
		//|| getIncline() >= -deg
		if (getIncline() >= -deg || getIncline() >= -deg || getIncline() >= -deg)
		{
			writeDebugStreamLine("NO RAMP");
			return;
		}
		println(7,"DOWN RAMP");
		writeDebugStreamLine("DOWN RAMP!!!!!!!!!!!!!");
		lastIncline = -1;
		statesSinceInclineChange = 6;
		playSound(soundDownwardTones);
		straight = 20;
		forward = 20;
		back = 0;
		stopMotors();
		int begin = nPgmTime;
		while(getIncline() < -deg && nPgmTime - begin < 1000){}
		if(!seeLine())
		{
			resetEncoders();
			motor[RMotor] = 20;
			motor[LMotor] = -20;
			while(getColor(Meye) != cBlack && nMotorEncoder[RMotor] < wheelbase*PI/8*cm){}
			if(getColor(Meye) == cBlack)
				return;
			turnRight(45);
			resetEncoders();
			motor[RMotor] = -20;
			motor[LMotor] = 20;
			while(getColor(Meye) != cBlack && nMotorEncoder[LMotor] < wheelbase*PI/8*cm){}
			if(getColor(Meye) == cBlack)
				return;
			turnLeft(45);
		}
		while(getIncline() < -deg)
		{
			twoEyeBasic();
			delay(50);
		}
		straight = 30;
		while(getIncline() < -10)
		{
			twoEyeBasic();
			delay(50);
		}
	}
	else
	{
		println(7,"FLAT");
		writeDebugStream("FLAT ");
		if(statesSinceInclineChange > 0)
		{
			statesSinceInclineChange--;
		}
		else
			lastIncline = 0;
		straight = 30;
		forward = 40;
		back = -24;
	}
	if(nPgmTime-timeSinceSetTurn >  1500)
		lastTurn = NONE;
	int green = -forward; //-17

	//short rgbL[4],rgbR[4];
	//getColorSensorData(Leye.port,colorRaw,&rgbL[0]);
	//getColorSensorData(Reye.port,colorRaw,&rgbR[0]);
	unsigned int state = getState();
	//if(rgbL[0] == 0 || rgbR[0] == 0)
	//	return;
	//float ratioL = (float) rgbL[1]/ (float)rgbL[0];
	//float ratioR = (float) rgbR[1]/ (float)rgbR[0];
	//println(1,"RGB: %d %d %.1f %.1f %.2f", rgbL[1], rgbL[0],Leye.thresh[0],Leye.thresh[1], ratioL);
	//writeDebugStreamLine("Leye: %d %d %.1f %.1f %.2f", rgbL[1], rgbL[0],Leye.thresh[0],Leye.thresh[1], ratioL);
	//println(3,"RGB: %d %d %.1f %.1f %.2f", rgbR[1], rgbR[0],Reye.thresh[0],Reye.thresh[1], ratioR);
	//writeDebugStreamLine("Reye: %d %d %.1f %.1f %.2f", rgbR[1], rgbR[0],Reye.thresh[0],Reye.thresh[1], ratioR);
	//state = (int)getColor(Meye)*10;
	//if (rgbL[1] < Leye.thresh[0]) state += (int)cBlack * 100;
	//else if (rgbL[1] > Leye.thresh[1]) state += (int)cWhite * 100;
	//else if (ratioL >= Leye.ratio) state += (int)cGreen * 100;
	//else state += (int)cGradient * 100;
	//if (rgbR[1] < Reye.thresh[0]) state += (int)cBlack;
	//else if (rgbR[1] > Reye.thresh[1]) state += (int)cWhite;
	//else if (ratioR >= Reye.ratio) state += (int)cGreen;
	//else state += (int)cGradient;
	if(getDigit(state,100) == cGreen) state = LEFTGREEN;
	if(getDigit(state,1) == cGreen) state = RIGHTGREEN;
	if(state == 0 && seeBlackArray())
	{
		writeDebugStream("GAP? ");
		//if(lastState == LEFTBLACK || lastState == RIGHTBLACK)
		//	state = lastState;
		//else
		state = 010;
	}
	println(5,"%d",state);
	writeDebugStream("STATE: %d ",state);
	if(lastIncline == 1 && state == 0)
	{
		while(seeBlackArray())
		{
			arrayPID();
		}
		state = 010;
	}
	float deg1;
	switch (state)
	{
		//case LEFTBLACK + RIGHTBLACK:
		//	goStraight(5);
		//	resetEncoders();
		//	while(getColor(Leye) != cBlack)
		//	{
		//		motor[LMotor] = -forward;
		//		motor[RMotor] = forward;
		//	}
		//	int tempEncoders = abs(nMotorEncoder[LMotor]);
		//	resetEncoders();
		//	while(getColor(Reye) != cBlack)
		//	{
		//		motor[LMotor] = forward;
		//		motor[RMotor] = -forward;
		//	}
		//	if(nMotorEncoder[LMotor]-tempEncoders > tempEncoders)
		//	{
		//		while(getColor(Leye) != cBlack)
		//		{
		//			motor[LMotor] = -forward;
		//			motor[RMotor] = forward;
		//		}
		//	}
		//	break;
	case NONE:
		println(0, "State: GAP");
		writeDebugStreamLine("GAP");
		resetEncoders();
		if(lastState == RIGHTBLACK || lastState == LEFTBLACK)
		{
			motor[LMotor] = -straight;
			motor[RMotor] = -straight;
			while((getColor(Leye) == cWhite || getColor(Leye) == cGradient) && (getColor(Reye) == cWhite || getColor(Reye) == cGradient)){}
		}
		int begin = nPgmTime;
		//while(seeBlackArray() && nPgmTime - begin < 2000)
		//{
		//	if(getColor(Meye) || getColor(Leye) == cGreen || getColor(Reye) == cGreen) return;
		//	twoEyePID(kp,tp);
		//}
		//motor[LMotor] = -straight;
		//motor[RMotor] = -straight;
		//while(nMotorEncoder[LMotor] > -8*cm)
		//{
		//	if(getColor(Leye) != cWhite || getColor(Reye) != cWhite)
		//		return;
		//}
		//goStraight(8);
		motor[LMotor] = straight;
		motor[RMotor] = straight;
		while(nMotorEncoder[LMotor] < 6*cm)
		{
			if(seeLine())
			{
				writeDebugStreamLine("GOT OUT OF GAP: %d",getState());
				return;
			}
		}
		playSound(soundLowBuzzShort);
		stopMotors();
		delay(100);
		SensorType[Leye.port] = sensorNone;
		SensorType[Reye.port] = sensorNone;
		wait1Msec(100);
		ubyte values[8];
		tp = 30;
		kp = 0.005;
		int backDist = 4;
		for(int i = 1; i <= 5; i++)
		{
			writeDebugStreamLine("GAP: ALLIGN");
			motor[LMotor] = -straight;
			motor[RMotor] = -straight;
			while(!seeBlackArray()){delay(50);}
			stopMotors();
			wait1Msec(100);
			//if (i > 3)
			//	backDist = 3;
			goBack(backDist);
			stopMotors();
			begin = nPgmTime;
			while(seeBlackArray())
			{
				arrayPID();
				if (nPgmTime - begin > 1500)
				{
					writeDebugStreamLine("FALSE GAP");
					SensorType[Leye.port] = sensorColorNxtFULL;
					SensorType[Reye.port] = sensorColorNxtFULL;
					wait1Msec(100);
					if(!seeLine())
					{
						while(getColor(Leye))
						{
							motor[LMotor] = -forward;
							motor[RMotor] = forward;
						}
					}
					return;
				}
			}
		}
		stopMotors();
		wait1Msec(100);
		//turnLeft(12);
		//if(lastTurn == LEFTBLACK)
		//	turnLeft(12);
		//if (lastTurn == RIGHTBLACK)
		//	turnRight(12);
		//turnRight(15);
		writeDebugStreamLine("GAP: FORWARD");
		SensorType[Leye.port] = sensorColorNxtFULL;
		SensorType[Reye.port] = sensorColorNxtFULL;
		wait1Msec(100);
		motor[LMotor] = forward;
		motor[RMotor] = forward;
		while(seeBoolLine()){}
		writeDebugStreamLine("MIDDLE OF FORWARD: %d",getState());
		while((getColor(Leye) == cWhite || getColor(Leye) == cGradient) && (getColor(Reye) == cWhite || getColor(Reye) == cGradient) && getColor(Meye) != cSilver)
		{
			//writeDebugStreamLine("WAITING FOR LINE: %d",getState());
			if(getBoolColor(Meye) == cBlack)
			{
				motor[LMotor] = straight;
				motor[RMotor] = straight;
			}
			else
			{
				motor[LMotor] = forward;
				motor[RMotor] = forward;
			}
		}
		stopMotors();
		delay(100);
		//writeDebugStreamLine("FINISHED GAP: %d", getState());
		lastState = state;
		break;
		//case RIGHTBLACK + LEFTBLACK:
		//	writeDebugStreamLine("LEFT AND RIGHT BLACK");
		//	goStraight(1);
		//	ubyte valu[8];
		//	MSLSAreadSensors(splitter,valu);
		//	int left = 0;
		//	int right = 0;
		//	for(int i = 0; i < 4; i++)
		//	{
		//		writeDebugStream("VALUE %d: %d ",i,valu[i]);
		//		if(valu[i] > 40)
		//		{
		//			writeDebugStreamLine("WHITE");
		//			left++;
		//		}
		//		else
		//			writeDebugStreamLine("BLACK");
		//	}
		//	for(int i = 4; i < 8; i++)
		//	{
		//		writeDebugStream("VALUE %d: %d ",i,valu[i]);
		//		if(valu[i] > 40)
		//		{
		//			writeDebugStreamLine("WHITE");
		//			right++;
		//		}
		//		else
		//			writeDebugStreamLine("BLACK");
		//	}
		//	goStraight(5.5);
		//	if(right > left)
		//	{
		//		motor[LMotor] = forward;
		//		motor[RMotor] = -forward;
		//		while(getColor(Meye) != cBlack){}
		//	}
		//	else if(left > right)
		//	{
		//		motor[LMotor] = -forward;
		//		motor[RMotor] = forward;
		//		while(getColor(Meye) != cBlack){}
		//	}
		//	else
		//	{
		//		playSound(soundBeepBeep);
		//		while(true)
		//		{
		//			stopMotors();
		//		}
		//	}
		//	break;
	case LEFTBLACK + RIGHTBLACK:
		println(0, "State: LEFT AND RIGHT BLACK");
		writeDebugStreamLine("LEFT AND RIGHT BLACK");
		if (lastState == RIGHTBLACK || lastState == LEFTBLACK)
		{
			motor[LMotor] = straight;
			motor[RMotor] = straight;
		}
		else
			//{
		//	stopMotors();
		//	resetEncoders();
		//	delay(100);
		//	motor[LMotor] = 30;
		//	while(getColor(Leye) == cBlack || getColor(Leye) == cGradient){}
		//	writeDebugStreamLine("LEYE: %d",getColor(Leye));
		//	if(getColor(Leye) == cGreen)
		//	{
		//		while(getColor(Leye) == cGreen){}
		//		return;
		//	}
		//	motor[LMotor] = -30;
		//	while(nMotorEncoder[LMotor] > 0){}
		//	stopMotors();
		//	motor[RMotor] = 30;
		//	while(getColor(Reye) == cBlack || getColor(Reye) == cGradient){}
		//	writeDebugStreamLine("REYE: %d",getColor(Reye));
		//	if(getColor(Reye) == cGreen)
		//	{
		//		while(getColor(Reye) == cGreen){}
		//		return;
		//	}
		//	motor[RMotor] = -30;
		//	while(nMotorEncoder[RMotor] > 0){}
		//	stopMotors();
		//	if(lastTurn == LEFTBLACK)
		//	{
		//		motor[RMotor] = 30;
		//		while(getColor(Reye) != cBlack){}
		//	}
		//	else if (lastTurn == RIGHTBLACK)
		//	{
		//		motor[LMotor] = 30;
		//		while(getColor(Leye) != cBlack){}
		//	}
		//}
		//if(lastTurn == LEFTBLACK)
		//{
		//	motor[LMotor] = 30;
		//	motor[RMotor] = 0;
		//	while(getColor(Leye) == cBlack){}
		//	while(getColor(Leye) != cBlack){}
		//}
		//else if(lastTurn == RIGHTBLACK)
		//{
		//	motor[LMotor] = 0;
		//	motor[RMotor] = 30;
		//	while(getColor(Reye) == cBlack){}
		//	while(getColor(Reye) != cBlack){}
		//}
		{
			motor[LMotor] = -straight;
			motor[RMotor] = -straight;
			while(getColor(Leye) == cBlack || getColor(Reye) == cBlack || (getColor(Reye) == cGradient&&getColor(Leye) == cGradient)){}
			if (getColor(Leye) == cGreen || getColor(Reye) == cGreen)
			{
				return;
			}
			motor[LMotor] = straight;
			motor[RMotor] = straight;
			while(getColor(Leye) != cBlack || getColor(Reye) != cBlack){}
			while(getColor(Leye) == cBlack || getColor(Reye) == cBlack){}
		}
		break;
	case RIGHTGREEN + MIDDLEBLACK:
	case RIGHTGREEN:
		println(0, "State: RIGHT GREEN");
		writeDebugStreamLine("RIGHT GREEN");
		playSound(soundBlip);
		if(getDigit(lastState,1) == cBlack)
		{
			writeDebugStreamLine("FALSE GREEN RIGHT BLACK");
			while(getColor(Reye) == cGreen)
			{
				twoEyeBasic(2);
			}
			return;
		}
		while(getColor(Reye) != cWhite && getColor(Reye) != cBlack)
		{
			twoEyeBasic(2);
		}
		delay(100);
		if (getColor(Reye) == cWhite)
		{
			playSound(soundBeepBeep);
			writeDebugStreamLine("FALSE GREEN");
			if (getState() == NONE)
			{
				motor[LMotor] = forward;
				motor[RMotor] = -forward;
				while(getColor(Reye) != cBlack){}
			}
			return;
		}
		goStraight(5);
		resetEncoders();
		motor[RMotor] = green;
		motor[LMotor] = forward;
		while(getColor(Meye) != cWhite){}
		while(getColor(Meye) != cBlack && nMotorEncoder[LMotor] < wheelbase*PI/4*cm){} //|| ){}
		while(getColor(Reye) == cBlack || getColor(Reye) == cGradient){}
		//deg1 = (nMotorEncoder[LMotor]/cm)/(wheelbase*PI)*360;
		//writeDebugStreamLine("AMOUNT TURNED: %f",deg1);
		//if (deg1 < 20)
		//{
		//	resetEncoders();
		//	while(getColor(Meye) != cWhite){}
		//	while(getColor(Meye) != cBlack && nMotorEncoder[LMotor] < wheelbase*PI/4*cm){} //|| ){}
		//	while(getColor(Reye) == cBlack || getColor(Reye) == cGradient){}
		//}
		turnRight(12);
		if (getColor(Reye) == cBlack)
			return;
		turnLeft(12);
		stopMotors();
		playSound(soundBlip);
		motor[RMotor] = straight;
		motor[LMotor] = straight;
		while(getColor(Reye) == cGreen){}
		lastState = state;
		break;
	case LEFTGREEN + MIDDLEBLACK:
	case LEFTGREEN:
		println(0, "State: LEFT GREEN");
		writeDebugStreamLine("LEFT GREEN");
		playSound(soundBlip);
		if(getDigit(lastState,100) == cBlack)
		{
			writeDebugStreamLine("FALSE GREEN LEFT BLACK");
			while(getColor(Leye) == cGreen)
			{
				twoEyeBasic(1);
			}
			return;
		}
		while(getColor(Leye) != cWhite && getColor(Leye) != cBlack)
		{
			twoEyeBasic(1);
		}
		delay(100);
		if (getColor(Leye) == cWhite)
		{
			playSound(soundBeepBeep);
			writeDebugStreamLine("FALSE GREEN");
			if (getState() == NONE)
			{
				motor[LMotor] = -forward;
				motor[RMotor] = forward;
				while(getColor(Leye) != cBlack){}
			}
			return;
		}
		goStraight(5);
		resetEncoders();
		motor[RMotor] = forward;
		motor[LMotor] =	green;
		while(getColor(Meye) != cWhite){}
		while(getColor(Meye) != cBlack && nMotorEncoder[RMotor] < wheelbase*PI/4*cm){} //|| ){}
		while(getColor(Leye) == cBlack || getColor(Leye) == cGradient){}
		deg1 = (nMotorEncoder[RMotor]/cm)/(wheelbase*PI)*360;
		writeDebugStreamLine("AMOUNT TURNED: %f",deg1);
		if (deg1 < 20)
		{
			resetEncoders();
			while(getColor(Meye) != cWhite){}
			while(getColor(Meye) != cBlack && nMotorEncoder[RMotor] < wheelbase*PI/4*cm){}
			while(getColor(Leye) == cBlack || getColor(Leye) == cGradient){}
		}
		turnLeft(12);
		if (getColor(Leye) == cBlack)
			return;
		turnRight(12);
		stopMotors();
		playSound(soundBlip);
		motor[RMotor] = straight;
		motor[LMotor] = straight;
		while(getColor(Leye) == cGreen){}
		lastState = state;
		break;
		//case LEFTBLACK + RIGHTBLACK:
		//	stopMotors();
		//	wait1Msec(100);
		//	int encoder = 0;
		//	int direction = lastTurn == RIGHTBLACK ? -1 : 1;
		//	resetEncoders();
		//	motor[LMotor] = -forward * direction;
		//	motor[RMotor] = forward * direction;
		//	while(getColor(Meye) != cBlack){}
		//	stopMotors();
		//	encoder = abs(nMotorEncoder[RMotor]);
		//	motor[LMotor] = forward * direction;
		//	motor[RMotor] = -forward  * direction;
		//	while(getColor(Meye) == cBlack){}
		//	while(getColor(Meye) != cBlack){}
		//	stopMotors();
		//	if(abs(nMotorEncoder[LMotor] - encoder) > encoder)
		//	{
		//		motor[LMotor] = -forward * direction;
		//		motor[RMotor] = forward * direction;
		//		while(getColor(Meye) == cBlack){}
		//		while(getColor(Meye) != cBlack){}
		//	}
		//	writeDebugStreamLine("DIR 1: %d DIR 2: %d",encoder,abs(nMotorEncoder[LMotor] - encoder));
		//	break;
		//case MIDDLEBLACK + LEFTBLACK:
		//case MIDDLEBLACK + RIGHTBLACK:
		//	motor[LMotor] = forward;
		//	motor[RMotor] = forward;
		//	break;
		//default:
		//	println(0, "State: PID");
		//	writeDebugStream("PID ");
		//	twoEyePID(kp,tp,rgbL[1],rgbR[1]);
		//	writeDebugStreamLine("LMOTOR: %d RMOTOR: %d",motor[LMotor],motor[RMotor]);
		//	break;
	default:
		twoEyeBasic(0,state);
		break;
	}
}
