/*
 * Switch.c
 *
 * Created: 11/6/2012 8:19:15 PM
 *  Author: Seth Ware
 */ 
/* Purpose of this code is to control the SPDT switch for the battery tester, the switch while in one position will depict one screen
when switched it will show the other screen*/

#include <avr/io.h>
#include "Switch.h"
#include "Battery_Testing.h"

void Switch()
{
	static uint8_t vote = 0;	//Keeps track of the Votes
	static uint8_t switchposition = Right;	//initializes the button to Released
		
	if((PIND&(1<<PD6))){		//Inactive reading PD0 (PD0 = 0) so it adds a zero to the end of vote			
		vote=vote<<1;
	}			
	else{						//Active reading PD0 (PD0 = 1) so it adds a one to the end of vote	
		vote=vote<<1|1;			
	}
	
	switch(switchposition)
	{
		
		case Left:	if((vote&0x0f) == 0x0f)		//Changes the state to right when vote ends in four 1's
						switchposition = Right;
						break;
						
		case Right:		if((vote&0x0f) == 0)		//Changes the state to left when vote ends in four 0's
						switchposition = Left;
						break;
	}

	Q_enqueue((void*) switchposition,&SwitchPos);
}

