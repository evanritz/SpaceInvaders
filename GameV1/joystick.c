// joystick.c

// This file adds functionality for some joystick I got in a ardunio kit
// Uses PORTE Analog pins: X - PE2 & Y - PE1
// Uses PORTE digital pin: SW - PE0

// Joystick_Init - enables adc for x and y and digital sw
// Joystick_Update - polls adc regs for x and y vals

// Joystick_X - returns Left or Right based on adc x val
// Joystick_Y - returns Up or Down based on adc y val
// Joystick_SW - returns true or false based on digital sw (USE PULL UP RESISTOR BUTTON)

// confirmed working on slide potentiometer and pull up resistor button

#include "tm4c123gh6pm.h"
#include "joystick.h"

volatile unsigned int adc_x, adc_y;

void Joystick_Init(void)
{
		
	// Enable use of PORTE
	SYSCTL_RCGCGPIO_R |= E;    
	while((SYSCTL_PRGPIO_R & E) == 0){};
	
	// SW: Pin 0
	GPIO_PORTE_DIR_R &= ~PE0;
	GPIO_PORTE_DEN_R |= PE0;	
	GPIO_PORTE_PUR_R |= PE0;

	// Y: Pin 1
	SYSCTL_RCGCADC_R |= 0x01;

	GPIO_PORTE_AFSEL_R |= PE1;
	GPIO_PORTE_DEN_R &= ~PE1;
	GPIO_PORTE_AMSEL_R |= PE1;
		
	ADC0_ACTSS_R &= ~0x08;
	ADC0_EMUX_R &= ~0xF0;
	ADC0_SSMUX3_R = 2; // PE1 = AIN2
	ADC0_SSCTL3_R |= 0x06;
	
	ADC0_ACTSS_R |= 0x08;

	// X: Pin 2
	SYSCTL_RCGCADC_R |= 0x02;
		
	GPIO_PORTE_AFSEL_R |= PE2;
	GPIO_PORTE_DEN_R &= ~PE2;
	GPIO_PORTE_AMSEL_R |= PE2;
		
	ADC1_ACTSS_R &= ~0x08;
	
	ADC1_EMUX_R &= ~0xF0;
	ADC1_SSMUX3_R = 1; // PE2 = AIN1
	ADC1_SSCTL3_R |= 0x06;
	
	ADC1_ACTSS_R |= 0x08;	

	
}

void Joystick_Update(void)
{
	
	// Get ADC Y Value from PE1
	ADC0_PSSI_R |= 0x08;
	while((ADC0_RIS_R & 0x08)==0);
	
	adc_y = ADC0_SSFIFO3_R;
	
	ADC0_ISC_R |= 0x08;
	
	// Get ADC X Value from PE2
	ADC1_PSSI_R |= 0x08;
	while((ADC1_RIS_R & 0x08)==0);
	
	adc_x = ADC1_SSFIFO3_R;
	
	ADC1_ISC_R |= 0x08;
}

int Joystick_X(void)
{
	if (ADC_Default_Max < adc_x)
		return Right;
	else if (ADC_Default_Min > adc_x)
		return Left;
	else
		return 0;
}


int Joystick_Y(void)
{
	if (ADC_Default_Max < adc_y)
		return Down;
	else if (ADC_Default_Min > adc_y)
		return Up;
	else
		return 0;
}

bool Joystick_SW(void)
{
	return !(GPIO_PORTE_DATA_R&PE0);
}
