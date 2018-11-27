#include <stdlib.h>
#include <avr/io.h>

void initPWM();
void initADC();
void initInterrupts();
void initLinkedQueue();
void configSensors(int station);

volatile uint16_t minReflectanceAluminum;
volatile uint16_t maxReflectanceAluminum;
volatile uint16_t minReflectanceSteel;
volatile uint16_t maxReflectanceSteel;
volatile uint16_t minReflectanceWhite;
volatile uint16_t maxReflectanceWhite;
volatile uint16_t minReflectanceBlack;
volatile uint16_t maxReflectanceBlack;
