/*
 * main.c
 *
 * Created: 11/2/2015 19:59:22 PM
 * Author : Calhoun Zabel
 */

#include <avr/io.h>

typedef enum { false = 0, true = 1  } bool; //Thank you Sabin

int main(void)
{
	DDRD = 0xFF; //Make D pins all outputs
	DDRB |= (1<<0); //Make pin B0 output
	PORTD = 0x00; //Set all D pins low
	PORTB &= (0<<0); //Set pin B0 to low
	unsigned int adc_value; //to store result of ADC conversion
	ADCSRA = (1<<ADPS2)|(1<<ADEN); //Set prescaler to 16, (1MHz / 16 = 62.5kHz) and enable conversion
	
	bool centered; //Flag for control of center LED
	
    while (1) 
    {
		// @@@@@@@@@@@@@@@@@@@@@@@@@
		// Check X Axis
		// @@@@@@@@@@@@@@@@@@@@@@@@@
		ADCSRA |= (1<<ADSC); //enable ADC
		ADMUX = 0xC1; //input on PC1
		while (ADCSRA & (1<<ADSC)); //wait for conversion to complete
		adc_value = ADCW; //store result of conversion
		//Centered
		if (adc_value >= 685 && adc_value <= 710){
			if (centered){ //If the previous result was centered (this is from the y-axis)
				PORTD &= (0<<0); //Turn X_CTRL pin off
				PORTD |= (1<<4); //Turn Center LED on
				centered = true; //Set flag for next conversion
			}
			else if (centered == false){ //If the previous result was not centered (y-axis)
				PORTD &= (0<<0)&(0<<4); //Turn X_CTRL and Center LED off
				centered = true; //Set flag for next conversion
			}
		}
		
		// ************ Negative X Values
		//Negative X1
		else if (adc_value < 685 && adc_value >= 670){
			PORTD &= (0<<4)&(0<<2)&(0<<1); //turn center pin and select pins off
			PORTD |= (1<<3)|(1<<0); //turn correct select pin on and X_CTRL power on
			centered = false; //Set flag for next conversion
		}
		//Negative X2
		else if (adc_value < 670 && adc_value >= 650){
			PORTD &= (0<<4)&(0<<2); //turn center off and correct select pins off
			PORTD |= (1<<3)|(1<<1)|(1<<0); //turn on correct select pins and turn X_CTRL on
			centered = false; //Set flag for next conversion
		}
		//Negative X3
		else if (adc_value < 650 && adc_value >= 625){
			PORTD &= (0<<4)&(0<<1); //turn center off and correct select pins off
			PORTD |= (1<<3)|(1<<2)|(1<<0); //turn on correct select pins and X_CTRL
			centered = false; //Set flag for next conversion
		}
		//Negative X4
		else if (adc_value < 625){
			PORTD &= (0<<4); //Turn Center off
			PORTD |= (1<<3)|(1<<2)|(1<<1)|(1<<0); //Turn select pins on and make sure X_CTRL is on
			centered = false; //Set flag for next conversion
		}
		
		// ************ Positive X Values
		//Positive X1
		else if (adc_value > 710 && adc_value <= 725){
			PORTD &= (0<<4)&(0<<1)&(0<<2)&(0<<3); //turn center and select pins all off
			PORTD |= (1<<0); //make sure X_CTRL is on
			centered = false; //Set flag for next conversion
		}
		//Positive X2
		else if (adc_value > 725 && adc_value <= 745){
			PORTD &= (0<<4)&(0<<3)&(0<<2); //turn center and correct select pins off
			PORTD |= (1<<1)|(1<<0); //turn select pin and X_CTRL on
			centered = false; //Set flag for next conversion
		}
		//Positive X3
		else if (adc_value > 745 && adc_value <= 760){
			PORTD &= (0<<4)&(0<<3)&(0<<1);
			PORTD |= (1<<2)|(1<<0);
			centered = false; //Set flag for next conversion
		}
		//Positive X4
		else if (adc_value > 760){
			PORTD &= (0<<4)&(0<<3);
			PORTD |= (1<<2)|(1<<1)|(1<<0);
			centered = false; //Set flag for next conversion
		}
		
		// @@@@@@@@@@@@@@@@@@@@@@@@@
		// Check Y Axis
		// @@@@@@@@@@@@@@@@@@@@@@@@@
		ADCSRA |= (1<<ADSC); //enable ADC
		ADMUX = 0xC2; //input on pin PC2
		while (ADCSRA & (1<<ADSC)); //Wait for conversion to complete
		adc_value = ADCW; //store result of conversion
		//Centered
		if (adc_value >= 680 && adc_value <= 705){
			if (centered){ //If previous result was centered
				PORTD &= (0<<5); //Set Y_CTRL pin low
				PORTD |= (1<<4); //Set Center LED high
				centered = true; //Set flag for next conversion
			}
			else if (centered == false){ //If previous result was not centered (from x-axis)
				PORTD &= (0<<5)&(0<<4); //Set Y_CTRL and Center low
				centered = true; //Set flag for next conversion
			}
		}
		
		// ************ Negative Y Values
		//Negative Y1
		else if (adc_value < 680 && adc_value >= 665){
			PORTD &= (0<<7)&(0<<6)&(0<<4); //Set Center pin and corresponding select pins low
			PORTB |= (1<<0); //Set corresponding select pin high
			PORTD |= (1<<5); //Make sure Y_CTRL pin is high			
			centered = false; //Set flag for next conversion
		}
		//Negative Y2
		else if (adc_value < 665 && adc_value >= 645){
			PORTD &= (0<<7)&(0<<4); //Set Center and corresponding select pins low
			PORTB |= (1<<0); //Set corresponding select pin high
			PORTD |= (1<<6)|(1<<5); //continue w/select pins and make sure Y_CTRL is high
			centered = false; //Set flag for next conversion
		}
		//Negative Y3
		else if (adc_value < 645 && adc_value >= 620){
			PORTD &= (0<<6)&(0<<4); //Set Center and corresponding select pins low
			PORTB |= (1<<0); //Set corresponding select pin high
			PORTD |= (1<<7)|(1<<5); //continue with select pins and make sure Y_CTRL is high
			centered = false; //Set flag for next conversion
		}
		//Negative Y4
		else if (adc_value < 620){
			PORTD &= (0<<4); //Set Center low
			PORTB |= (1<<0); //Set corresponding select pin high
			PORTD |= (1<<7)|(1<<6)|(1<<5); //Set remaining select pins and Y_CTRL high
			centered = false; //Set flag for next conversion
		}
		
		// ************ Positive Y Values
		//Positive Y1
		else if (adc_value > 710 && adc_value <= 725){
			PORTD &= (0<<7)&(0<<6)&(0<<4); //Set Center low and corresponding select pins
			PORTB &= (0<<0); //Set corresponding select pin low
			PORTD |= (1<<5); //Set Y_CTRL high
			centered = false; //Set flag for next conversion
		}
		//Positive Y2
		else if (adc_value > 725 && adc_value <= 745){
			PORTD &= (0<<7)&(0<<4); //Set Center and corresponding select pins low
			PORTB &= (0<<0); //Set corresponding select pin low
			PORTD |= (1<<6)|(1<<5); //Set corresponding select pins and Y_CTRL high
			centered = false; //Set flag for next conversion
		}
		//Positive Y3
		else if (adc_value > 745 && adc_value <= 760){
			PORTD &= (0<<6)&(0<<4); //Set Center and select pins low
			PORTB &= (0<<0); //Set corresponding select pin low
			PORTD |= (1<<7)|(1<<5); //Set corresponding select pins and Y_CTRL high
			centered = false; //Set flag for next conversion
		}
		//Positive Y4
		else if (adc_value > 760){
			PORTD &= (0<<4); //Set Center low
			PORTB &= (0<<0); //Set corresponding select pin low
			PORTD |= (1<<7)|(1<<6)|(1<<5); //Set corresponding select pins and Y_CTRL high
			centered = false; //Set flag for next conversion
		}
    }
}
