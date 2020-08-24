/*
 * Battery_Testing.c
 *
 * Created: 11/5/2012 2:35:47 PM
 * Author: Nicholas Smith, Sean Robb, Seth Ware
 * About: Putting it all together
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "Queue.h"
#include "Voltmeter.h"
#include "Battery_Testing.h"
#include "LCD.h"
#include "button.h"
#include "Ammeter.h"
#include "RPMSlider.h"

//All the queue is extern so they are already initialized to 0
Queue Voltage;
Queue runtime;
Queue timeupdate;
Queue LCDMessage;
Queue ButtonState;
Queue ValueUpdate;
Queue BottomUpdate;
Queue resetclk;
Queue Current;
Queue SwitchPos;
Queue RPMSlider;
Queue SliderQueue;

int main(void)
{	
	TotalInitialization();
	while(1)
    {
        Voltmeter();
		Ammeater();
		enterButton();
		RPM_Slider();
		Slider();
		Switch();
		UIMain();
		WriteMessage();
		UpdateValues();
    }
}

void TotalInitialization()
{
	//lcd--------------------------------------------------
	//RS is C0, R/W is C1, E is C2
	DDRC |= 0x07;
	PORTC &= 0xF8;
	//DB7 down to DB4 corresponds to D3 down to D0
	DDRD |= 0x0F;
	PORTD &= 0x80;
	performLCDHandshake();
	turnDisplayOn();
	//-----------------------------------------------------
	//Set up timer2 for clock ------------------------------
	//pg82		   |------CTC Mode-------|Normal port operation|------------clk/8------------|
	TCCR2=(1<<FOC2)|(1<<WGM21)|(0<<WGM20)|(0<<COM21)|(0<<COM20)|(0<<CS22)|(1<<CS21)|(0<<CS20);
	OCR2=249;							//system clock is 8mhz
	TIMSK=TIMSK|(1<<OCIE2);				//Set interrupts	
	sei();
	//-----------------------------------------------------
	//set up the lights------------------------------------
	DDRB |= 0x07;
	//-----------------------------------------------------
	//set up timer0 for pwm--------------------------------
	OCR0 = 255;//initially not moving --going to need to send right duty cycles to turn it left or right
	TCCR0 = 0x7B; //Fast mode, set OC2 on compare match, clear on bottom, inverting mode, 32 prescaler for 1 Mhz clk
	//-----------------------------------------------------
}