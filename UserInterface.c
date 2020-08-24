/*
 *	UserInterface.c
 *
 *	Created: 11/10/2012 
 *  Author: Nicholas Smith
 *	Purpose: This class will get all the information needed to display and
 *			 digest it to know what to display and then queue that so LCD
 *			 can dequeue it
 */ 
#include <avr/io.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "UserInterface.h"
#include "Battery_Testing.h"

///////////////////////////////////////////////////////////////////////////
// Parameter: 
//			None.
//
// Returns:   
//			None.
//
// Description: 
//			Has the different states for the LCD so it can send the right
//			information for the LCD to display.
//
///////////////////////////////////////////////////////////////////////////

void UIMain()
{
//-------------------------------------------------getting values---------------------------------------------------------
	uint16_t mAh;
	uint8_t timeout;
	uint16_t rpm = (uint16_t) Q_dequeue(&RPMSlider);
	uint16_t temppercent= (uint16_t) Q_dequeue(&SliderQueue);
	uint8_t SwitchPosition = (uint8_t) Q_dequeue(&SwitchPos);
	uint16_t voltage = (uint16_t) Q_dequeue(&Voltage);
	uint16_t mA = (uint16_t)Q_dequeue(&Current);
	Time t;
	Time* currenttime;
	if ((Q_isEmpty(&timeupdate))==false)//making sure the struct isnt getting written to while reading
	{
		currenttime = (Time*) Q_dequeue(&runtime);//getting the address to the struct
		t = *currenttime;//getting the value from the address
		Q_dequeue(&timeupdate); //getting rid of the 1 that is put in it, forcing it empty
	}
	static Time SavedTime;//saved value from time
	static uint16_t goal = 1000;
	static uint16_t SavedV = 0;
	static uint8_t SaveSwitch = 0;
	static uint16_t SavedmA = 10;	
//------------------------------------------------------------------------------------------------------------------------
/*
	Put the saved values in an array and each screen changes which value is grabbed from the array--the index of the array is the 
	hour. When the conclusion screen timeouts go back to hour 0
*/
		
	//charge estimator	
	mAh = (SavedmA*1000)/(1000*(t.hr+(t.min/60)+(t.sec/3600)));
	mAh =mAh/1000;
	
	static uint8_t screennumber = 0;
	static bool updatescreen = true;
	uint8_t button = (uint8_t) Q_dequeue(&ButtonState);
	if(button == 1 )//|| button == 0) 
	{
		if (screennumber == 4)
		{	
			screennumber = 0;
		}
		else
		{	
			++screennumber;
		}
		updatescreen = true;
	}
	if (updatescreen == true)
	{
		switch(screennumber)//this will load the initial screen onto the LCD
		{
			case 0: Q_enqueue((void*) &WelcomeMessage[0], &LCDMessage);//Welcome Screen
					PORTB = 0x01;//ready light
					break;		
			case 1: Q_enqueue((void*) &Screen1Initial[0], &LCDMessage);//Screen 1	
					break;
			case 2: Q_enqueue((void*) &Screen2Initial[0], &LCDMessage);//Screen 2			
					PORTB = 0x02;//testing light
					sei();
					break;
			case 3: Q_enqueue((void*) &Screen3Initial[0], &LCDMessage);//Screen 3
					PORTB = 0x04;//done testing light
					SavedTime.hr = t.hr;
					SavedTime.min = t.min;
					SavedTime.sec = t.sec;
					SavedV = voltage;
					SavedmA = mA;
					break;
			case 4: Q_enqueue((void*) &conclusionscreen[0], &LCDMessage);//conclusion Screen
					timeout = t.sec;
					break;					
			default:Q_enqueue((void*) &errorscreen[0], &LCDMessage);//Error Screen
					PORTB = 0x07;//all lights on
					break;
		}//end of switch
		updatescreen = false;
	}//end of if
	
	static uint8_t tempbottom[16];
	switch(screennumber)//this will load the initial screen onto the LCD
	{	
		case 1: Q_enqueue((void*) true, &resetclk);
				if (SwitchPosition)//the user wants to measure in voltage
				{	
					uint16_t SliderVoltage = ((temppercent*100)/83);
					tempbottom[0] = '0' + ((SliderVoltage%1000)/100);
					tempbottom[1] = '0' + ((SliderVoltage%100)/10);
					tempbottom[2] = '.';
					tempbottom[3] = '0' + ((SliderVoltage%10)/1);	
					tempbottom[4] = 'V';
				}
				else
				{
					uint16_t SliderAmps = temppercent * 10;
					if (SliderAmps >= 999)
					{
						SliderAmps = 999;
					}
					tempbottom[0] = '0' + ((SliderAmps%1000)/100);
					tempbottom[1] = '0' + ((SliderAmps%100)/10);
					tempbottom[2] = '0' + ((SliderAmps%10)/1);
					tempbottom[3] = 'm';
					tempbottom[4] = 'A';
				}
				//RPM code
				tempbottom[5] = ' ';
				tempbottom[6] = '0' + ((rpm%1000)/100);
				tempbottom[7] = '0' + ((rpm%100)/10);
				tempbottom[8] = '0' + ((rpm%10)/1);
				tempbottom[9] = '%';
				tempbottom[10] = ' ';
				tempbottom[11] = 'e';
				tempbottom[12] = 'n';
				tempbottom[13] = 't';
				tempbottom[14] = 'e';
				tempbottom[15] = 'r';
				Q_enqueue((void*) &tempbottom, &BottomUpdate);//Screen 1
				SaveSwitch = SwitchPosition;
				goal = temppercent;
				break;
					
		case 2: tempbottom[0] = '0' + ((t.hr%100)/10);
				tempbottom[1] = '0' + ((t.hr%10)/1);  
				tempbottom[2] = ':';				  
				tempbottom[3] = '0' + ((t.min%100)/10);
				tempbottom[4] = '0' + ((t.min%10)/1);
				tempbottom[5] = ' ';
				tempbottom[6] = ' ';
				tempbottom[7] = '0' + ((rpm%1000)/100);
				tempbottom[8] = '0' + ((rpm%100)/10);
				tempbottom[9] = '0' + ((rpm%10)/1);
				tempbottom[10] = '%'; 
				tempbottom[11] = ' ';
				tempbottom[12] = '0' + ((voltage%1000)/100);
				tempbottom[13] = '0' + ((voltage%100)/10);
				tempbottom[14] = '.';
				tempbottom[15] = '0' + ((voltage%10)/1);
				Q_enqueue((void*) &tempbottom, &BottomUpdate);//Screen 2
				if (SaveSwitch)
				{
					if (((voltage*830)/100) < goal)
					{
						screennumber = 3;
						updatescreen = true;
					}
				}
				else
				{
					if ((mA/12) < goal)
					{
						screennumber = 3;
						updatescreen = true;
					}
				}
				break;
					
		case 3: tempbottom[0] = '0' + ((SavedmA%1000)/100);
				tempbottom[1] = '0' + ((SavedmA%100)/10);  
				tempbottom[2] = '0' + ((SavedmA%10)/1);				  
				tempbottom[3] = ' ';
				tempbottom[4] = '0' + ((SavedV%100)/10);
				tempbottom[5] = '.';
				tempbottom[6] = '0' + ((SavedV%10)/1);
				tempbottom[7] = ' ';
				tempbottom[8] = '0' + ((SavedTime.hr%100)/10);
				tempbottom[9] = '0' + ((SavedTime.hr%10)/1);
				tempbottom[10] = '.';
				tempbottom[11] = '0' + (((SavedTime.min/60)%10)/1); 
				tempbottom[12] = ' ';
				tempbottom[13] = '0' + ((mAh%1000)/100);
				tempbottom[14] = '0' + ((mAh%100)/10);
				tempbottom[15] = '0' + ((mAh%10)/1);
				Q_enqueue((void*) &tempbottom, &BottomUpdate);//Screen 3	
				break;
		
		case 4: if (t.sec - 5 == timeout)//go back to screennumber 3 where timeout is when we hit screen 4
				{
					screennumber = 3;
					updatescreen = true;
				}				
				break;
				
		default:break;
	}//end of switch
}