/*
 * Voltmeter.c
 *
 * Created: 11/5/2012 1:06:03 PM
 * Author: Nicholas Smith, Sean Robb, and Seth Ware
 * About: This is the code that lights up the LEDs depending on voltage for testing
 */ 

#include <avr/io.h>
#include "Voltmeter.h"
#include "Battery_Testing.h"

///////////////////////////////////////////////////////////////////////////
// Parameter: 
//			None.
//
// Returns:   
//			None.
//
// Description: 
//			The code that makes the voltmeter go
//
///////////////////////////////////////////////////////////////////////////
void Voltmeter()
{		
	uint16_t ndv = 0;
	ADCSRA = 0xC6; //on, start conversion, auto triggering, divide by 64
	ADMUX = 0x61;//vref off, left adjust,PA1 has result
	while ((ADCSRA&(1<<ADSC))==(1<<ADSC)){;}	//Waits till conversion is complete
	uint16_t dvin;//what the adch will become, used this instead of adch just incase you want to use ADCW
	dvin=ADCH;//puts ADCH into the vin
	ndv=(dvin*100+256)/512;//calculates the voltage
		
	Q_enqueue((void*) ndv, &Voltage);
}
