
/*
 * NXT I2C register storage
 * byte 0 = 3;
   byte 1 =  old I2C address;
   byte 2 =  0x41;  the command register to change i2c address
   byte 3 =  <sequence as below>
 *           0xA0 0xAA 0x A5 and <new address>
 *
 */


//=================================================//
// change the I2C address (for Mindsensors sensors only
//=================================================//

void  alterI2cAddr(tSensors port, ubyte old,ubyte new)
{
	ubyte i2cMsg[8];

	nI2CBytesReady[port]	 = 0;
  SensorType[port] = sensorI2CCustom;

  // Build the I2C message
  i2cMsg[0] = 3;
  i2cMsg[1] = old;
  i2cMsg[2] = 0x41;

	i2cMsg[3]=0xA0;
	//while (nI2CStatus[port] != NO_ERR){displayTextLine(4, "Status: %d", nI2CStatus[port]);}  // Wait till I2C bus is ready
	sendI2CMsg(port, &i2cMsg[0], 0);									// Send the message
	wait1Msec(10);

	i2cMsg[3]=0xAA;
	//while (nI2CStatus[port] != NO_ERR){displayTextLine(4, "Status: %d", nI2CStatus[port]);}  // Wait till I2C bus is ready
	sendI2CMsg(port, &i2cMsg[0], 0);									// Send the message
	wait1Msec(10);

	i2cMsg[3]=0xA5;
	//while (nI2CStatus[port] != NO_ERR){displayTextLine(4, "Status: %d", nI2CStatus[port]);}  // Wait till I2C bus is ready
	sendI2CMsg(port, &i2cMsg[0], 0);									// Send the message
	wait1Msec(10);

	i2cMsg[3]=new;
	//while (nI2CStatus[port] != NO_ERR){displayTextLine(4, "Status: %d", nI2CStatus[port]);}  // Wait till I2C bus is ready
	sendI2CMsg(port, &i2cMsg[0], 0);									// Send the message
	wait1Msec(10);

}

//=================================================
task main()
{ ubyte old = 0x54,new=0x55;
	displayTextLine(1, "Change MS I2C");
	displayTextLine(2, "from X%02x", old);
	displayTextLine(3, "to   X%02x", new);

	alterI2cAddr(S1,old,new);
	wait1Msec(3000);

  stopAllTasks();
}
