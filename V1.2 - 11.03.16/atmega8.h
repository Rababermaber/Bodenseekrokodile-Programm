// atmega8.h: UART-Empfang für Atmega8-Liniensensor
// basierend auf Peter Fleury's UART Bibliothek
// Umgeschrieben und vereinfacht zum reinen Empfang von Liniendaten

#ifndef _ATMEGA8_LINIE
#define _ATMEGA8_LINIE

//////////////////
// Definitionen //
//////////////////

// Baudrate auswählen (braucht CPU-Frequenz)
#define UART_BAUD_SELECT(baudRate, xtalCpu) ((xtalCpu) / ((baudRate) * 16l) - 1)

///////////////
// Variablen //
///////////////

// Liniensensoren
extern volatile uint8_t linie[8];
extern volatile int16_t trueWinkel;
extern volatile int16_t preSensor;
extern volatile uint8_t lMuesli;
extern volatile uint8_t aus;
extern volatile int16_t anPreSensor;
//extern volatile int8_t altGesX;
//extern volatile int8_t altGesY;
//extern volatile int8_t gesX;
//extern volatile int8_t gesY;
extern volatile int16_t dPhi;       
extern volatile int16_t lSensor;
extern volatile uint8_t korrRichtung;
////////////////
// Funktionen //
////////////////

// Kommunikation mit Atmega8 initialisieren
extern void atmega8_init(uint16_t baudrate);

extern void atmega8_vars(int16_t* us);


#endif
