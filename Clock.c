/*
 * Clock.c
 *
 * Created: 10/30/2012 6:18:31 PM
 *  Author: Sean Robb
 *
 *	Purpose: This is meant to run at 8 MHz and have a ISR
 *			 about every 1/4 of a millisecond with our struct
 *			 will increment the sec, min and hr.
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "Battery_Testing.h"

ISR(TIMER2_COMP_vect)	{

	
	static Time t = {0,0,0};	// to get rid of global use a queue	
	Time* tptr = &t;
	static uint16_t tick=0;
	if (!Q_isEmpty(&resetclk))
	{
		Q_dequeue(&resetclk);
		t.hr =0;
		t.sec =0;
		t.min = 0;
	}
	tick++; //equals 1/4 a millisecond
	
	if (tick >= 4124){					//Every Sec
		tick =0;
		t.sec+=1;
			if (t.sec>= 60){			//Every Min
				t.sec=0;
				t.min++;
				if(t.min>=60){		//Every Hr
						t.min=0;
						t.hr++;
					}

			}
			//dequeue struct
		}
	if (Q_isEmpty(&timeupdate))
	{
		Q_enqueue((void*) tptr, &runtime);
		Q_enqueue((void*) 1, &timeupdate);
	}


}
void InitializeClock()
{//before we changed to timer2
	//pg82		   |------CTC Mode-------|Normal port operation|------------clk/8------------|
	TCCR0=(1<<FOC0)|(1<<WGM01)|(0<<WGM00)|(0<<COM01)|(0<<COM00)|(0<<CS02)|(1<<CS01)|(0<<CS00);
	OCR0=249;							//system clock is 8mhz
	TIMSK=TIMSK|(1<<OCIE0);				//Set interrupts
   /*
	*				fclk_I/O			8MHz = .125 us	6MHz = 0.166666 us	4MHz = .25 us	1MHz = 1 us
	*	fOCn=	-----------------
	*			2 (N) (1 + OCRn)
	*/	
}
