/*
 * DigitalLevel.c
 *
 * Created: 10/30/2015 7:05:53 PM
 *  Author: msabin
 */ 



#include <avr/io.h>
#include "Enums.h"

typedef enum { false=0, true=1  } bool;

//#include "ADC.h"
//#include "Axis.h"
//#include "LightUp.h"

int main(void)
{
    while(1)
    {
		
    }
}

void adc_init()
{
	 //AREF = AVcc
	 ADMUX = (1<<REFS0);
	
	 //ADC Enable and prescaler of 128
	 //1000000/128 = 7812.5
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
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
	
	return (ADC);
}


axis_type axis(uint16_t value)
{

	bool pos_x_axis = value <= 205 && value >= 158;
	bool neg_x_axis = value <= 149 && value >= 102;
	bool pos_y_axis = value <= 158 && value >= 205;
	bool neg_y_axis = value <= 149 && value >= 102;
	bool centered = value == 154 ;
	
	if(pos_x_axis){
		return POS_X_AXIS;
	}
	else if(neg_x_axis){
		return NEG_X_AXIS;
	}
	else if(pos_y_axis){
		return POS_Y_AXIS;
	}
	else if(neg_y_axis){
		return NEG_Y_AXIS;
	}
	else if(centered)
	{
		return CENTERED;
	}
	
	//no range to fit the axis
	return NONE;
	
}


void led_binary(axis_type axis, uint16_t value)
{
	switch(axis)
	{
		case POS_X_AXIS:
		if(value > 154 && value <= 158){led_select(POS_X1);}
		if(value > 158 && value <= 163){led_select(POS_X2);};
		if(value > 163 && value <= 173){led_select(POS_X3);};
		if(value > 173 && value <= 205){led_select(POS_X4);};
		break;
		
		case NEG_X_AXIS:
		if(value > 154 && value <= 149){led_select(NEG_X1);}
		if(value > 149 && value <= 144){led_select(NEG_X2);};
		if(value > 144 && value <= 135){led_select(NEG_X3);};
		if(value > 135 && value <= 102){led_select(NEG_X4);};
		break;
		
		case POS_Y_AXIS:
		if(value > 154 && value <= 158){led_select(POS_Y1);}
		if(value > 158 && value <= 163){led_select(POS_Y2);};
		if(value > 163 && value <= 172){led_select(POS_Y3);};
		if(value > 172 && value <= 205){led_select(POS_Y4);};
		break;
		
		case NEG_Y_AXIS:
		if(value > 154 && value <= 149){led_select(NEG_Y1);}
		if(value > 149 && value <= 145){led_select(NEG_Y1);};
		if(value > 145 && value <= 135){led_select(NEG_Y1);};
		if(value > 135 && value <= 102){led_select(NEG_Y1);};
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


 //X LED CTRL = PD0
 //Y LED CTRL = PD5

void led_select(led_axis_type type)
{

	PD0 = 1; PD5 = 1;
	
	switch(type)
	{
		case POS_X1:
		PD1 = 0; PD2 = 0; PD3 = 0;
		case POS_X2:
		PD1 = 0; PD2 = 0; PD3 = 1;
		case POS_X3:
		PD1 = 0; PD2 = 1; PD3 = 0;
		case POS_X4:
		PD1 = 0; PD2 = 1; PD3 = 1;
		case NEG_X1:
		PD1 = 1; PD2 = 0; PD3 = 0;
		case NEG_X2:
		PD1 = 1; PD2 = 0; PD3 = 1;
		case NEG_X3:
		PD1 = 1; PD2 = 1; PD3 = 0;
		case NEG_X4:
		PD1 = 1; PD2 = 1; PD3 = 1;
		case POS_Y1:
		PD6 = 0; PD7 = 0; PB0 = 0;
		case POS_Y2:
		PD6 = 0; PD7 = 0; PB0 = 1;
		case POS_Y3:
		PD6 = 0; PD7 = 1; PB0 = 0;
		case POS_Y4:
		PD6 = 0; PD7 = 1; PB0 = 1;
		case NEG_Y1:
		PD6 = 1; PD7 = 0; PB0 = 0;
		case NEG_Y2:
		PD6 = 1; PD7 = 0; PB0 = 1;
		case NEG_Y3:
		PD6 = 1; PD7 = 1; PB0 = 0;
		case NEG_Y4:
		PD6 = 1; PD7 = 1; PB0 = 1;
		case CNTR:
		PD4 = 1;
		case NOTHING:
		PD0 = 0; PD4 = 0; PD5 = 0;
	}

}
