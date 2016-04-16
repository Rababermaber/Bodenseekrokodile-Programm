// main.h: Headerdatei für main.c

////////////
// Header //
////////////

#include <avr/interrupt.h>	// ISR - Interrupt
#include <avr/eeprom.h>		// Electronically Erasable Presistent Read Only Memory
#include <avr/wdt.h>		// Watchdog / automatischer Reset
#include <util/delay.h>		// Delay Cycles
#include <stdbool.h>		// Microcontroller-Spezifisch
#include "stddefs.h"		// Parameter für Programm
#include "sinus.h"			// Eigene Sinusfunktion
#include "twimaster.h"		// Master i2C Komunikation
#include "lcd03.h"			// Steuerung für LCD-Display
#include "cmps03.h"			// Kompassensor
#include "srf08.h"			// Ultraschallmessung
#include "motor.h"
#include "ir.h"
#include "mpu650.h"         //Gyro
#include "atmega8.h"
#include "bno055.h"
#include "globals.h"
#include "wtdMOVEMENT.h"
//#include "wtdIR.h"
uint8_t EEMEM leer=0;
uint8_t EEMEM test=1;
///////////////
// Variablen //
///////////////
//extern volatile int16_t us[4];
uint8_t in_calibrate = 0;
uint8_t step_calibrate = 0;
