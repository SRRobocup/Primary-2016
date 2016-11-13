
const tMotor Lm = motorA;
const tMotor Rm = motorC;

const float Tdia=3;
const float WB=14;
const float GR=3;
const float EncPerCm = 360.0*GR / (Tdia *  PI);

void turnright(float deg)
{
	//	float dege=Wb/Tdia*deg;
	float Cmperturn=WB*PI*(deg/360);
	float encturn=Cmperturn*EncPerCm;
	nMotorEncoder[Rm]=0;
	while( nMotorEncoder[Rm]<encturn)
	{
		nxtDisplayTextLine(0,"%d",nMotorEncoder[Rm]);
		motor[Rm]=50;
		motor[Lm]=-50;
	}
}

void turnleft(float deg)
{
	//	float dege=Wb/Tdia*deg;
	float Cmperturn=WB*PI*(deg/360);
	float encturn=Cmperturn*EncPerCm;
	nMotorEncoder[Lm]=0;
	while( nMotorEncoder[Lm]<encturn)
	{
		nxtDisplayTextLine(0,"%d",nMotorEncoder[Lm]);
		motor[Rm]=-50;
		motor[Lm]=50;
	}
}


void gostraight(float cm)
{

	nMotorEncoder[Lm]=0;
	while(nMotorEncoder[Lm]<(cm*EncPerCm)
	{
		motor[Lm]=30;
		motor[Rm]=30;

	}
	motor[Lm]=0;
	motor[Rm]=0;
}

void goback(float cm)
{

	nMotorEncoder[Lm]=0;
	while(abs(nMotorEncoder[Lm])<(cm*EncPerCm)
	{
		motor[Lm]=-30;
		motor[Rm]=-30;

	}
	motor[Lm]=0;
	motor[Rm]=0;
}

void Stop()
{
	motor[Lm]=0;
	motor[Rm]=0;
}

void obstvariables()
{
float bz=2, dia=11, wb=13.5, inner=dia+(bz*2), outer=inner+(wb*2);
float opow=70, ipow=opow*(inner/outer);
}
