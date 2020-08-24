/*
 * Slider.c
 *
 *	Created: 11/6/2012 8:08:12 PM
 *  Author: Nicholas Smith
 *	Purpose:This code will read in the position from the sliders and then 
 *			figure out the correct percentage of the position
 */ 

#include <avr/io.h>
#include "Slider.h"
#include "Battery_Testing.h"

///////////////////////////////////////////////////////////////////////////
// Parameter: 
//			None.
//
// Returns:   
//			None.
//
// Description: 
//			Take a 0-5 volt value and convert it to a percentage
//
///////////////////////////////////////////////////////////////////////////
void Slider()
{	
	ADCSRA = 0xC6; //on, start conversion, auto triggering, divide by 64
	ADMUX = 0x65;//vref off, left adjust,PA5 has result
	while ((ADCSRA&(1<<ADSC))==(1<<ADSC)){;}	//Waits till conversion is complete
	uint16_t ZeroToFive = (ADCH*100+256)/512;	
	uint16_t slider = ZeroToFive * 2;
	Q_enqueue((void*) slider, &SliderQueue);
}