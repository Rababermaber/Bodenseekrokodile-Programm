// stddefs.h: Microcontroller-Spezifische Definitionen

#include <avr/io.h>
#include <math.h>
#include <stdlib.h>

/////////////
// MOTOREN //
/////////////

// PWM-Ports
#define OCR_M0 OCR0A//OCR0B
#define OCR_M1 OCR0B//OCR0A
#define OCR_M2 OCR2A
#define OCR_M3 OCR2B
// Richtungsports
#define PM3 PD5
#define PM2 PD4
#define PM1 PB2
#define PM0 PB1
#define PORT_M3 PORTD
#define PORT_M2 PORTD
#define PORT_M1 PORTB
#define PORT_M0 PORTB
// Motorpower-Ports
#define POWER_M3 PD6
#define POWER_M2 PD7
#define POWER_M1 PB3
#define POWER_M0 PB4
#define PPORT_M3 PORTD
#define PPORT_M2 PORTD
#define PPORT_M1 PORTB
#define PPORT_M0 PORTB

//////////////
// DRIBBLER //
//////////////

#define PORT_DRB PORTB
#define DRB_ON PB0

////////////
// SCHUSS //
////////////

#define PORT_SCHUSS PORTC
#define P_SCHUSS PC4
#ifdef _SOLENOID
	#define KICK_DAUER 50
#else
	#define KICK_DAUER 150
#endif

//////////////
// Schalter //
//////////////

#define PIN_MOTORTASTER PINC
#define P_MOTORTASTER PC2
#define MOTORTASTER (PIN_MOTORTASTER & (1 << P_MOTORTASTER))
#define PIN_EEPROMTASTER PINC
#define P_EEPROMTASTER PC6
#define EEPROMTASTER (PIN_EEPROMTASTER & (1 << P_EEPROMTASTER))
#define PIN_BAD PINC
#define P_BAD PC3
#define DRIBBLER_SHUNT (PIN_BAD & (1 << P_BAD))

#define SW_LINKS (PINC & (1 << PC7))
#define SW_RECHTS (PINC & (1 << PC5))

//////////
// LEDs //
//////////

#define PORT_LED PORTB
#define P_LEDR 6
#define P_LEDG 5
#define P_LEDW 7
#define LEDR (1 << P_LEDR)
#define LEDG (1 << P_LEDG)
#define LEDW (1 << P_LEDW)

#define PIN_MOT_AN PINC
#define P_MOT_AN 2

////////
// US //
////////

//////////
// CMPS //
//////////

#define cmps03 0xC0	// Adresse des Kompass

//////////
// BNO ///
//////////
#define YAW 0x1A
#define PITCH 0x1E
#define ROLL 0x1C

#define GYRO_X 0x14
#define GYRO_Y 0x16
#define GYRO_Z 0x18

#define ACC_X 0x28
#define ACC_Y 0x2A
#define ACC_Z 0x2C

#define BNO_SELFTEST 0x36
#define BNO_SYSTEMERROR 0x3A
#define BNO_SYSTEMSTATUS 0x39
#define BNO_OPMODE 0x3D


/////////////////
// PORT-MAKROS //
/////////////////

#define SETPORT(port, pin) (port |= (1 << pin))
#define CLEARPORT(port, pin) (port &= ~(1 << pin))
#define SETLED(led) (PORT_LED |= (1 << led))
#define CLEARLED(led) (PORT_LED &= ~(1 << led))
#define LEDON(led) (PORT_LED |= led)
#define LEDOF(led) (PORT_LED &= ~led)
#define PINSET(port, pin) ((port&(1 << pin)))
#define MOT_AN (!PINSET(PIN_MOT_AN, P_MOT_AN))
