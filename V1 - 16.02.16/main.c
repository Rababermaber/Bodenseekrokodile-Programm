#include "main.h"

int8_t zaehler = 1;
int16_t ir_zaehler = 0;
uint16_t wert = 0;
int16_t us_l = 0;
int16_t us_r = 0;
int16_t us_a = 0;
int16_t cmp = 0;
int16_t cmp_rel = 0;
int16_t cmp_start;
uint16_t ir_1 = 0;
uint16_t ir_2 = 0;
uint16_t ir_3 = 0;
uint16_t ir_4 = 0;
uint16_t ir_5 = 0;
uint16_t ir_6 = 0;
uint16_t ir_7 = 0;
uint16_t ir_8 = 0;
char text;





ISR(TIMER1_OVF_vect) //Interrupt
{
 if (zaehler == 1) {
  //++- ULTRASCHALL -++//
	if (srf08_ready(0xE0)) {	
		us_a = US_read(0xE0);
		US_start(0xE0);
	}
	if (srf08_ready(0xE2)) {
		us_l = US_read(0xE2);
		US_start(0xE2);
	}
	if (srf08_ready(0xE6)) {
		us_r = US_read(0xE6);
		US_start(0xE6);
	}
 }
 
 if (zaehler == 2) {
 //++- GYROSENSOR -++//
	Gyro_byte ();
 //++- KOMPASS -++//
	cmp = CMPS_grad();
	cmp_rel = relativeRichtung(cmp_start);
 //++- INFRAROT -++//
	ir_1 = ADC_Werte[0];
	ir_2 = ADC_Werte[1];
	ir_3 = ADC_Werte[2];
	ir_4 = ADC_Werte[3];
	ir_5 = ADC_Werte[4];
	ir_6 = ADC_Werte[5];
	ir_7 = ADC_Werte[6];
	ir_8 = ADC_Werte[7];
 }
 if (zaehler == 3) {
	clear_lcd();
	display_output_string(1,1,"US|l/r/a");
	display_output_int(1,10,us_l);
	display_output_int(1,14,us_r);
	display_output_int(1,18,us_a);
	display_output_string(2,1,"GYRO");
	display_output_int(2,6,yaw);
	display_output_int(2,10,pitch);
	display_output_int(2,14,roll);
	display_output_string(3,1,"CMP");
	display_output_int(3,5,cmp); 
	display_output_int(3,10,cmp_rel);
	if (ir_zaehler < 100) {
		display_output_string(4,1,"A");
		display_output_int(4,2,ir_1);
		display_output_string(4,6,"B");
		display_output_int(4,7,ir_2);
		display_output_string(4,11,"C");
		display_output_int(4,12,ir_3);
		display_output_string(4,16,"D");
		display_output_int(4,17,ir_4);
		ir_zaehler ++;
	} else if (ir_zaehler < 200) {
		display_output_string(4,1,"E");
		display_output_int(4,2,ir_5);
		display_output_string(4,6,"F");
		display_output_int(4,7,ir_6);
		display_output_string(4,11,"G");
		display_output_int(4,12,ir_7);
		display_output_string(4,16,"H");
		display_output_int(4,17,ir_8);
		ir_zaehler ++;
	} else {
		ir_zaehler = 0;
	}
 }
 
 if (zaehler == 3) {
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
	cmp_start = CMPS_grad();
	sei();//Interrupt starten

	
	while(true)									// main Schleife
	{
		SETLED(P_LEDR);
		SETLED(P_LEDW);
		
		MotorPower(0,255);
		MotorPower(1,255);
		MotorPower(2,255);
		
		
		
		
		
		wdt_reset();
		
		
	}
}
