/*
 * DigitalLevel.c
 *
 * Created: 10/30/2015 7:05:53 PM
 *  Author: msabin
 */ 


#include <avr/io.h>
#include "Enums.h"

typedef enum { false = 0, true = 1  } bool;

void adc_init();
unsigned int adc_read(uint16_t);
led_axis_type find_led_axis_type(plane_type, unsigned int);
void light_led(led_axis_type);

int main(void)
{

	// led ctrl
	DDRD |= 1 << DDD0;
	DDRD |= 1 << DDD5;
	 
	//center
	DDRD |= 1 << DDD4;
	
	// x axis
	DDRD |= 1 << DDD1;
	DDRD |= 1 << DDD2;
	DDRD |= 1 << DDD3;
	
	// y axis
	DDRD |= 1 << DDD6;
	DDRD |= 1 << DDD7;
	DDRB |= 1 << DDB0;

	uint16_t ch_x = 0x02;
	uint16_t ch_y = 0x01;

	led_axis_type led_type = NONE; // START WITH THE NONE STATE

    while(1)
    {
		// X-Axis operations
		// Reads from ADC
		// Truns the led on the x axis depending on the converted value
		adc_init();
		unsigned int digital_value_x = adc_read(ch_x);
		led_type = find_led_axis_type(X_PLANE, digital_value_x);
		light_led(led_type);
		
		// Y-Axis Operations
		// Reads from ADC
		// Turns the led on y axis depending on the converted value
		adc_init();
		unsigned int digital_value_y = adc_read(ch_y);
		led_type = find_led_axis_type(Y_PLANE, digital_value_y);
		light_led(led_type);
    }
}

// Compare the values and Select the LED depending on the value.
// This function calls another function that turns the LED ON.
led_axis_type find_led_axis_type(plane_type plane, unsigned int value)
{

	switch (plane)
	{
		case X_PLANE:
		
			if(value > 685 && value < 710){return CENTER;}
		
			if(value >= 710 && value <= 725){return (POS_X1);}
			if(value > 725 && value <= 745){return(POS_X2);}
			if(value > 745 && value <= 760){return(POS_X3);}
			if(value > 760){return(POS_X4);}
			
			if(value > 670 && value <= 685){return(NEG_X1);}
			if(value > 650 && value <= 670){return(NEG_X2);}
			if(value > 625 && value <= 650){return(NEG_X3);}
			if(value < 625){return(NEG_X4);}
		break;
		
		case Y_PLANE:
			
			if(value > 680 && value < 710){return CENTER;}
			
			if(value >= 710 && value <= 725){return(POS_Y1);}
			if(value > 725 && value <= 745){return(POS_Y2);}
			if(value > 745 && value <= 760){return(POS_Y3);}
			if(value > 760){return(POS_Y4);}
			
			if(value > 665 && value <= 680){return(NEG_Y1);}
			if(value > 645 && value <= 665){return(NEG_Y1);}
			if(value > 620 && value <= 645){return(NEG_Y1);}
			if(value < 620){return(NEG_Y1);}
		break;
		
		default:
			return NONE;
		break;
	}	
}

// Turn ON the LED depending on the type of LED value
void light_led(led_axis_type type)
{
	switch(type)
	{
		case POS_X1:
			PORTD &= 0 << PD1; PORTD &= 0 <<  PD2; PORTD &= 0 << PD3;
			break;
		
		case POS_X2:
			PORTD &= 0 << PD1; PORTD &= 0 << PD2; PORTD |= 1 << PD3;
			break;
		
		case POS_X3:
			PORTD &= 0 << PD1; PORTD |= 1 << PD2; PORTD &= 0 << PD3;
			break;
		
		case POS_X4:
			PORTD &= 0 << PD1; PORTD |= 1 << PD2; PORTD |= 1 << PD3;
			break;
		
		case NEG_X1:
			PORTD |= 1 << PD1; PORTD &= 0 << PD2; PORTD &= 0 << PD3;
			break;
		
		case NEG_X2:
			PORTD |= 1 << PD1; PORTD &= 0 << PD2; PORTD |= 1 << PD3;
			break;
		
		case NEG_X3:
			PORTD |= 1 << PD1; PORTD |= 1 << PD2; PORTD &= 0 << PD3;
			break;
		
		case NEG_X4:
			PORTD |= 1 << PD1; PORTD |= 1 << PD2; PORTD |= 1 << PD3;
			break;
		
		case POS_Y1:
			PORTD |= 1 << PD6; PORTD |= 1 << PD7; PORTB |= 1 << PB0;
			break;
		
		case POS_Y2:
			PORTD &= 0 << PD6; PORTD &= 0 << PD7; PORTB |= 1 << PB0;
			break;
		
		case POS_Y3:
			PORTD &= 0 << PD6; PORTD |= 1 << PD7; PORTB &= 0 << PB0;
			break;
		
		case POS_Y4:
			PORTD &= 0 << PD6; PORTD |= 1 << PD7; PORTB |= 1 << PB0;
			break;
		
		case NEG_Y1:
			PORTD |= 1 << PD6; PORTD &= 0 << PD7; PORTB &= 0 << PB0;
			break;
		
		case NEG_Y2:
			PORTD |= 1 << PD6; PORTD &= 0 << PD7; PORTB |= 1 << PB0;
			break;
		
		case NEG_Y3:
			PORTD |= 1 << PD6; PORTD |= 1 << PD7; PORTB &= 0 << PB0;
			break;
		
		case NEG_Y4:
			PORTD |= 1 << PD6; PORTD |= 1 << PD7; PORTB |= 1 << PB0;
			break;
		
		case CENTER:
			PORTD |= 1 << PD4;
			break;
		
		case NONE:
		default:
			PORTD &= 0 << PD0; PORTD &= 0 << PD4; PORTD &=  0 << PD5;
			break;
	}

}

void adc_init()
{
	//AREF = AVcc
	ADMUX = 0xc0;
	
	//ADC Enable and prescaler of 128
	//1000000/16 = 7812.5
	ADCSRA =(1<<ADPS2)|(1<<ADEN);
}

//Read the value of the port and spits
//the Converted digital value
unsigned int adc_read(uint16_t ch)
{
	//select the corresponding channel 0~6
	//ANDing with ’6? will always keep the value
	//of ‘ch’ between 0 and 6
	//ch &= 0b00000110;   AND operation with 6
	//ADMUX = (ADMUX & 0xF8)|ch; // clears the bottom 3 bits before ORing
	ADMUX |= ch;
	
	// start single convertion
	//write ’1? to ADSC
	ADCSRA |= (1<<ADSC);
	
	//wait for conversion to complete
	//ADSC becomes ’0? again
	//till then, run loop continuously
	while(ADCSRA & (1<<ADSC));
	
	return (ADCW);
}
