#include "mTimer.h"

//########################################################################
//function for timer in milliseconds
void mTimer(int count){
	TCCR1B|=(1<<CS10)|(1<<WGM12); //set timer control prescaling to none and mode
	//to Clear Time on Compare, CTC.
	OCR1A=1000; // Output compare register is set to 1000D.
	TCNT1=0; // Initialize counter
	//TIMSK1|= (0<<1);//Timer counter output compare A match is disabled.
	TIFR1|=(1<<OCF1A); //Timer counter interrupt flag is cleared and timer is started.

	int i=0; // for counting the loop
	while(i<count){
		if((TIFR1&(0b00000010))==0b00000010){
			TIFR1|=(1<<OCF1A); //Timer counter interrupt flag is cleared.
			i++;
		}//if
	}//while
}//mTimer
