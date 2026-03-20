// This code is mostly from http://efundies.com/avr-and-printf/

#define F_CPU 16000000UL
#define IR_RATE 36000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdbool.h>
#include <util/delay.h>
#include "usart.h"

#define sBAUD 580
#define sOUTPORT PORTD
#define sTXD (1<<PD2)
#define MIN(a, b) ((a) < (b) ? (a) : (b))
/* Pinout for DIP28 ATMega328P:

                           -------
     (PCINT14/RESET) PC6 -|1    28|- PC5 (ADC5/SCL/PCINT13)
       (PCINT16/RXD) PD0 -|2    27|- PC4 (ADC4/SDA/PCINT12)
       (PCINT17/TXD) PD1 -|3    26|- PC3 (ADC3/PCINT11)
      (PCINT18/INT0) PD2 -|4    25|- PC2 (ADC2/PCINT10)
 (PCINT19/OC2B/INT1) PD3 -|5    24|- PC1 (ADC1/PCINT9)
    (PCINT20/XCK/T0) PD4 -|6    23|- PC0 (ADC0/PCINT8)
                     VCC -|7    22|- GND
                     GND -|8    21|- AREF
(PCINT6/XTAL1/TOSC1) PB6 -|9    20|- AVCC
(PCINT7/XTAL2/TOSC2) PB7 -|10   19|- PB5 (SCK/PCINT5)
   (PCINT21/OC0B/T1) PD5 -|11   18|- PB4 (MISO/PCINT4)
 (PCINT22/OC0A/AIN0) PD6 -|12   17|- PB3 (MOSI/OC2A/PCINT3)
      (PCINT23/AIN1) PD7 -|13   16|- PB2 (SS/OC1B/PCINT2)
  (PCINT0/CLKO/ICP1) PB0 -|14   15|- PB1 (OC1A/PCINT1)
                           -------
*/

volatile unsigned int toggle_flag = 0;

void adc_init(void)
{
    ADMUX = (1<<REFS0);
    ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}

uint16_t adc_read(int pin)
{
    pin &= 0x7;
    ADMUX = (ADMUX & 0xF8)|pin;
     
    ADCSRA |= (1<<ADSC);
     
    while(ADCSRA & (1<<ADSC)); //as long as ADSC pin is 1 just wait.
     
    return (ADCW);
}

void configure_software_UART(void)
{
	DDRD |= sTXD; //Configure sTXD as output
	PORTD |= sTXD; //Default state is 1
//UNUSED	DDRD &= (~sRXD); //Configure sRXD as input
}

void SendByte (unsigned char c)
{
	unsigned char i;
	
	// Send start bit
	//sOUTPORT &= (~sTXD);
	toggle_flag=1;
	_delay_us(1E6/sBAUD);
	// Send 8 data bits
	for (i=0; i<8; i++)
	{
		if( c & 1)
		{
	//		sOUTPORT |= sTXD;
			toggle_flag=0;
		}
		else
		{
	//		sOUTPORT &= (~sTXD);
			toggle_flag=1;
		}
		c>>=1;
		_delay_us(1E6/sBAUD);
	}
	// Send the stop bit
	//sOUTPORT |= sTXD;
	toggle_flag=0;
	_delay_us(1E6/sBAUD);
}

void SendString(char *s)
{
	while(*s!=0) SendByte(*s++);
}

ISR(TIMER1_COMPA_vect)
{
	if(toggle_flag)
    	PINB = (1 << PB1);  // toggle PB1
    else
    	PORTB &= ~(1<<PB1);
 
}

void timer1_init(void)
{
    DDRB |= (1 << PB1); //Set PB1 to output
    TCCR1A = 0x00;

    TCCR1B = (1 << WGM12) | (1 << CS10); // Prescaler = 1
    OCR1A = 210; // Preload such that f=38kHz
    TIMSK1 = (1 << OCIE1A);
}

int main( void )
{
	unsigned int x, y, speed;
//	char buff[8];
	
	usart_init (); // configure the usart and baudrate
	adc_init();
	configure_software_UART();
	timer1_init();
	
	// Putty UART
	DDRB |= 0x01;
	PORTB |= 0x01;
	
	_delay_ms(500); // Give putty some time to start.
	sei(); // Enable global interrupts
	printf("\nJoystick test\n");
	while(1)
	{
		y=adc_read(0);
		x=adc_read(1);
		speed=adc_read(3);
		speed=MIN(speed/10,100);
		printf("x=%d, %fV ; y=%d, %fV, speed=%d                       \r", x, (x*5)/1023.0, y, (y*5)/1023.0, speed);
		_delay_ms(50);
		SendByte(speed);
		_delay_ms(5);
		if(x>510)
			//Moving right
			SendByte('R');
		else if(x<490)
			//Moving left
			SendByte('L');
		else if(y>510)
			//Moving forwards
			SendByte('F');
		else if(y<490)
			//Moving backwards
			SendByte('B');
		else
			//y is not moving
			SendByte('S');
	
	_delay_ms(5);
	}
}
