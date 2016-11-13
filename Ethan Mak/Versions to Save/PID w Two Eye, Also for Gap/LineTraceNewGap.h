#include "Global.h"
const int LEFTBLACK = 100;
const int LEFTGREEN = 200;
const int MIDDLEBLACK = 10;
const int RIGHTBLACK = 1;
const int RIGHTGREEN = 2;
const int NONE = 0;
unsigned int lastState = 0;
unsigned int lastTurn = 0;
float integral = 0;

unsigned int getState()
{
	int x = ((int)getColor(Leye) * 100) + ((int)getBoolColor(Meye) * 10) + (int)getColor(Reye);
	return x;
}

void twoEyePID(float kp = 0.7, int tp = 50, int greenL = -1,int greenR = -1)
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
	if(abs(curr) < 5)
	{
		writeDebugStreamLine("RESET");
		integral = 0;
	}
	float ki = kp/10;
	float err = curr * kp;
	motor[LMotor] = tp + err + integral*ki;
	motor[RMotor] = tp - err - integral*ki;
	integral += curr;
}

void twoEye()
{
	int tp = 50;
	float kp = 0.55;
	//if(getIncline() > 15)
	//{
	//	println(6,"RAMP");
	//	playSound(soundFastUpwardTones);
	//	tp = 60;
	//	kp = 0.3;
	//}
	//else
	//	println(6,"FLAT");
	int green = -forward; //-17
	short rgbL[4],rgbR[4];
	getColorSensorData(Leye.port,colorRaw,&rgbL[0]);
	getColorSensorData(Reye.port,colorRaw,&rgbR[0]);
	unsigned int state = 0;
	if(rgbL[0] == 0 || rgbR[0] == 0)
		return;
	float ratioL = (float) rgbL[1]/ rgbL[0];
	float ratioR = (float) rgbR[1]/ rgbR[0];
	println(1,"RGB: %d %d %.1f %.1f %.2f", rgbL[1], rgbL[0],Leye.thresh[0],Leye.thresh[1], ratioL);
	println(3,"RGB: %d %d %.1f %.1f %.2f", rgbR[1], rgbR[0],Reye.thresh[0],Reye.thresh[1], ratioR);
	state = (int)getColor(Meye)*10;
	if (rgbL[1] < Leye.thresh[0]) state += (int)cBlack * 100;
	else if (rgbL[1] > Leye.thresh[1]) state += (int)cWhite * 100;
	else if (ratioL >= Leye.ratio) state += (int)cGreen * 100;
	else state += (int)cGradient * 100;
	if (rgbR[1] < Reye.thresh[0]) state += (int)cBlack;
	else if (rgbR[1] > Reye.thresh[1]) state += (int)cWhite;
	else if (ratioR >= Reye.ratio) state += (int)cGreen;
	else state += (int)cGradient;
	if(getDigit(state,100) == cGreen) state = LEFTGREEN;
	if(getDigit(state,1) == cGreen) state = RIGHTGREEN;
	if(state == 0 && seeBlackArray())
		state = 010;
	println(5,"%d",state);
	writeDebugStream("STATE: %d ",state);
	switch (state)
	{
	case NONE:
		println(0, "State: GAP");
		writeDebugStreamLine("GAP");
		resetEncoders();
		int begin = nPgmTime;
		while(seeBlackArray() && nPgmTime - begin < 2000)
		{
			if(getColor(Meye) || getColor(Leye) == cGreen || getColor(Reye) == cGreen) return;
			twoEyePID(kp,tp);
		}
		motor[LMotor] = tp;
		motor[RMotor] = tp;
		while(nMotorEncoder[LMotor] < 15*cm){if(seeLine()) return;}
		playSound(soundLowBuzzShort);
		stopMotors();
		delay(100);
		repeat(5)
		{
			SensorType[Leye.port] = sensorNone;
			SensorType[Reye.port] = sensorNone;
			wait1Msec(100);
			motor[LMotor] = -straight;
			motor[RMotor] = -straight;
			while(!seeBlackArray()){delay(50);}
			stopMotors();
			wait1Msec(100);
			SensorType[Leye.port] = sensorColorNxtFULL;
			SensorType[Reye.port] = sensorColorNxtFULL;
			wait1Msec(100);
			goBack(5);
			stopMotors();
			begin = nPgmTime;
			while(seeBlackArray() && nPgmTime - begin < 2000)
			{
				writeDebugStreamLine("GAP: ALLIGN");
				twoEyePID(0.8,30);
			}
		}
		stopMotors();
		wait1Msec(100);
		//turnRight(15);
		writeDebugStreamLine("GAP: FORWARD");
		motor[LMotor] = forward;
		motor[RMotor] = forward;
		while(getColor(Leye) != cWhite || getColor(Reye) != cWhite){}
		while((getColor(Leye) == cWhite || getColor(Leye) == cGradient) && (getColor(Reye) == cWhite || getColor(Reye) == cGradient)){}
		writeDebugStreamLine("%d", getState());
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
	case RIGHTGREEN + MIDDLEBLACK:
	case RIGHTGREEN:
		println(0, "State: RIGHT GREEN");
		writeDebugStreamLine("RIGHT GREEN");
		//while(true){}
		playSound(soundBlip);
		while(getColor(Reye) != cWhite && getColor(Reye) != cBlack)
		{
			twoEyePID(kp,tp);
		}
		delay(100);
		if (getColor(Reye) == cWhite)
		{
			playSound(soundBeepBeep);
			writeDebugStreamLine("FALSE GREEN");
			return;
		}
		goStraight(5);
		resetEncoders();
		motor[RMotor] = green;
		motor[LMotor] = forward;
		while(getColor(Meye) != cWhite){}
		while(getColor(Meye) != cBlack && nMotorEncoder[LMotor] < wheelbase*PI/4*cm){}
		stopMotors();
		playSound(soundBlip);
		motor[RMotor] = straight;
		motor[LMotor] = straight;
		while(getColor(Reye) == cGreen){}
		break;
	case LEFTGREEN + MIDDLEBLACK:
	case LEFTGREEN:
		println(0, "State: LEFT GREEN");
		writeDebugStreamLine("LEFT GREEN");
		//while(true){}
		playSound(soundBlip);
		while(getColor(Leye) != cWhite && getColor(Leye) != cBlack)
		{
			twoEyePID(kp,tp);
		}
		delay(100);
		if (getColor(Leye) == cWhite)
		{
			playSound(soundBeepBeep);
			writeDebugStreamLine("FALSE GREEN");
			return;
		}
		goStraight(5);
		resetEncoders();
		motor[RMotor] = forward;
		motor[LMotor] =	green;
		while(getColor(Meye) != cWhite){}
		while(getColor(Meye) != cBlack && nMotorEncoder[RMotor] < wheelbase*PI/4*cm){}
		stopMotors();
		playSound(soundBlip);
		motor[RMotor] = straight;
		motor[LMotor] = straight;
		while(getColor(Leye) == cGreen){}
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
	case MIDDLEBLACK + LEFTBLACK:
	case MIDDLEBLACK + RIGHTBLACK:
		motor[LMotor] = forward;
		motor[RMotor] = forward;
		break;
	default:
		println(0, "State: PID");
		writeDebugStream("PID ");
		twoEyePID(kp,tp,rgbL[1],rgbR[1]);
		writeDebugStreamLine("LMOTOR: %d RMOTOR: %d",motor[LMotor],motor[RMotor]);
		break;
	}
	lastState = state;
}
