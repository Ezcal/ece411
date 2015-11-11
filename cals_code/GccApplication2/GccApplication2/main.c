/*
 * GccApplication2.c
 *
 * Created: 11/2/2015 19:59:22 PM
 * Author : Cal
 */ 

#include <avr/io.h>


int main(void)
{
	DDRD = 0xFF; //Make D pins all outputs
	DDRB |= (1<<0); //Make pin B0 output
	PORTD = 0x00; //Set all D pins low
	PORTB &= (0<<0); //Set pin B0 to low
	unsigned int adc_value; //to store result of ADC conversion
	ADCSRA = (1<<ADPS2)|(1<<ADEN); //
	
    while (1) 
    {
		// @@@@@@@@@@@@@@@@@@@@@@@@@
		// Check X Axis
		// @@@@@@@@@@@@@@@@@@@@@@@@@
		ADCSRA |= (1<<ADSC); //enable ADC
		ADMUX = 0x02; //input on PC2
		while (ADCSRA & (1<<ADSC)); //wait for conversion to complete
		adc_value = ADCW; //store result of conversion
		//Centered
		if (adc_value >= 148 && adc_value <= 152){
			PORTD &= (0<<0); //Turn X_CTRL pin off
			PORTD |= (1<<4); //Turn Center LED on
		}
		// ************ Negative X Values
		//Negative X1
		else if (adc_value < 148 && adc_value >= 144){
			PORTD &= (0<<4)&(0<<2)&(0<<1); //turn center pin and select pins off
			PORTD |= (1<<3)|(1<<0); //turn correct select pin on and X_CTRL power on
		}
		//Negative X2
		else if (adc_value < 144 && adc_value >= 140){
			PORTD &= (0<<4)&(0<<2); //turn center off and correct select pins off
			PORTD |= (1<<3)|(1<<1)|(1<<0); //turn on correct select pins and turn X_CTRL on
		}
		//Negative X3
		else if (adc_value < 140 && adc_value >= 136){
			PORTD &= (0<<4)&(0<<1); //turn center off and correct select pins off
			PORTD |= (1<<3)|(1<<2)|(1<<0); //turn on correct select pins and X_CTRL
		}
		//Negative X4
		else if (adc_value < 136){
			PORTD &= (0<<4); //Turn Center off
			PORTD |= (1<<3)|(1<<2)|(1<<1)|(1<<0); //Turn select pins on and make sure X_CTRL is on
		}
		// ************ Positive X Values
		//Positive X1
		else if (adc_value > 152 && adc_value <= 156){
			PORTD &= (0<<4)&(0<<1)&(0<<2)&(0<<3); //turn center and select pins all off
			PORTD |= (1<<0); //make sure X_CTRL is on
		}
		//Positive X2
		else if (adc_value > 156 && adc_value <= 160){
			PORTD &= (0<<4)&(0<<3)&(0<<2); //turn center and correct select pins off
			PORTD |= (1<<1)|(1<<0); //turn select pin and X_CTRL on
		}
		//Positive X3
		else if (adc_value > 160 && adc_value <= 164){
			PORTD &= (0<<4)&(0<<3)&(0<<1);
			PORTD |= (1<<2)|(1<<0);
		}
		//Positive X4
		else if (adc_value > 164){
			PORTD &= (0<<4)&(0<<3);
			PORTD |= (1<<2)|(1<<1)|(1<<0);
		}
		
		// @@@@@@@@@@@@@@@@@@@@@@@@@
		// Check Y Axis
		// @@@@@@@@@@@@@@@@@@@@@@@@@
		ADCSRA |= (1<<ADSC); //enable ADC
		ADMUX = 0x01; //input on pin PC1
		while (ADCSRA & (1<<ADSC)); //Wait for conversion to complete
		adc_value = ADCW; //store result of conversion
		//Centered
		if (adc_value >= 148 && adc_value <= 152){
			PORTD &= (0<<5); //Set Y_CTRL pin low
			PORTD |= (1<<4); //Set Center LED high
		}
		// ************ Negative Y Values
		//Negative Y1
		else if (adc_value < 148 && adc_value >= 144){
			PORTD &= (0<<7)&(0<<6)&(0<<4); //Set Center pin and corresponding select pins low
			PORTB |= (1<<0); //Set corresponding select pin high
			PORTD |= (1<<5); //Make sure Y_CTRL pin is high			
		}
		//Negative Y2
		else if (adc_value < 144 && adc_value >= 140){
			PORTD &= (0<<7)&(0<<4); //Set Center and corresponding select pins low
			PORTB |= (1<<0); //Set corresponding select pin high
			PORTD |= (1<<6)|(1<<5); //continue w/select pins and make sure Y_CTRL is high
		}
		//Negative Y3
		else if (adc_value < 140 && adc_value >= 136){
			PORTD &= (0<<6)&(0<<4); //Set Center and corresponding select pins low
			PORTB |= (1<<0); //Set corresponding select pin high
			PORTD |= (1<<7)|(1<<5); //continue with select pins and make sure Y_CTRL is high
		}
		//Negative Y4
		else if (adc_value < 136){
			PORTD &= (0<<4); //Set Center low
			PORTB |= (1<<0); //Set corresponding select pin high
			PORTD |= (1<<7)|(1<<6)|(1<<5); //Set remaining select pins and Y_CTRL high
		}
		// ************ Positive Y Values
		//Positive Y1
		else if (adc_value > 152 && adc_value <= 156){
			PORTD &= (0<<7)&(0<<6)&(0<<4); //Set Center low and corresponding select pins
			PORTB &= (0<<0); //Set corresponding select pin low
			PORTD |= (1<<5); //Set Y_CTRL high
		}
		//Positive Y2
		else if (adc_value > 156 && adc_value <= 160){
			PORTD &= (0<<7)&(0<<4); //Set Center and corresponding select pins low
			PORTB &= (0<<0); //Set corresponding select pin low
			PORTD |= (1<<6)|(1<<5); //Set corresponding select pins and Y_CTRL high
		}
		//Positive Y3
		else if (adc_value > 160 && adc_value <= 164){
			PORTD &= (0<<6)&(0<<4); //Set Center and select pins low
			PORTB &= (0<<0); //Set corresponding select pin low
			PORTD |= (1<<7)|(1<<5); //Set corresponding select pins and Y_CTRL high
		}
		//Positive Y4
		else if (adc_value > 164){
			PORTD &= (0<<4); //Set Center low
			PORTB &= (0<<0); //Set corresponding select pin low
			PORTD |= (1<<7)|(1<<6)|(1<<5); //Set corresponding select pins and Y_CTRL high
		}
    }
}

