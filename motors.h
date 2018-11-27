#include <stdlib.h>
#include <avr/io.h>

volatile unsigned int position;
volatile unsigned int currentBin;
volatile unsigned int maxdelay;
volatile unsigned int mindelay;

void startDCMotor();
void stopDCMotor();
void initStepperMotor();
int rotate_cw(int steps);
int rotate_ccw(int steps);
