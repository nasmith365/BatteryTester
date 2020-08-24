/*
 * LCD.c
 *
 * Created: 10/29/2012 1:36:49 PM
 * Authors: Nicholas Smith
 * Lab: All the screens for the LCD Display for the battery tester
 */ 

#include <avr/io.h>
#include <stdbool.h>
#include <avr/pgmspace.h>
#include "LCD.h"
#include "Battery_Testing.h"

///////////////////////////////////////////////////////////////////////////
// 
// Parameter: 	
//		bool val - turn the display on?
//
// Returns:   	
//		None.
// 
// Description:	
//		Based on val, the display with receive the on or off instruction
// 
///////////////////////////////////////////////////////////////////////////
void turnDisplayOn()
{	
	//Assuming C1 (R/W) and C0 (RS) are pulled low after every data transfer	
	//Pull C2 (E) high, then write handshake
	PORTC |= 1 << PC2;
	//Instruction bits
	// Nothing needs to be asserted high
	//Pull C2 (E) low to complete handshake
	PORTC &= ~(1 << PC2);
	//Drop raised bits for future use
	// Nothing was asserted high
	//Pull C2 (E) high, then write handshake
	PORTC |= 1 << PC2;
	//Instruction bits				 on		    cursor	     blink
	PORTD = PORTD | (1 << PD3) | (1 << PD2) | (1 << PD1) | (1 << PD0);
	//Pull C2 (E) low to complete handshake
	PORTC &= ~(1 << PC2);
	//Drop raised bits for future use
	PORTD = PORTD & ~(1 << PD3) & ~(1 << PD2) & ~(1 << PD1) & ~(1 << PD0);		
	
	readyToSendNext();	
}

///////////////////////////////////////////////////////////////////////////
// 
// Parameter:
//		None.
//
// Returns:   	
//		None.
// 
// Description:	
//		Wastes 110.00 microseconds.
// 
///////////////////////////////////////////////////////////////////////////
void waste110Microseconds()
{
	//110.00 microsecond delay
    for(int loop = 0; loop < 4; ++loop){;}	
}

///////////////////////////////////////////////////////////////////////////
// 
// Parameter: 	
//		None.
//
// Returns:   	
//		None.
// 
// Description:	
//		The entire LCD handshake in one method.
// 
///////////////////////////////////////////////////////////////////////////
void performLCDHandshake()
{
	//15+ ms handshake
	//15.117 ms simulated
	for(int loop = 0; loop < 120; ++loop)
	{
		waste110Microseconds();
	}
	
	dasHandshake();
	
	//4.1+ ms handshake
	//4.237 ms simulated
	for(int loop = 0; loop < 33; ++loop)
	{
		waste110Microseconds();
	}
	
	dasHandshake();
		
	//Final handshake
	//110.00 us simulated
	waste110Microseconds();
	
	dasHandshake();
	
	//Busy flag is enabled and capable of being read
	readyToSendNext();
	
	//Let's perform I through V from datasheet
	//I Function Set
	//Pull C2 (E) high, then write handshake
	PORTC |= 1 << PC2;
	//Handshake bits
	PORTD |= (1 << PD1);
	//Pull C2 (E) low to complete handshake
	PORTC &= ~(1 << PC2);
	//Drop raised bits for future use
	PORTD &= ~(1 << PD1);
	
	//busy flag
	readyToSendNext();
	
	//II Function Set
	//Pull C2 (E) high, then write handshake
	PORTC |= 1 << PC2;
	//Handshake bits
	PORTD |= (1 << PD1);
	//Pull C2 (E) low to complete handshake
	PORTC &= ~(1 << PC2);
	//Drop raised bits for future use
	PORTD &= ~(1 << PD1);	
	//Pull C2 (E) high, then write handshake
	PORTC |= 1 << PC2;
	//			    N (2 lines)| F (5x10 dots)
	// others are don't care values
	PORTD = PORTD | (1 << PD3) | (1 << PD2);
	//Pull C2 (E) low to complete handshake
	PORTC &= ~(1 << PC2);
	//Drop raised bits for future use
	PORTD = PORTD & ~(1 << PD3) & ~(1 << PD2);

	readyToSendNext();	
	
	//III Display Off
	//Pull C2 (E) high, then write handshake
	PORTC |= 1 << PC2;
	//Handshake bits
	// Nothing needs to be asserted high
	//Pull C2 (E) low to complete handshake
	PORTC &= ~(1 << PC2);
	//Drop raised bits for future use
	// Nothing was asserted high
	//Pull C2 (E) high, then write handshake
	PORTC |= 1 << PC2;
	//Handshake bits
	PORTD = PORTD | (1 << PD3);
	//Pull C2 (E) low to complete handshake
	PORTC &= ~(1 << PC2);
	//Drop raised bits for future use
	PORTD = PORTD & ~(1 << PD3);
	
	readyToSendNext();
	
	//IV Display Clear
	//Pull C2 (E) high, then write handshake
	PORTC |= 1 << PC2;
	//Handshake bits
	// Nothing needs to be asserted high
	//Pull C2 (E) low to complete handshake
	PORTC &= ~(1 << PC2);
	//Drop raised bits for future use
	// Nothing was asserted high
	//Pull C2 (E) high, then write handshake
	PORTC |= 1 << PC2;
	//Handshake bits
	PORTD = PORTD | (1 << PD0);
	//Pull C2 (E) low to complete handshake
	PORTC &= ~(1 << PC2);
	//Drop raised bits for future use
	PORTD = PORTD & ~(1 << PD0);		
	
	readyToSendNext();
	
	//V Entry Mode Set
	//Pull C2 (E) high, then write handshake
	PORTC |= 1 << PC2;
	//Handshake bits
	// Nothing needs to be asserted high
	//Pull C2 (E) low to complete handshake
	PORTC &= ~(1 << PC2);
	//Drop raised bits for future use
	// Nothing was asserted high
	//Pull C2 (E) high, then write handshake
	PORTC |= 1 << PC2;
	// S (DB4) = 0 to not shift display
	//						  |I/D (increment)
	PORTD = PORTD | (1 << PD2)| (1 << PD1);	
	//Pull C2 (E) low to complete handshake
	PORTC &= ~(1 << PC2);
	//Drop raised bits for future use
	PORTD = PORTD & ~(1 << PD2)& ~(1 << PD1);
	
	readyToSendNext();
	//All done!
}

///////////////////////////////////////////////////////////////////////////
// Parameter: 
//			None.
//
// Returns:   
//			None.
//
// Description: 
//			Gets structure that contain position and char array and size of   
//			array from the UI class to write.
//
///////////////////////////////////////////////////////////////////////////
void UpdateValues()
{
	if (Q_isEmpty(&BottomUpdate))
	{
		return;
	}
	uint8_t *ptr;
	ptr = (uint8_t) Q_dequeue(&BottomUpdate);
	uint8_t temp[16];
	for (uint8_t i =0; i<16; ++i)
	{
		temp[i] = *(ptr+i);
	}
	MoveCursor(0x00, false);//everything being written is going to be on the bottom
	for(int index = 0; index < 16; ++index)
	{
		//pull C0 (RS) high to allow data entry
		PORTC |= 1 << PC0;	
		//Pull C2 (E) high, then write 
		PORTC |= 1 << PC2;
		//Top 4 bits set up
		PORTD |= (temp[index]>>4);
		//Pull C2 (E) low to transmit
		PORTC &= ~(1 << PC2);
		//Drop raised bits for future use
		PORTD &= ~(temp[index]>>4);
		//Pull C2 (E) high, more data
		PORTC |= 1 << PC2;
		//Bottom 4 bits set up
		PORTD |= (temp[index] & 0x0F);
		//Pull C2 (E) low to transmit
		PORTC &= ~(1 << PC2);
		//Drop raised bits for future use
		PORTD &= ~(temp[index] & 0x0F);
		//pull C0 (RS) low for instruction input after leaving method
		PORTC &= ~(1 << PC0);
		//busy flag will be 1; waits for 0
		readyToSendNext();	
	}	
	
}

///////////////////////////////////////////////////////////////////////////
// Parameter: 
//		None.
//
// Returns:
//		None.
//
// Description:
//		Going to write the top part of the message to the lcd 
//
///////////////////////////////////////////////////////////////////////////
void WriteMessage()
{
	if (Q_isEmpty(&LCDMessage))
	{
		return;
	}
	
	uint16_t ptr = (uint16_t) Q_dequeue(&LCDMessage);
	uint8_t length = 32;
	uint8_t temp[32];
	for (uint8_t i =0; i<length; ++i)
	{
		temp[i] = pgm_read_byte(ptr+i);
	}
	MoveCursor(0x00, true);
	for(int index = 0; index < length; ++index)
		{
			if (index == 16)
			{
				MoveCursor(0x00,false);
			}
			//pull C0 (RS) high to allow data entry
			PORTC |= 1 << PC0;	
			//Pull C2 (E) high, then write 
			PORTC |= 1 << PC2;
			//Top 4 bits set up
			PORTD |= (temp[index]>>4);
			//Pull C2 (E) low to transmit
			PORTC &= ~(1 << PC2);
			//Drop raised bits for future use
			PORTD &= ~(temp[index]>>4);
			//Pull C2 (E) high, more data
			PORTC |= 1 << PC2;
			//Bottom 4 bits set up
			PORTD |= (temp[index] & 0x0F);
			//Pull C2 (E) low to transmit
			PORTC &= ~(1 << PC2);
			//Drop raised bits for future use
			PORTD &= ~(temp[index] & 0x0F);
			//pull C0 (RS) low for instruction input after leaving method
			PORTC &= ~(1 << PC0);
			//busy flag will be 1; waits for 0
			readyToSendNext();	
		}	
}
///////////////////////////////////////////////////////////////////////////
// 
// Parameter: 	
//		None.
//
// Returns:   	
//		None.
// 
// Description:	
//		Das hand... shake it like a salt shaker
// 
///////////////////////////////////////////////////////////////////////////
void dasHandshake()
{
	//Pull C2 (E) high, then write handshake
	PORTC |= 1 << PC2;

	//Handshake bits
	PORTD = PORTD | (1 << PD1) | (1 << PD0);

	//Pull C2 (E) low to complete handshake
	PORTC &= ~(1 << PC2);

	//Drop raised bits for future use
	PORTD = PORTD & ~(1 << PD1) & ~(1 << PD0);
}

///////////////////////////////////////////////////////////////////////////
// 
// Parameter: 	
//		None.
//
// Returns:   	
//		bool - Is the busy flag 0
// 
// Description:	
//		Checks busy flag each iteration
// 
///////////////////////////////////////////////////////////////////////////
void readyToSendNext()
{
	//set PD3 to input pin
	DDRD &= ~(1<<PD3);
	//pull C1 (R/W) high
	PORTC |= 1 << PC1;
	
	char pind3 = 0x08;
	//check DB7 (D3) for busy flag
	while(pind3 == 0x08)
	{
		//Pull C2 (E) high, then write 
		PORTC |= 1 << PC2;
		pind3 = (PIND & (1<<PD3));
		//Pull C2 (E) low 
		PORTC &= ~(1 << PC2);
		//second pulse to pass address counter		
		//Pull C2 (E) high
		PORTC |= 1 << PC2;
		//Pull C2 (E) low 
		PORTC &= ~(1 << PC2);	
	}
	//pull C1 (R/W) low
	PORTC &= ~(1 << PC1);
	//set PD3 to output pin
	DDRD |= (1<<PD3);	
}

///////////////////////////////////////////////////////////////////////////
// 
// Parameter: 	
//		uint8_t hexvalue - value where the cursor needs to be on the bottom
//		bool top - tells if the cursor is supposed to be on bottom or top
//
// Returns:   	
//		None.
// 
// Description:	
//		Moves the cursor to the correct place on the bottom line
// 
///////////////////////////////////////////////////////////////////////////
void MoveCursor(uint8_t hexvalue, bool top)
{
	//Pull C2 (E) high, then write 
	PORTC |= 1 << PC2;	
	//set dd ram address to 0x40
	//Handshake bits
	if (top)
	{
		PORTD |= 0x08;//DB7 is high add to 0x4
	} 
	else
	{
		PORTD |= 0x0C;//DB7 is high add to 0x4
	}
	//Pull C2 (E) low to complete handshake
	PORTC &= ~(1 << PC2);
	//Drop raised bits for future use
	PORTD &= ~(0x0C);		
	//Pull C2 (E) high, then write
	PORTC |= 1 << PC2;
	PORTD |= hexvalue;//DB7 is high add to 0x4
	//Pull C2 (E) low to complete handshake
	PORTC &= ~(1 << PC2);
	//Drop raised bits for future use
	PORTD &= ~hexvalue;	
	readyToSendNext();
}