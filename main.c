#include "main.h"
#include "LinkedQueue.h"
#include "mTimer.h"
#include "init.h"
#include "motors.h"
#include "lcd.h"

int main(){

	STATE = 0;
	PAUSE = 0;
	STATION = 8;
	position = 4;
	mindelay =6;
	maxdelay =27;

	objectCount = 0;
	countAlum = 0;
	countSteel = 0;
	countWhite = 0;
	countBlack = 0;

	cli();	// Disables all interrupts

	DDRA = 0xff; //Set PORTA to output for stepper motor
	DDRB = 0xff; //Set PORTB to output for DC motor
	DDRC = 0xff; //Set PORTC to output for LCD
	DDRD = 0b11110000; //Set PORTD to input for 4 sensors and 2 push buttons (default)

	rotate_cw(101);
	initStepperMotor(); // initialize stepper motor to home position
	currentBin = 3;
	initPWM(); // initialize PWM
	initInterrupts(); // initialize interrupts
	startDCMotor(); // start DC motor
	initADC(); // initialize ADC
	initLinkedQueue(); // initialize linked queue
	configSensors(STATION); // set sensor thresholds based on station


	// Enable all interrupts
	sei();	// Note this sets the Global Enable for all interrupts

	goto POLLING_STAGE;

	// POLLING STATE
	POLLING_STAGE:
		switch(STATE){
			case (0) :
				goto POLLING_STAGE;
				break;	//not needed but syntax is correct
			case (1) :
				goto PAUSE_STAGE;
				break;
			case (2) :
				goto RAMPDOWN_STAGE;
				break;
			case (5) :
				goto END;
			default :
				goto POLLING_STAGE;
		}//switch STATE

	// PAUSE STATE
	PAUSE_STAGE:
		//Reset the state variable
		STATE = 0;
		goto POLLING_STAGE;

	// RAMPDOWN STATE
	RAMPDOWN_STAGE:
		mTimer(6000);
		PORTB = 0x00;
		cli();
		printLCD();

		//Reset the state variable
		STATE = 0;
		goto POLLING_STAGE;

	END:
		// The closing STATE ... how would you get here? ANS: When ramp down button is pressed?
		PORTA = 0xF0;	// Indicates this state is active
		// Stop everything here...'MAKE SAFE'
	return(0);
}

// INT0 is triggered by pre-reflectance optical sensor
ISR(INT0_vect){
	ADCSRA |= _BV(ADSC); // initialize the ADC, start one conversion
	minADC = 0xffff;
	objectCount += 1;
}

// triggered when each ADC conversion is complete
ISR(ADC_vect){
	ADC_result = ADCL | ADCH<<8;
 	if(ADC_result < minADC){
		minADC = ADC_result;
	}

	if((PIND & 0b00000001) == 1){
	 	ADCSRA |= _BV(ADSC); // initialize the ADC, start one conversion
	} else {
		initLink(&newLink);
		newLink->o.reflectance = minADC;
		enqueue(&head, &tail, &newLink);
		if((newLink->o.reflectance >= minReflectanceAluminum) && (newLink->o.reflectance < maxReflectanceAluminum)){
			newLink->o.objectType = 0;
		} else if ((newLink->o.reflectance >= minReflectanceSteel) && (newLink->o.reflectance < maxReflectanceSteel)){
			newLink->o.objectType = 1;
		} else if ((newLink->o.reflectance >= minReflectanceWhite) && (newLink->o.reflectance < maxReflectanceWhite)){
			newLink->o.objectType = 2;
		} else if ((newLink->o.reflectance >= minReflectanceBlack) && (newLink->o.reflectance < maxReflectanceBlack)){
			newLink->o.objectType = 3;
		} else {
			while(1){
				stopDCMotor();
			}
		}
		STATE = 0;
	}
}

// INT1 triggered by exit optical sensor
ISR(INT1_vect){
	//mTimer(30);
	dequeue(&head, &tail, &rtnLink);
	if(rtnLink->o.objectType == 0){ //	PORTC = 1;
		if(currentBin == 0){
			// do nothing
		} else if(currentBin == 1){
			stopDCMotor();
			rotate_cw(100);
		} else if(currentBin == 2){
			stopDCMotor();
			rotate_ccw(50);
		} else if(currentBin == 3){
			stopDCMotor();
			rotate_cw(50);
		}
		currentBin = 0;
	} else if(rtnLink->o.objectType == 1){ //	PORTC = 2;
		if(currentBin == 0){
			stopDCMotor();
			rotate_cw(100);
		} else if(currentBin == 1){
			// do nothing
		} else if(currentBin == 2){
			stopDCMotor();
			rotate_cw(50);
		} else if(currentBin == 3){
			stopDCMotor();
			rotate_ccw(50);
		}
		currentBin = 1;
	} else if(rtnLink->o.objectType == 2){ //	PORTC = 3;
		if(currentBin == 0){
			stopDCMotor();
			rotate_cw(50);
		} else if(currentBin == 1){
			stopDCMotor();
			rotate_ccw(50);
		} else if(currentBin == 2){
			// do nothing
		} else if(currentBin == 3){
			stopDCMotor();
			rotate_cw(100);
		}
		currentBin = 2;
	} else if(rtnLink->o.objectType == 3){  //		PORTC = 4;
		if(currentBin == 0){
			stopDCMotor();
			rotate_ccw(50);
		} else if(currentBin == 1){
			stopDCMotor();
			rotate_cw(50);
		} else if(currentBin == 2){
			stopDCMotor();
			rotate_cw(100);
		} else if(currentBin == 3){
			// do nothing;
		}
		currentBin = 3;
	}

	startDCMotor();
	mTimer(20);

	if(rtnLink->o.objectType == 0){ //	PORTC = 1;
		countAlum++;
	} else if(rtnLink->o.objectType == 1){ //	PORTC = 2;
		countSteel++;
	} else if(rtnLink->o.objectType == 2){ //	PORTC = 3;
		countWhite++;
	} else if(rtnLink->o.objectType == 3){  //		PORTC = 4;
		countBlack++;
	}

	objectCount -= 1;
	free(rtnLink);
    STATE = 0;
}

// INT3 triggered by system pause button
ISR(INT2_vect){
	mTimer(30); /* Wait 20ms */
	while((PIND & 0b00000100) == 0); 	/* Wait till button is released */
	mTimer(30); /* Wait 20ms */
	if(PAUSE == 0){
		stopDCMotor();
		PAUSE = 1;
		STATE = 1;
		printLCD();
	} else {
		startDCMotor();
		PAUSE = 0;
		STATE = 0;
	}
}

// INT4 triggered by system ramp-down button
ISR(INT3_vect){
	mTimer(30); /* Wait 20ms */
	while((PIND & 0b00001000) == 0); 	/* Wait till button is released */
	mTimer(30); /* Wait 20ms */

	//rampDown = 1;
	STATE = 2;
}

ISR(BADISR_vect){
	while(1){
		Lcd4_Init();
		Lcd4_Clear();
		Lcd4_Set_Cursor(1,0);
		Lcd4_Write_String("********************" );
		Lcd4_Set_Cursor(2,0);
		Lcd4_Write_String("**     ERROR!     **" );
		Lcd4_Set_Cursor(3,0);
		Lcd4_Write_String("**                **" );
		Lcd4_Set_Cursor(4,0);
		Lcd4_Write_String("********************" );
	}
}
