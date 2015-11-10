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
uint16_t adc_read(uint8_t);
axis_type axis(uint16_t, plane_type);
void led_binary(axis_type, uint16_t);
void led_select(led_axis_type);

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

	uint8_t ch_x = 2;
	uint8_t ch_y = 1;

	adc_init();

    while(1)
    {
		// X-Axis operations
		// Reads from ADC
		// Truns the led on the x axis depending on the converted value
		uint16_t digital_value_x = adc_read(ch_x);
		axis_type axis_x = axis(digital_value_x, X_PLANE);
		led_binary(axis_x, digital_value_x);
		
		// Y-Axis Operations
		// Reads from ADC
		// Turns the led on y axis depending on the converted value
		uint16_t digital_value_y = adc_read(ch_y);
		axis_type axis_y = axis(digital_value_y, Y_PLANE);
		led_binary(axis_y, digital_value_y);
		
    }
}

void adc_init()
{
	 //AREF = AVcc
	 ADMUX = (1<<REFS0);
	
	 //ADC Enable and prescaler of 128
	 //1000000/16 = 7812.5
	ADCSRA = (1<<ADEN)|(1<<ADPS2);
}

 //Read the value of the port and spits
 //the Converted digital value
uint16_t adc_read(uint8_t ch)
{
	 //select the corresponding channel 0~6
	 //ANDing with ’6? will always keep the value
	 //of ‘ch’ between 0 and 6
	//ch &= 0b00000110;   AND operation with 6
	ADMUX = (ADMUX & 0xF8)|ch; // clears the bottom 3 bits before ORing
	
	// start single convertion
	 //write ’1? to ADSC
	ADCSRA |= (1<<ADSC);
	
	 //wait for conversion to complete
	 //ADSC becomes ’0? again
	 //till then, run loop continuously
	while(ADCSRA & (1<<ADSC));
	
	return (ADCW);
}


axis_type axis(uint16_t value, plane_type plane)
{
	
	if(value < 158 && value > 149){return CENTERED;}
		
	if (plane == X_PLANE)
	{
		bool pos_x_axis = value <= 205 && value >= 158;
		bool neg_x_axis = value <= 149 && value >= 102;
		
		if(pos_x_axis){
			return POS_X_AXIS;
		}
		if(neg_x_axis){
			return NEG_X_AXIS;
		}
	}
	
	if (plane == Y_PLANE)
	{
		bool pos_y_axis = value <= 158 && value >= 205;
		bool neg_y_axis = value <= 149 && value >= 102;
		
		if(pos_y_axis){
			return POS_Y_AXIS;
		}
		if(neg_y_axis){
			return NEG_Y_AXIS;
		}
	}

	//no range to fit the axis
	return NONE;
	
}

// Compare the values and Select the LED depending on the value.
// This function calls another function that turns the LED ON.
void led_binary(axis_type axis, uint16_t value)
{
	PORTD &= 0 << PD0; PORTD &= 0 << PD4; PORTD &=  0 << PD5;
	
	switch(axis)
	{
		case POS_X_AXIS:
			PORTD |= 1 << PD0; 
			if(value > 154 && value <= 158){led_select(POS_X1);}
			if(value > 158 && value <= 163){led_select(POS_X2);}
			if(value > 163 && value <= 173){led_select(POS_X3);}
			if(value > 173 && value <= 205){led_select(POS_X4);}
			break;
		
		case NEG_X_AXIS:
			PORTD |= 1 << PD0; 
			if(value > 154 && value <= 149){led_select(NEG_X1);}
			if(value > 149 && value <= 144){led_select(NEG_X2);}
			if(value > 144 && value <= 135){led_select(NEG_X3);}
			if(value > 135 && value <= 102){led_select(NEG_X4);}
			break;
		
		case POS_Y_AXIS:
			PORTD |=  1 << PD5;
			if(value > 154 && value <= 158){led_select(POS_Y1);}
			if(value > 158 && value <= 163){led_select(POS_Y2);}
			if(value > 163 && value <= 172){led_select(POS_Y3);}
			if(value > 172 && value <= 205){led_select(POS_Y4);}
			break;
		
		case NEG_Y_AXIS:
			PORTD |=  1 << PD5;
			if(value > 154 && value <= 149){led_select(NEG_Y1);}
			if(value > 149 && value <= 145){led_select(NEG_Y1);}
			if(value > 145 && value <= 135){led_select(NEG_Y1);}
			if(value > 135 && value <= 102){led_select(NEG_Y1);}
			break;
		
		case CENTERED:
			led_select(CNTR);
			break;
		
		case NONE:
		default:
			led_select(NOTHING);
			break;
	}
}


// Turn ON the LED depending on the type of LED value
void led_select(led_axis_type type)
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
		
		case CNTR:
			PORTD |= 1 << PD4;
			break;
		
		case NOTHING:
		default:
			PORTD &= 0 << PD0; PORTD &= 0 << PD4; PORTD &=  0 << PD5;
			break;
	}

}
