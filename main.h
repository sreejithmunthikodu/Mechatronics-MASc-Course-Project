#include <avr/interrupt.h>
#include <avr/io.h>

// Global Variables
volatile char STATE;
volatile uint16_t ADC_result;
volatile unsigned int ADC_result_flag;
volatile unsigned int PAUSE;
volatile unsigned int STATION;

volatile uint16_t reflectanceTemp;
volatile uint16_t minADC;

//volatile unsigned int i =0;
volatile unsigned int objectCount;
volatile unsigned int countAlum;
volatile unsigned int countSteel;
volatile unsigned int countWhite;
volatile unsigned int countBlack;
