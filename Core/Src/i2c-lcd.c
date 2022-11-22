
/** Put this in the src folder **/

#include "i2c-lcd.h"
extern I2C_HandleTypeDef hi2c1;  // change your handler here accordingly

#define SLAVE_ADDRESS_LCD 0x4E // change this according to ur setup

void sendCmd (char cmd)
{
	//Initializes two required variables
  char data_u, data_l;

	//Defines an array so that the data from each command can be send in two seperate packets
	uint8_t data_t[4];

	//This block saves copys both halfs of the data into the array using the two variables defined at the beginning
	data_u = (cmd&0xf0);
	data_l = ((cmd<<4)&0xf0);
	data_t[0] = data_u|0x0C;
	data_t[1] = data_u|0x08; 
	data_t[2] = data_l|0x0C; 
	data_t[3] = data_l|0x08;

	//Sends the command to the LCD
	HAL_I2C_Master_Transmit (&hi2c1, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
}

void sendData (char data)
{
	//Defines required variables and defines array (same process as the sendCmd function)
	char data_u, data_l;
	uint8_t data_t[4];

	//Splits data into two halfs and transmits it (same process as the sendCmd function)
	data_u = (data&0xf0);
	data_l = ((data<<4)&0xf0);
	data_t[0] = data_u|0x0D;  //en=1, rs=0
	data_t[1] = data_u|0x09;  //en=0, rs=0
	data_t[2] = data_l|0x0D;  //en=1, rs=0
	data_t[3] = data_l|0x09;  //en=0, rs=0
	HAL_I2C_Master_Transmit (&hi2c1, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);

	//This block only differs from the last function in that it sends a character to be printed instead of a command
}

void clear (void)
{
	//This block uses the sendData function to send a blank character to each cell of the lcd
	sendCmd (0x80);
	for (int i=0; i<70; i++)
	{
		sendData (' ');
	}
}

void putCursor(int row, int col)
{
	//This function takes a row and column number and changes the location of the cursor
    switch (row)
    {
		//Based on the value of row (either row 0 or row 1), the column is set
		// Only two cases because the LCD is 16x2
        case 0:
			//Row 0 (first row)
            col |= 0x80;
            break;
        case 1:
			//Row 1 (second row)
            col |= 0xC0;
            break;
    }
	//Sends the column location to the LCD
    sendCmd (col);
}


void lcdInit (void)
{
	// 4 bit initialisation
	HAL_Delay(50);  // wait for >40ms
	sendCmd (0x30);
	HAL_Delay(5);  // wait for >4.1ms
	sendCmd (0x30);
	HAL_Delay(1);  // wait for >100us
	sendCmd (0x30);
	HAL_Delay(10);
	sendCmd (0x20);  // 4bit mode
	HAL_Delay(10);

  // dislay initialisation
	sendCmd (0x28); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
	HAL_Delay(1);
	sendCmd (0x08); //Display on/off control --> D=0,C=0, B=0  ---> display off
	HAL_Delay(1);
	sendCmd (0x01);  // clear display
	HAL_Delay(1);
	HAL_Delay(1);
	sendCmd (0x06); //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
	HAL_Delay(1);
	sendCmd (0x0C); //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
}

void sendString (char *str)
{
	//Uses the sendData command for each character in the string
	while (*str) sendData (*str++);
}
