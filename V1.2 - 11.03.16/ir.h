// ir.h: Funktionen zum Auslesen der Infrarotsensoren
// EIN MAL IN main.c EINBINDEN! SONST VARIABLEN-KONFILKT!

//////////////////
// Definitionen //
//////////////////

// Verzögerung im Interrupt (us)
//#define IR_DELAY_ENABLED
#define IR_DELAY 5

// Anzahl der IR-Sensoren
#define MAX_IR 8

// Tiefpass (VAL+1 % 2^n = 0)
#define IR_TIEFPASS_ENABLE
#define _TIEFPASS_IR 63

///////////////
// Variablen //
///////////////

uint8_t muxIR = 0;						// ADC-Kanal
uint16_t ADC_Werte[MAX_IR] = { 0 };	// ADC-Messwerte
uint16_t ADC_Werte_TP[MAX_IR] = { 0 };	// ADC-Messwerte mit Tiefpass
int16_t ADC_Offset[MAX_IR] = { 0 };	// ADC-Korrektur

////////////////
// Funktionen //
////////////////

// ADC Interrupt initialisieren
void ReadChannelInit(uint8_t ADCINTERRUPT)
{
	/* (1 << REFS0) -> Referenzspannung 5,0V */
	/* (1 << REFS1) -> Referenzspannung 1,2V */
	/* (1 << REFS0) | (1 << REFS1) -> Referenzspannung 2,56V */
	ADMUX |= (1 << REFS0); // interne Referenzspannung nutzen
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	if(ADCINTERRUPT) { // wenn ADC Interrupts aktiviert sind  
		ADCSRA |= (1 << ADIE) | (1 << ADSC); // ADIE = Interrupt enable; ADSC = ADC auslesen -> Damit startet der Selbsterhaltungsprozess  
	}
	else {
		ADCSRA &=~ (1 << ADIE) | (1 << ADSC);
	}
}

// ADC-Interrupt
ISR(ADC_vect)
{
	// wird nach jeder ADC Messung ausgeführt
	
	#ifdef IR_DELAY_ENABLED
		// eventuell DELAY
		// ACHTUNG!!! FRISST SEHR VIEL ZEIT!!!
		// INTERRUPT WIRD ALLE 83us AUFGERUFEN
		_delay_us(IR_DELAY);
	#endif
	
	// letzten Messwert in Array eintragen
	ADC_Werte[muxIR] = ADCW-ADC_Offset[muxIR];
	
	// Tiefpass
	#ifdef IR_TIEFPASS_ENABLE
		ADC_Werte_TP[muxIR] = (ADC_Werte_TP[muxIR]*_TIEFPASS_IR+ADC_Werte[muxIR])/(_TIEFPASS_IR+1);
	#endif
	
	// Nächste Messung
	if(++muxIR >= MAX_IR)
		muxIR = 0;
	ADMUX = muxIR | (1 << REFS0); // nächsten Kanal setzen
	ADCSRA |= (1 << ADSC); // neue Messung starten
}
