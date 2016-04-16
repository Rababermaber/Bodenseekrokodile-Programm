#include "main.h"

int16_t compass=0;
int8_t zaehler=1;
uint16_t wert=0;
int16_t us=0;
char text;





ISR(TIMER1_OVF_vect) //Interrupt
{
 if (zaehler == 1) {
 clear_lcd();
 compass = CMPS_grad();	
 Gyro_byte ();
 display_output_int (1,1,compass);
 display_output_int (2,1,yaw);
 display_output_int (2,6,pitch);
 display_output_int (2,12,roll);	
 display_output_int (3,1,ADC_Werte[7]);
 }
 
 if (zaehler == 2) {
  if (srf08_ready(0xE0)) {
  us = US_read(0xE0);
  US_start(0xE0);
  }
  display_output_int(4,1,us);
 }
 
 if (zaehler == 20) {
 zaehler = 1;
 } else {
 zaehler++;
 }
}

int main (void)
{
	wdt_reset();
	wdt_enable(WDTO_2S);    //Watchdog an
	DDRD = 0xFF;     		//Pullups, Port Initialisierung  
	DDRD &= ~ (1<<PD3);
	DDRD &= ~ (1<<PD2);
	DDRB = 0xFF;
	PORTC |= (1 << PC2);
	PORTC |= (1 << PC3);
	PORTC |= (1 << PC5);
	PORTC |= (1 << PC6);
	PORTC |= (1 << PC7);
	PORTD |= (1<<PD3);
	PORTD |= (1<<PD2);
	i2c_init();
	////Hardware PWM für Motoren 20000000 Hz / 256 / 256 (8bit Timer) = 304 Hz///
	TCCR0A = (1<<COM0A1)|(1<<COM0B1)|(1<<WGM01)|(1<<WGM00);  // Timer 0 für PWM bei A und B
	TCCR0B = (0 << CS02)|(1 << CS01)|(1 << CS00);           // Vorteiler / prescaler 1024 für Timer 0 (S. 100)############## PWM Frequenz ändern
	TCCR2A = (1<<COM2A1)|(1<<COM2B1)|(1<<WGM21)|(1<<WGM20);  // Timer 2 für PWM bei A und B
	TCCR2B = (1 << CS22)|(0 << CS21)|(0 << CS20);            // Vorteiler / prescaler 1024 für Timer 2 (S. 149)############## 
	
	
	
	
	TIMSK1 = (1<<TOIE1);									// Overflow Interrupt 312.5 1/s
	TCCR1B = 1;
	ReadChannelInit(1);
	
	if(PINSET(MCUSR, WDRF))
	   SETLED(P_LEDW);
	MCUSR &= ~(1 << WDRF);

	gyro_init();
	sei();//Interrupt starten

	
	while(true)									// main Schleife
	{
		SETLED(P_LEDR);
		SETLED(P_LEDW);
		
		wdt_reset();
		
		
	}
}
