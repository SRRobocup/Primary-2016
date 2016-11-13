/*

* change_add.c v2.0

*

* This is driver/utility program to use  with mindsensors.com  sensor to

* change the address.

*

* written by Dr. Nitin Patil

* 2006(c) mindsensors.com

* modified - by Elizabeth : June 2016

* allow to use various ports and function parameters

*/




/*status:




i2cStatusNoError         =  0,  // No error

	        i2cStatusPending,               // Transaction is being processed

	        i2cStatusFailed,                // The transaction has failed

	        i2cStatusBadConfig,             // The port has not been configured properly

	        i2cStatusStopped,               // A transaction has completed successfully.

	        i2cStatusStartTransfer = 8,

*/




/*

* NXT I2C register storage

*

* 0x41 is command register

* writing  0xA0 0xAA 0x A5 and <new address> will change the address of

* the device to new address

* New address is effective immediately.

*/




#ifdef EV3

#define STAT_COMM_PENDING i2cStatusPending

#define NO_ERR   i2cStatusNoError

#endif

//////////////////////////////////////////////////////////////////////////////

//

//  change the adress of the I2C sensor oldAddresswith the NewAddress

//

/////////////////////////////////////////////////////////////////////////////




void  Change_add(byte oldAddress,byte newAddress, tSensors link=S1)

{

	char i2cMsg[8];

	const byte kMsgSize        = 0;

	const byte kDeviceAddress    = 1;

	const byte kCommandAddress  = 2;

	const byte kCommand  = 3;




	nI2CBytesReady[link]	 = 0;

	SensorType[link] = sensorI2CCustom;




	// Build the I2C message

	i2cMsg[kMsgSize]           = 3;

	i2cMsg[kDeviceAddress]     = oldAddress ;

	i2cMsg[kCommandAddress]    =0x41;




	i2cMsg[kCommand]=0xA0;




	while ( nI2CStatus[link] == STAT_COMM_PENDING) sleep(100);




	displayTextLine(7,"Err:%d", nI2CStatus[link]);

	// Wait till I2C bus is ready

	sendI2CMsg(link, i2cMsg, 0);									// Send the message

	wait10Msec(10);




	i2cMsg[kCommand]=0xAA;




	while (nI2CStatus[link] == STAT_COMM_PENDING) sleep(100);

	displayTextLine(7,"Err:%d", nI2CStatus[link]);

	sendI2CMsg(link, i2cMsg, 0);									// Send the message

	wait10Msec(10);




	i2cMsg[kCommand]=0xA5;




	while (nI2CStatus[link] == STAT_COMM_PENDING) sleep(100);




	displayTextLine(7,"Err:%d", nI2CStatus[link]);

	sendI2CMsg(link, i2cMsg, 0);									// Send the message

	wait10Msec(10);




	i2cMsg[kCommand]=newAddress;

	while (nI2CStatus[link] != NO_ERR)

		displayTextLine(7,"Err:%d", nI2CStatus[link]);




	sendI2CMsg(link, i2cMsg, 0);									// Send the message

	wait10Msec(10);

}




/*

This is driver/utility program to scan the devices on the NXT

portsdefault port is port

*/




//////////////////////////////////////////////////////////////////////////////

//

//      demo main function to show change adress function

//

/////////////////////////////////////////////////////////////////////////////




task main()

{

	displayTextLine(1, "mindsensors.com");

	displayTextLine(2, "address change");




	Change_add(0x54,0x58, S3);

	delay(2000);




	stopAllTasks();

}
