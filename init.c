#include "init.h"

void initPWM(){
	/* Generating PWM */
	 TCCR0A|=1<<WGM00|1<<WGM01;		/*Set timer to fast PWM mode, update OCRA at TOP=0xFF*/
	 //TIMSK0|=0<<OCIE0A;		 	/*Interrupt is disabled for TCNT0 on A Compare */
	 //TIFR0|=1<<OCF0A;				/*Reset TIFR0 and initiate counter */
	 TCCR0A|=1<<COM0A1;		 		/*Set output mode to non-inverted, OC0A clear on compare, set at TOP */
	 TCCR0B|=1<<CS01;				/*Set Timer pre-scale to 8    */
	 OCR0A=90;						/*Duty Cycle out of 255 */
	 DDRB|=1<<DDB7;					/*Set PORTB PIN7 as output to enable OC0A to control PIN output */
}

void initADC(){
	// by default, the ADC input (analog input is set to be ADC0 / PORTF0
	ADCSRA |= _BV(ADEN); // enable ADC
	ADCSRA |= _BV(ADIE); // enable interrupt of ADC
	ADCSRA |= _BV(ADPS2); // set ADC prescaler to 128
	ADMUX |= _BV(REFS0); // set reference voltage to VCC
	ADMUX |= _BV(MUX0); // use ADC channel 1
	// ADLAR = 0 (default)
}

void initInterrupts(){
	// config the external interrupt ======================================
	EIMSK|= _BV(INT0); // enable INT0 for pre-reflectance optical sensor
	EIMSK|= _BV(INT1); // enable INT1 for exit optical sensor
	EIMSK|= _BV(INT2); // enable INT2 for system pause PB
	EIMSK|= _BV(INT3); // enable INT3 for system ramp-down PB

	EICRA |= _BV(ISC01) | _BV(ISC00); // rising edge INT0 for pre-reflectance optical sensor
	EICRA |= _BV(ISC11); // falling edge INT1 for exit optical sensor
	EICRA |= _BV(ISC21); // falling edge INT2 for system pause PB
	EICRA |= _BV(ISC31); // falling edge INT3 for system ramp-down PB
}

void configSensors(int station){
	if(station == 3){
		minReflectanceAluminum = 0;
		maxReflectanceAluminum = 400;
		minReflectanceSteel = 401;
		maxReflectanceSteel = 775;
		minReflectanceWhite = 776;
		maxReflectanceWhite = 932;
		minReflectanceBlack = 933;
		maxReflectanceBlack = 0xffff;
	} else if(station == 8){
		minReflectanceAluminum = 0;
		maxReflectanceAluminum = 350;
		minReflectanceSteel = 351;
		maxReflectanceSteel = 750;
		minReflectanceWhite = 751;
		maxReflectanceWhite = 936;
		minReflectanceBlack = 937;
		maxReflectanceBlack = 0xfff;
	} else if(station == 11){
		minReflectanceAluminum = 130;
		maxReflectanceAluminum = 210;
		minReflectanceSteel = 520;
		maxReflectanceSteel = 640;
		minReflectanceWhite = 900;
		maxReflectanceWhite = 935;
		minReflectanceBlack = 936;
		maxReflectanceBlack = 980;
	}
}
