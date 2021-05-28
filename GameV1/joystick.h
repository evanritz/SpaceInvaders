#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <stdbool.h>

#define E 0x10

#define PE0 0x01
#define PE1 0x02
#define PE2 0x04

#define Right 1
#define Left 	2
#define Up 		3
#define Down 	4

// ADC value range
#define ADC_Default_Max 2100
#define ADC_Default_Min 2000

void Joystick_Init(void);
void Joystick_Update(void);

bool Joystick_SW(void);
int Joystick_X(void);
int Joystick_Y(void);

#endif
