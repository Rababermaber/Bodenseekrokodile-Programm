w#include "main.h"

int8_t zaehler = 1; 
int16_t ir_zaehler = 0;
volatile int16_t us_l = 0;
volatile int16_t us_r = 0;
volatile int16_t us_a = 0;
volatile int16_t cmp = 0;
volatile int16_t cmp_rel = 0;
volatile int16_t time = 0;
int8_t shoots;
int16_t startwert;
bool set_time = false;
bool ball = true;
bool lcd_clear = true;





ISR(TIMER1_OVF_vect) //Interrupt
{
 if(set_time) {
	time ++;		//time +1000 = 7Sekunden  // 1 time = 0,007s = 7ms // zeit x(in s) = (1000 / 7 * x) -> 1s = 142time 
 }	
 
 if(zaehler == 1) {
  //++- ULTRASCHALL -++//
	if(srf08_ready(0xE0)) {	
		us_a = US_read(0xE0);
		US_start(0xE0);
	}
	if(srf08_ready(0xE2)) {
		us_l = US_read(0xE2);
		US_start(0xE2);
	}
	if(srf08_ready(0xE6)) {
		us_r = US_read(0xE6);
		US_start(0xE6);
	}
 }
 
 if(zaehler == 2) {
 //++- GYROSENSOR -++//
	Gyro_byte ();
 //++- KOMPASS -++//
	cmp = CMPS_grad();
	//cmp_rel = relativeRichtung(startwert);
	
	getShootangle(us_l,us_r,us_a);
 }
 if((zaehler == 3) && (!PINSET(PINC,PC6))) {
	if(lcd_clear) {
		clear_lcd();
		lcd_clear = false;
	} else {
		lcd_clear = true;
	}
	display_output_string(1,1,"US|l/r/a");
	display_output_int(1,10,us_l);
	display_output_int(1,14,us_r);
	display_output_int(1,18,us_a);
	display_output_string(3,1,"CMP");
	display_output_int(3,5,cmp); 
	display_output_int(3,10,cmp_rel);
	display_output_int(3,15,getIR_max());
	if(ir_zaehler < 100) {
		display_output_string(4,1,"A");
		display_output_int(4,2,ADC_Werte[0]);
		display_output_string(4,6,"B");
		display_output_int(4,7,ADC_Werte[1]);
		display_output_string(4,11,"C");
		display_output_int(4,12,ADC_Werte[2]);
		display_output_string(4,16,"D");
		display_output_int(4,17,ADC_Werte[3]);
		display_output_string(2,1,"GYRO");
		display_output_int(2,6,yaw);
		display_output_int(2,10,pitch);
		display_output_int(2,14,roll);
		ir_zaehler ++;
	} else if(ir_zaehler < 200) {
		display_output_string(4,1,"E");
		display_output_int(4,2,ADC_Werte[4]);
		display_output_string(4,6,"F");
		display_output_int(4,7,ADC_Werte[5]);
		display_output_string(4,11,"G");
		display_output_int(4,12,ADC_Werte[6]);
		display_output_string(4,16,"H");
		display_output_int(4,17,ADC_Werte[7]);
		display_output_string(2,1,"X/Y");
		display_output_int(2,6,x);
		display_output_int(2,10,y);
		display_output_int(2,14,disp);
		
		//display_output_string(2,1,"Zeit");
		//display_output_int(2,7,time);
		ir_zaehler ++;
	} else {
		ir_zaehler = 0;
	}
 }
 
 if(zaehler == 3) {
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
	PORTD |= (1 << PD3);
	PORTD |= (1 << PD2);
	i2c_init();
	////Hardware PWM für Motoren 20000000 Hz / 256 / 256 (8bit Timer) = 304 Hz///
	TCCR0A = (1<<COM0A1)|(1<<COM0B1)|(1<<WGM01)|(1<<WGM00);  // Timer 0 für PWM bei A und B
	TCCR0B = (0 << CS02)|(1 << CS01)|(1 << CS00);           // Vorteiler / prescaler 1024 für Timer 0 (S. 100)############## PWM Frequenz ändern
	TCCR2A = (1<<COM2A1)|(1<<COM2B1)|(1<<WGM21)|(1<<WGM20);  // Timer 2 für PWM bei A und B
	TCCR2B = (1 << CS22)|(0 << CS21)|(0 << CS20);            // Vorteiler / prescaler 1024 für Timer 2 (S. 149)############## 
	
	
	
	
	TIMSK1 = (1<<TOIE1);									// Overflow Interrupt 312.5 1/s
	TCCR1B = 1;
	ReadChannelInit(1);
	
	startwert=eeprom_read_word((void*)14);
	
	if(PINSET(MCUSR, WDRF))
	   SETLED(P_LEDW);
	MCUSR &= ~(1 << WDRF);

	gyro_init();
	sei();//Interrupt starten

	while(true)									// main Schleife
	{
		if(!PINSET(PINC,PC2))//EEPROM-Reset
		{
			SETLED(P_LEDW);
			for(uint8_t i=0;i<50;i++)
			{
				_delay_ms(1);
				wdt_reset();
			}
			eeprom_write_word((void*)1,0);
			eeprom_write_word((void*)14,cmp);
			startwert=cmp;
			CLEARLED(P_LEDW);
		}
		
		if(!PINSET(PINC,PC3)) {
			//SETLED(P_LEDR);
			if(shoots < 3) {
				CMPcorrection = getShootangle(us_l,us_r,us_a);
				if(relativeRichtung(startwert+CMPcorrection) <=5) {
					if(ball) {
						time = 0;
						set_time = true;
						ball = false;
					} 
				}	
				if(time > 100) {
					//SETPORT(PORTB,0);
					//_delay_ms(10);
					//CLEARPORT(PORTB,0);
					shoots ++;
					time = 0;
				}	
			} else {
				CMPcorrection = 0;
			}
		} else {
			//CLEARLED(P_LEDR);
			CMPcorrection = 0;
			if(ball == false) {
				ball = true;
				set_time = false;
				shoots = 0;
			}	
		}
			
		if(getIR_max() > 600) {
			cmp_rel = startwert - cmp;
			if(cmp_rel<-1800)
				cmp_rel+=3600;
			else if(cmp_rel>1800)
				cmp_rel-=3600;
			cmp_rel/=10;		
			//doDriveIR(cmp_rel,roll,90);
		} else {
			MotorPower(0,0);
			MotorPower(1,0);
			MotorPower(2,0);
		}
		
		wdt_reset();
		
		
	}
}
