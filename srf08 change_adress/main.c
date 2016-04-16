#include "main.h"








/// Programm zu �ndern der Adresse eines Ultrascahllsensors (srf08)
/// alte Adresse:
#define adress_old 0xE6
/// neue Adresse:
#define adress_new 0xE0



















ISR(TIMER1_OVF_vect) //Interrupt
{
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
	////Hardware PWM f�r Motoren 20000000 Hz / 256 / 256 (8bit Timer) = 304 Hz///
	TCCR0A = (1<<COM0A1)|(1<<COM0B1)|(1<<WGM01)|(1<<WGM00);  // Timer 0 f�r PWM bei A und B
	TCCR0B = (0 << CS02)|(1 << CS01)|(1 << CS00);           // Vorteiler / prescaler 1024 f�r Timer 0 (S. 100)############## PWM Frequenz �ndern
	TCCR2A = (1<<COM2A1)|(1<<COM2B1)|(1<<WGM21)|(1<<WGM20);  // Timer 2 f�r PWM bei A und B
	TCCR2B = (1 << CS22)|(0 << CS21)|(0 << CS20);            // Vorteiler / prescaler 1024 f�r Timer 2 (S. 149)############## 
	
	
	
	
	TIMSK1 = (1<<TOIE1);									// Overflow Interrupt 312.5 1/s
	TCCR1B = 1;
	ReadChannelInit(1);
	
	if(PINSET(MCUSR, WDRF))
	   SETLED(P_LEDW);
	MCUSR &= ~(1 << WDRF);

	sei();//Interrupt starten
	bool t = true;
	int16_t z = 0;
	
	while(true)									// main Schleife	
	{
	  if(t) {
		SETLED(P_LEDR);
		t=false; z++;
		_delay_ms(20);
	  } else {
		CLEARLED(P_LEDR);
		t=true;
		_delay_ms(20);
	  } 
	  if(z==20) {
	   change_i2c_address_srf08(adress_old,adress_new);
	   _delay_ms(50);
	  }
	  if(z==24) {
	    while(true) {
			if(t) {
				SETLED(P_LEDR);
				SETLED(P_LEDW);
				SETLED(P_LEDG);
				_delay_ms(1000);
				t=false;
			} else {
				CLEARLED(P_LEDR);
				CLEARLED(P_LEDW);
				CLEARLED(P_LEDG);
				_delay_ms(1000);
				t=true;
			}
			wdt_reset();
		}	
	  }
	 wdt_reset();	
	}
}
