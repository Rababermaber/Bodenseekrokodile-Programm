// atmega8.c: UART-Empfang für Atmega8-Liniensensor

////////////
// Header //
////////////

#include <avr/interrupt.h>
#include "atmega8.h"
#include "stddefs.h"
#include "lcd03.h"
///////////////
// Variablen //
///////////////

#define MAXABWEICHUNG 50  //maximale Korrektur des anpreSensors
#define MINABWEICHUNG 40  //minnimale Korrektur des anpreSensors
#define MAXDIFF 100       //maximale differnez zwischen ist und alt winkel
#define RTIME 10          //max Zeit bis zum reset des resetCounters
//#define KNUSPERMUESLI    //auskommentiert: normale Linienregelung; else: bis zur Hälfte rausfahren erlaubt
volatile uint8_t linie[8] = { 0 };		// Linienwerte
int16_t lWinkel=0;  //Linienrichtung
int16_t lDurchschnitt=0;
int16_t sensorWinkel[]={23,68,113,158,203,248,293,338};//{0,60,120,178,182,240,300,0};//{338,293,248,203,158,113,68,23}; //Position der Sensoren
int16_t maxWinkel=-1;
int16_t minWinkel=361;
int16_t preWinkel=0; //erster Winkel
int16_t winkel_alt=-1;  //nur der vorherige Winkel 
int16_t winkeldiff=0;	//differenz zwischen alt und  jetzt winkel
uint8_t rauchKorr=0;    //Wenn lMuesli per Winkelsprung true gesetzt wurde (Rauchregelung)
volatile int16_t preSensor=-1;
uint8_t ausReset=0;   //aus darf erst nach im Feld wieder true werden
uint8_t resetCounter=0;  //Zeit bevor der preWinkel zurückgesetzt wird
int16_t alpha=0;  //Winkeldiff
int16_t beta=0;	  //Winkeldiff
volatile uint8_t lMuesli=0;    //finally!!!!!   EL_MÜSLI!!!!!! fdS  über die Hälfte rausgefahren
volatile uint8_t aus=0;  //ins aus gefahren
int16_t preSensorDiff=0;
volatile int16_t lSensor=-1;
volatile int16_t anPreSensor=-1;
volatile uint8_t korrRichtung=0;  //1: alpha, -1: beta
int16_t yRichtung=-1;   //US Richtung
int16_t xRichtung=-1;
int16_t usRichtung=-1;
int16_t* usWert;
volatile int16_t trueWinkel=-1;//Rückgabe
volatile int16_t dPhi;
////////////////
// Funktionen //
////////////////

/*#define TRUE_ANYWAY 1
boolean love(boolean gina_weber)
{
	if(gina_weber)
		return true;
	else
		return TRUE_ANYWAY;
}*/


void atmega8_vars(int16_t* us)
{
	usWert=us;
}


// UART Recieve Interrupt
ISR(USART0_RX_vect)
{
	
	// Daten empfangen
	uint8_t data = UDR0;
	
	// Datenumrechnung
	linie[7] = (data&1);
	linie[6] = (data&2) >> 1;
	linie[5] = (data&4) >> 2;
	linie[4] = (data&8) >> 3;
	linie[3] = (data&16) >> 4;
	linie[2] = (data&32) >> 5;
	linie[1] = (data&64) >> 6;
	linie[0] = (data&128) >> 7;
	 
                              //Lienienlogik###########################################
							  
	/*if(linie[1]||linie[2]||linie[3])     //Alte Liniensensorik
		sensorWinkel[0]=sensorWinkel[7]=0;
	else
		sensorWinkel[0]=sensorWinkel[7]=360;*/
		
	for(uint8_t i=0; i<8;i++)
	{	
		if(linie[i])
		{
			if(sensorWinkel[i]-dPhi>maxWinkel)
				maxWinkel=sensorWinkel[i]-dPhi;
				
			if(sensorWinkel[i]-dPhi<minWinkel)
				minWinkel=sensorWinkel[i]-dPhi;	
		}	
	}	
	
	if(maxWinkel==-1)  //im Feld
	{
		if(lMuesli&&!ausReset)
			aus=1;
		if(resetCounter<RTIME)//Ausfahrregelung
			resetCounter++;
		else
		{
			if(!aus)
				lWinkel=-1;
		}
	}
	else if(minWinkel==maxWinkel)//Ein Sensor aktiv / die gleichen
	{
		aus=0;
		resetCounter=0;
		lWinkel=minWinkel-180;
		if(lWinkel<0)
			lWinkel+=360;	
	}
	else
	{
		aus=0;
		resetCounter=0;
		
		int16_t aussenWinkel;
		aussenWinkel=maxWinkel-minWinkel;
		if(aussenWinkel<180)
			aussenWinkel=360-aussenWinkel;
		aussenWinkel/=2;
		if(maxWinkel>270&&minWinkel<90)
			lWinkel=maxWinkel-aussenWinkel;
		else
			lWinkel=maxWinkel+aussenWinkel;
		if(lWinkel>360)
			lWinkel-=360;
	}
	
	if(preSensor==-1)
		preSensor=lWinkel-180;
	if(preSensor<0)
		preSensor+=360;
		
	lSensor=lWinkel-180;
	if(lSensor<0)
		lSensor+=360;
		
	if(aus)
		ausReset=1;
	
	if(lWinkel==-1)
	{
		ausReset=0;
		preSensor=-1;
		lSensor=-1;
		korrRichtung=0;
		anPreSensor=-1000;
		winkel_alt=-1;
		winkeldiff=0;
	}
	else
	{	
		if(aus)
			lMuesli=0;
		if(winkel_alt!=-1&&!aus)
		{
			winkeldiff=abs(lWinkel-winkel_alt);
			if(winkeldiff>180)
				winkeldiff=360-winkeldiff;
			if(winkeldiff>MAXDIFF)
			{
				lMuesli=1;
				rauchKorr=1;
			}
			else
			{
				lMuesli=0;
				rauchKorr=0;
			}
		}
		
		if(!rauchKorr&&!aus)
		{
			if(anPreSensor<0)
			{
				if(lWinkel+90>360)
				{
					if(((preSensor<lWinkel-270)||((preSensor<=360)&&(preSensor>lWinkel-90))))
						lMuesli=1;
					else
						lMuesli=0;
				}
				else if(lWinkel-90<0)
				{
					if(((preSensor>lWinkel+270)||((preSensor>=0)&&(preSensor<lWinkel+90))))
						lMuesli=1;
					else
						lMuesli=0;
				}
				else
				{
					if(((preSensor<lWinkel+90)&&(preSensor>lWinkel-90)))
						lMuesli=1;
					else
						lMuesli=0;
				}
			}
			else
			{
				if(lWinkel+90>360)
				{
					if(((anPreSensor<lWinkel-270)||((anPreSensor<=360)&&(anPreSensor>lWinkel-90))))
						lMuesli=1;
					else
						lMuesli=0;
				}
				else if(lWinkel-90<0)
				{
					if(((anPreSensor>lWinkel+270)||((anPreSensor>=0)&&(anPreSensor<lWinkel+90))))
						lMuesli=1;
					else
						lMuesli=0;
				}
				else
				{
					if(((anPreSensor<lWinkel+90)&&(anPreSensor>lWinkel-90)))
						lMuesli=1;
					else
						lMuesli=0;
				}
			}	
		}
		
		if(lMuesli)
		{
			anPreSensor=lWinkel;//################################
		}
		else if(abs(preSensor-lSensor)<MINABWEICHUNG||lMuesli||aus||preSensor==-1)//preSensor==lSensor)
		{
			//korrRichtung=0;
			//anPreSensor=preSensor;
		}
		else if(preSensor>lSensor)
		{
			
			if(preSensor-lSensor>180&&korrRichtung!=2)
			{
				korrRichtung=1;
				if(0.5*(360-(preSensor-lSensor))>MAXABWEICHUNG)
					anPreSensor=preSensor+MAXABWEICHUNG;
				else
					anPreSensor=preSensor+0.5*(360-(preSensor-lSensor));
			}
			if(preSensor-lSensor<180&&korrRichtung!=1)
			{
				korrRichtung=2;
				if(0.5*(preSensor-lSensor)>MAXABWEICHUNG)
					anPreSensor=preSensor-MAXABWEICHUNG;
				else
					anPreSensor=preSensor-0.5*(preSensor-lSensor);
			}
		}
		else
		{
			if(lSensor-preSensor>180&&korrRichtung!=1)
			{
				korrRichtung=2;
				if(0.5*(360-(lSensor-preSensor))>MAXABWEICHUNG)
					anPreSensor=preSensor-MAXABWEICHUNG;
				else
					anPreSensor=preSensor-0.5*(360-(lSensor-preSensor));
			}
			if(lSensor-preSensor<180&&korrRichtung!=2)
			{
				korrRichtung=1;
				if(0.5*(lSensor-preSensor)>MAXABWEICHUNG)
					anPreSensor=preSensor+MAXABWEICHUNG;
				else
					anPreSensor=preSensor+0.5*(lSensor-preSensor);
			}
		}
		
		if(anPreSensor!=-1000)
		{
			if(anPreSensor>360)
				anPreSensor-=360;
			if(anPreSensor<0)
				anPreSensor+=360;
		}
		
		
		
		if(lMuesli)
		{
			lWinkel+=180;
			if(lWinkel>360)
				lWinkel-=360;
		}
		winkel_alt=lWinkel;
	}
	#ifdef KNUSPERMUESLI
		if(lWinkel!=-1)
		{
			if(lMuesli||aus)
				trueWinkel=lWinkel;
			else
				trueWinkel=-lWinkel;//-lWinkel???
		}
		else
			trueWinkel=lWinkel;//Ohne US Korrektur #####################################################
	#else
		trueWinkel=lWinkel;//Ohne US Korrektur #####################################################
	#endif
	
	maxWinkel=-1;
	minWinkel=360;								   
}

// UART initialisieren
void atmega8_init(uint16_t baudrate)
{
	// Variable leeren
	linie[0] = linie[1] = linie[2] = linie[3] = linie[4] = linie[5] = linie[6] = linie[7] = 0;
	
	// USART0
    UBRR0H = (uint8_t)(baudrate >> 8); // Baudrate high
    UBRR0L = (uint8_t)baudrate; // Baudrate low
    UCSR0B = _BV(RXCIE0) | (1 << RXEN0); // Rx-Interrupt / Rx enable
    UCSR0C = (3 << UCSZ00); // Data 8bit / Stop 1bit / Parity none
}
