/*
 * LCD.h
 *
 * Created: 10/29/2012 1:38:04 PM
 * Author:	Nicholas Smith
 * Lab:		All the screens for the LCD Display for the battery tester
 */ 


#ifndef LCD_H_
#define LCD_H_

void turnDisplayOn();
void waste110Microseconds();
void performLCDHandshake();
void dasHandshake();
void readyToSendNext();
void MoveCursor(uint8_t hexvalue, bool top);
void WriteMessage();
void UpdateValues();

#endif /* LCD_H_ */