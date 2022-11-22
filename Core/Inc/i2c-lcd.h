
#include "stm32f4xx_hal.h"

void lcdInit (void);   // initialize lcd

void sendCmd (char cmd);  // send command to the lcd

void sendData (char data);  // send data to the lcd

void sendString (char *str);  // send string to the lcd

void putCursor(int row, int col);  // put cursor at the entered position row (0 or 1), col (0-15);

void clear (void);
