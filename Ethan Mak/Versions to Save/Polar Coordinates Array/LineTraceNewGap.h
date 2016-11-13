#include "Global.h"
const int LEFTBLACK = 100;
const int LEFTGREEN = 200;
const int MIDDLEBLACK = 10;
const int RIGHTBLACK = 1;
const int RIGHTGREEN = 2;
const int NONE = 0;
unsigned int lastState = 0;
unsigned int lastTurn = 0;

unsigned int getState()
{
	int x = ((int)getColor(Leye) * 100) + ((int)getColor(Meye) * 10) + (int)getColor(Reye);
	return x;
}

float arctan2(float y, float x)
{
	float deg = radiansToDegrees(atan2(y,x));
	if (deg > 180)
		return 360-deg;
	return deg;
}

void twoEye()
{
	int fast = 40;
	int green = -forward; //-17
	int state = getState();
	println(5,"%d",state);
	int temp;
	if (state != 303)
	{
		if(getDigit(state,1) == cGradient)
			if (lastState == RIGHTBLACK)
			state = RIGHTBLACK;
		else
		{
			temp = getDigit(state,100) * 100 + getDigit(state,10) * 10;
			state = temp == 0 ? getDigit(state,100) * 100 + getDigit(state,10) * 10 + RIGHTBLACK: temp;
		}
		//if(getDigit(state,10) == cGradient)
		//{
		//	temp = getDigit(state,100) * 100 + getDigit(state,1);
		//	state = temp;
		//}
		if(getDigit(state,100) == cGradient)
			if (lastState == LEFTBLACK)
			state = LEFTBLACK;
		else
		{
			temp = getDigit(state,10) * 10 + getDigit(state,1);
			state = temp == 0 ? LEFTBLACK + getDigit(state,10) * 10 + getDigit(state,1) : temp;
		}
	}
	if (state == 301)
		state = 1;
	if (state == 103)
		state = 100;
	switch (state)
	{
	case NONE:
		println(0, "State: GAP");
		resetEncoders();
		motor[LMotor] = straight;
		motor[RMotor] = straight;
		while(nMotorEncoder[LMotor] < 10*cm){ if(seeBlack()){playSound(soundBlip); return;}}
		playSound(soundBeepBeep);
		stopMotors();
		SensorType[Leye.port] = sensorNone;
		SensorType[Reye.port] = sensorNone;
		wait1Msec(100);
		stopMotors();
		ubyte temp[8];
		bool val[8];
		int begin=-1,end=-1,lastArr,lastBack;
		motor[LMotor] = -straight;
		motor[RMotor] = -straight;
		while(!seeBlackArray()){delay(50);}
		repeat(2)
		{
			stopMotors();
			repeat(3)
			{
				MSLSAreadSensors(splitter,temp);
				for (int i = 0; i < 8;i++)
					val[i] = val[i] || (temp[i] < arrThreshold);
			}
			for(int i=0;i<8;i++)
			{
				if (val[i] && begin == -1)
					begin = i;
				if (val[i])
					end = i;
			}
			println(5,"%d %d", begin, end);
			writeDebugStreamLine("B:%d E:%d",begin,end);
			wait1Msec(1000);
			float degree;
			bool last[2];
			if(!begin) begin = end;
			last[0] = getBoolArray(begin-1);
			last[1] = getBoolArray(end+1);
			motor[LMotor] = -20;
			motor[RMotor] = -20;
			while(getBoolArray(begin-1) != last[0] && getBoolArray(end+1) != last[1]){ sleep(50);}
			playSound(soundBlip);
			last[0] = getBoolArray(begin-1);
			last[1] = getBoolArray(end+1);
			float max;
			//float val[500];
			float sum = 0;
			float temp;
			float dist, dist1;
			float lastHrz = 0;
			int arr;
			int iteration = 0;
			int lastEncoder = nMotorEncoder[LMotor];
			resetEncoders();
			if(last[1])
			{
				max = getArray(end+2) - getArray(end+1);
				lastArr = getArray(end+2);
				lastBack = abs(nMotorEncoder[LMotor])/cm;
				writeDebugStreamLine("MAX: %d LASTARR: %d",max,lastArr);
				//for(int i = 0; i < 20; i++)
				while(nMotorEncoder[LMotor] > -5*cm)
				{
					//writeDebugStream("BEGIN: %d MIN: %d MAX: %d",nPgmTime,0,max);
					arr = abs(getArray(end+2) - lastArr);
					dist = map(arr,0,max,0,0.8);
					dist1 = abs(nMotorEncoder[LMotor])/cm;
					temp = arctan2(dist-lastHrz,dist1-lastBack);
					sum += temp;
					writeDebugStreamLine("DEG: %f HRZ: %f BAC: %f ARR: %d",temp,dist,dist1,arr);
					iteration++;
					lastEncoder = nMotorEncoder[LMotor];
					lastBack = dist1;
					lastHrz = dist;
					while(nMotorEncoder[LMotor] + 5 > lastEncoder && nMotorEncoder[LMotor] > -5*cm){}
				}
				stopMotors();
				playSound(soundBlip);
				degree=-sum/iteration;
			}
			else
			{
				max = getArray(begin-2) - getArray(begin-1);
				lastArr = getArray(begin-2);
				lastBack = abs(nMotorEncoder[LMotor])/cm;
				writeDebugStreamLine("MAX: %d LASTARR: %d",max,lastArr);
				//for(int i = 0; i < 20; i++)
				while(nMotorEncoder[LMotor] > -5*cm)
				{
					//writeDebugStream("BEGIN: %d MIN: %d MAX: %d",nPgmTime,0,max);
					arr = abs(getArray(begin-2) - lastArr);
					dist = map(arr,0,max,0,0.8);
					dist1 = abs(nMotorEncoder[LMotor])/cm;
					temp = arctan2(dist-lastHrz,dist1-lastBack);
					sum += temp;
					writeDebugStreamLine("DEG: %f HRZ: %f BAC: %f ARR: %d",temp,dist-lastHrz,dist1-lastBack,arr);
					iteration++;
					lastEncoder = nMotorEncoder[LMotor];
					lastBack = dist1;
					lastHrz = dist;
					while(nMotorEncoder[LMotor] + 5 > lastEncoder && nMotorEncoder[LMotor] > -5*cm){}
				}
				stopMotors();
				playSound(soundBlip);
				degree=sum/iteration;
			}
			//playSound(soundBeepBeep);
			println(5,"%f",degree);
			writeDebugStreamLine(" Deg:%f",degree);
			wait1Msec(1000);
			stopMotors();
			turn(degree);
			stopMotors();
			motor[LMotor] = straight;
			motor[RMotor] = straight;
			while(seeBlackArray()){delay(50);}
			motor[LMotor] = -straight;
			motor[RMotor] = -straight;
			while(!seeBlackArray()){delay(50);}
		}
		SensorType[Leye.port] = sensorColorNxtFULL;
		SensorType[Reye.port] = sensorColorNxtFULL;
		wait1Msec(100);
		while(seeBlack()){}
		while((getColor(Leye) == cWhite || getColor(Leye) == cGradient) && (getColor(Reye) == cWhite || getColor(Reye) == cGradient)){}
		writeDebugStreamLine("%d", getState());
		break;
		//case RIGHTBLACK + LEFTBLACK:
		//	if(lastTurn == RIGHTBLACK)
		//	{
		//		setMotorSyncEncoder(LMotor, RMotor, -100, wheelbase*PI/4*cm, forward);
		//		while(stillRunning(LMotor) || stillRunning(RMotor))
		//		{

		//		}
		//		stopMotors();
		//		resetSync();
		//	}
		//	break;
	case RIGHTGREEN + MIDDLEBLACK:
	case RIGHTGREEN:
		println(0, "State: RIGHT GREEN");
		playSound(soundBlip);
		motor[LMotor] = straight;
		motor[RMotor] = straight;
		while(getColor(Reye) != cWhite && getColor(Reye) != cBlack){}
		playSound(soundBlip);
		if (getColor(Reye) == cWhite)
		{
			playSound(soundBeepBeep);
			return;
		}
		goStraight(6.3);
		resetEncoders();
		motor[RMotor] = green;
		motor[LMotor] = forward;
		while(getColor(Meye) != cWhite){}
		while(getColor(Meye) == cWhite && nMotorEncoder[RMotor] < wheelbase*PI/4*cm){}
		stopMotors();
		playSound(soundBlip);
		motor[RMotor] = straight;
		motor[LMotor] = straight;
		while(getColor(Reye) == cGreen){}
		break;
	case LEFTGREEN + MIDDLEBLACK:
	case LEFTGREEN:
		println(0, "State: LEFT GREEN");
		playSound(soundBlip);
		motor[LMotor] = straight;
		motor[RMotor] = straight;
		while(getColor(Leye) != cWhite && getColor(Leye) != cBlack){}
		playSound(soundBlip);
		if (getColor(Leye) == cWhite)
		{
			playSound(soundBeepBeep);
			return;
		}
		goStraight(6.3);
		resetEncoders();
		motor[RMotor] = forward;
		motor[LMotor] =	green;
		while(getColor(Meye) != cWhite){}
		while(getColor(Meye) == cWhite && nMotorEncoder[LMotor] < wheelbase*PI/4*cm){}
		stopMotors();
		playSound(soundBlip);
		motor[RMotor] = straight;
		motor[LMotor] = straight;
		while(getColor(Leye) == cGreen){}
		break;
	case LEFTBLACK:
		println(0, "State: LEFT BLACK");
		motor[LMotor] = back;
		motor[RMotor] = forward;
		lastTurn = LEFTBLACK;
		break;
	case RIGHTBLACK:
		println(0, "State: RIGHT BLACK");
		playSound(soundBlip);
		motor[LMotor] = forward;
		motor[RMotor] = back;
		lastTurn = RIGHTBLACK;
		break;
	default:
		println(0, "State: STRAIGHT");
		motor[LMotor] = straight;
		motor[RMotor] = straight;
		break;
	}
	lastState = state;
}

void twoEyePID()
{
	if (getColor(Leye) == cGreen)
	{
		println(0, "State: LEFT GREEN");
		playSound(soundBlip);
		motor[LMotor] = straight;
		motor[RMotor] = straight;
		while(getColor(Leye) != cWhite && getColor(Leye) != cBlack){}
		playSound(soundBlip);
		if (getColor(Leye) == cWhite)
		{
			playSound(soundBeepBeep);
			return;
		}
		goStraight(6.3);
		resetEncoders();
		motor[RMotor] = forward;
		motor[LMotor] =	-forward;
		while(getColor(Meye) != cWhite){}
		while(getColor(Meye) == cWhite && nMotorEncoder[LMotor] < wheelbase*PI/4*cm){}
		stopMotors();
		playSound(soundBlip);
		motor[RMotor] = straight;
		motor[LMotor] = straight;
		while(getColor(Leye) == cGreen){}
	}
	if(getColor(Reye) == cGreen)
	{
		println(0, "State: RIGHT GREEN");
		playSound(soundBlip);
		motor[LMotor] = straight;
		motor[RMotor] = straight;
		while(getColor(Reye) != cWhite && getColor(Reye) != cBlack){}
		playSound(soundBlip);
		if (getColor(Reye) == cWhite)
		{
			playSound(soundBeepBeep);
			return;
		}
		goStraight(6.3);
		resetEncoders();
		motor[RMotor] = -forward;
		motor[LMotor] = forward;
		while(getColor(Meye) != cWhite){}
		while(getColor(Meye) == cWhite && nMotorEncoder[RMotor] < wheelbase*PI/4*cm){}
		stopMotors();
		playSound(soundBlip);
		motor[RMotor] = straight;
		motor[LMotor] = straight;
		while(getColor(Reye) == cGreen){}
	}
	short rgb[4], rgb2[4];
	getColorSensorData(Leye.port, colorAtoD, &rgb[0]);
	getColorSensorData(Reye.port, colorAtoD, &rgb2[0]);
	if(rgb[1] > 331)
		rgb[1] = 331;
	if(rgb2[1] > 331)
		rgb2[1] = 331;
	float curr = rgb[1] - rgb2[1];
	float kp = 0.5;
	float err = curr * kp;
	int tp = 30;
	motor[LMotor] = tp + err;
	motor[RMotor] = tp - err;
}
