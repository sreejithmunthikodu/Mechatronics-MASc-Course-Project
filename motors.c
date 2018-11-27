#include "motors.h"
#include "mTimer.h"
#include "LinkedQueue.h"

//function to start DC motor
void startDCMotor(){
	PORTB = 0b00000010; // start the conveyor
}

//function to stop DC motor
void stopDCMotor(){
	PORTB = 0b00000000; // stop the conveyor
}

//function to initialize bin to home position
void initStepperMotor(){
	while((PINF & 0b00000100) == 0b00000100){
		rotate_cw(2);
	}
}

//function for clockwise rotation
int rotate_cw(int steps){
	int count=0;
	int delay;
	int acceleration=1;
	int delta;
	delta=maxdelay-mindelay;
	delay=maxdelay;

	while(steps>count){
		if(position==4){
			PORTA= 0b00011011; /*clockwise rotation through 0.9 degrees, M2, L4 energized */
			mTimer(delay);
			count++;
			if ((delay>mindelay)&(acceleration==1)) /* check if in acceleration mode */
			{
				delay=delay-1;						/* decrease delay by 5 milliseconds or accelerate */
			}
			else if((steps-count)<delta)				/* check if in deceleration mode */
			{
				delay=delay+1;						/* increase delay by 5 milliseconds or accelerate */
				acceleration=0;						/* set to deceleration mode */
			}
			position = 1;
		} else if (position==1){
			PORTA = 0b00011101; /* half step clockwise rotation through 0.9 degrees, M1, L2 energized */
			mTimer(delay);
			count++;
			if ((delay>mindelay)&(acceleration==1)) /* check if in acceleration mode */
			{
				delay=delay-1;						/* decrease delay by 5 milliseconds or accelerate */
			}
			else if((steps-count)<delta)				/* check if in deceleration mode */
			{
				delay=delay+1;						/* increase delay by 5 milliseconds or accelerate */
				acceleration=0;						/* set to deceleration mode */
			}
			position = 2;
		} else if (position==2){
			PORTA=0b00101101; /*clockwise rotation through 0.9 degrees, M1, L2 energized */
			mTimer(delay);
			count++;
			if ((delay>mindelay)&(acceleration==1)) /* check if in acceleration mode */
			{
				delay=delay-1;						/* decrease delay by 5 milliseconds or accelerate */
			}
			else if((steps-count)<delta)				/* check if in deceleration mode */
			{
				delay=delay+1;						/* increase delay by 5 milliseconds or accelerate */
				acceleration=0;						/* set to deceleration mode */
			}
			position = 3;
		} else if (position==3){
			PORTA =0b00101011; /* half step clockwise rotation through 0.9 degrees, M2, L3 energized */
			mTimer(delay);
			count++;
			if ((delay>mindelay)&(acceleration==1)) /* check if in acceleration mode */
			{
				delay=delay-1;						/* decrease delay by 5 milliseconds or accelerate */
			}
			else if((steps-count)<delta)				/* check if in deceleration mode */
			{
				delay=delay+1;						/* increase delay by 5 milliseconds or accelerate */
				acceleration=0;						/* set to deceleration mode */
			}
			position = 4;
		}// end if
	} /* end while */
	return(0);
} /* end rotate_ccw */

//function for counter clockwise rotation
int rotate_ccw(int steps){
	int count=0;
	int delay;
	int acceleration=1;
	int delta;
	delta=maxdelay-mindelay;
	delay=maxdelay;
	while(steps>count){
		if(position==4){
			PORTA= 0b00101011; /*clockwise rotation through 0.9 degrees, M2, L4 energized */
			mTimer(delay);
			count++;
			if ((delay>mindelay)&(acceleration==1)) /* check if in acceleration mode */
			{
				delay=delay-1;						/* decrease delay by 5 milliseconds or accelerate */
			}
			else if((steps-count)<delta)				/* check if in deceleration mode */
			{
				delay=delay+1;						/* increase delay by 5 milliseconds or accelerate */
				acceleration=0;						/* set to deceleration mode */
			}
			position = 1;
		} else if (position==1){
			PORTA = 0b00101101; /* half step clockwise rotation through 0.9 degrees, M1, L2 energized */
			mTimer(delay);
			count++;
			if ((delay>mindelay)&(acceleration==1)) /* check if in acceleration mode */
			{
				delay=delay-1;						/* decrease delay by 5 milliseconds or accelerate */
			}
			else if((steps-count)<delta)				/* check if in deceleration mode */
			{
				delay=delay+1;						/* increase delay by 5 milliseconds or accelerate */
				acceleration=0;						/* set to deceleration mode */
			}
			position = 2;
		} else if (position==2){
			PORTA=0b00011101; /*clockwise rotation through 0.9 degrees, M1, L2 energized */
			mTimer(delay);
			count++;
			if ((delay>mindelay)&(acceleration==1)) /* check if in acceleration mode */
			{
				delay=delay-1;						/* decrease delay by 5 milliseconds or accelerate */
			}
			else if((steps-count)<delta)				/* check if in deceleration mode */
			{
				delay=delay+1;						/* increase delay by 5 milliseconds or accelerate */
				acceleration=0;						/* set to deceleration mode */
			}
			position = 3;
		} else if (position==3){
			PORTA =0b00011011; /* half step clockwise rotation through 0.9 degrees, M2, L3 energized */
			mTimer(delay);
			count++;
			if ((delay>mindelay)&(acceleration==1)) /* check if in acceleration mode */
			{
				delay=delay-1;						/* decrease delay by 5 milliseconds or accelerate */
			}
			else if((steps-count)<delta)				/* check if in deceleration mode */
			{
				delay=delay+1;						/* increase delay by 5 milliseconds or accelerate */
				acceleration=0;						/* set to deceleration mode */
			}
			position = 4;
		}
	} /* end while */
	return(0);
} /* end rotate_ccw */
