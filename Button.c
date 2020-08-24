/*
 * Debouncing.c
 *
 *	Created: 10/24/2012 3:28:06 PM
 *  Author: Sean Robb, Nicholas Smith
 *
 *	Purpose: This program debounces the button PD0
 */ 

#include <avr/io.h>
#include "button.h"
#include "Battery_Testing.h"

void InitializeButton()
{
	DDRD &= ~(1<<PD7);		//for Button
}	
		
void enterButton()
{
	static uint8_t vote = 0;	//Keeps track of the Votes
	static uint8_t button = Inactive;	//initializes the button to Released
		
	if((PIND&(1<<PD7))){		//Inactive reading PD0 (PD0 = 0) so it adds a zero to the end of vote			
		vote=vote<<1;
	}			
	else{						//Active reading PD0 (PD0 = 1) so it adds a one to the end of vote	
		vote=vote<<1|1;			
	}
	
	switch(button)
	{
		
		case Inactive:	if((vote&0x0f) == 0x0f)		//Changes the state to pressed when vote ends in four 1's
						button = Pressed;
						break;
						
		case Pressed:	button = Hold;				//Changes state to Hold							
						break;
						
		case Hold:		if((vote&0x0f) == 0)		//Changes the state to Released when vote ends in four 0's
						button = Released;
						break;
						
		case Released:	button = Inactive;			//Changes state to Inactive
						break; 
	}
	Q_enqueue((void*) button, &ButtonState);
}
