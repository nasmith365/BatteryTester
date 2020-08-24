/*
 * Ammeter_Test.c
 *
 * Created: 11/11/2012 2:13:37 PM
 *  Author: Sean Robb, Nicholas Smith, Seth Ware
 *	Purpose: This program is supposed to take in the voltages across a circuit and then calculate the current
 *
 */ 

#include <avr/io.h>
#include "Ammeter.h"
#include "Battery_Testing.h"

void Ammeater()
{
	uint16_t volt1,volt2,current,resistor=1;//resistor value will change
	//Calculates the first voltage
	ADCSRA = 0xC6; //on, start conversion, auto triggering, divide by 64
	ADMUX = 0x61;									//vref off, left adjust,PA1 has result
	while ((ADCSRA&(1<<ADSC))==(1<<ADSC)){;}		//Waits till conversion is complete
	volt1=(ADCH*100+256)/512;							//calculates the voltage

	ADCSRA = 0xC6; //on, start conversion, auto triggering, divide by 64	
	//Calculates the second voltage
	ADMUX = 0x62;									//vref off, left adjust,PA2 has result
	while ((ADCSRA&(1<<ADSC))==(1<<ADSC)){;}		//Waits till conversion is complete
	volt2=(ADCH*100+256)/512;							//calculates the voltage	

	volt1 = volt1 *100;
	volt2 = volt2 * 100;
	//Calculates the current through the meter
	current=((volt1-volt2)/resistor);//volt 1 will always be larger than volt2
	
	Q_enqueue((void*) current, &Current);
}