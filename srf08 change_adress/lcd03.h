/*
 * i2c_display.h
 *
 * Created: 19.05.2013 22:31:38
 *  Author: Hannes
 */ 

#ifndef I2C_DISPLAY_H
#define I2C_DISPLAY_H

#include <stdint.h>
#include "twimaster.h"	  

#define I2C_DISPLAY 0xC6

#define LCD_KEY_1 0
#define LCD_KEY_2 1
#define LCD_KEY_3 2
#define LCD_KEY_4 3
#define LCD_KEY_5 4
#define LCD_KEY_6 5
#define LCD_KEY_7 6
#define LCD_KEY_8 7
#define LCD_KEY_9 8
#define LCD_KEY_ST 9
#define LCD_KEY_0 10
#define LCD_KEY_RT 11


enum E_CURSOR{
	LCD_CURSOR_OFF = 0,        /*   DB2: turn display on              */
	LCD_CURSOR_UNDERLINE = 2,  /*   DB1: turn cursor UNDERLINE        */
	LCD_CURSOR_BLINK = 1       /*   DB0: blinking cursor ?            */
};

#define LCD_ON 1
#define LCD_OFF 0

//#define I2C_RNKEYLCD 0x40    // device address key-LCD

void display_output_string (int zeile, int spalte, char text []);

void display_output_num (int zeile, int spalte, int zahl, int num);

void display_output_int (int zeile, int spalte, int zahl);

/************************************************************************/
/*************     RN-KEYLCD LCD-DISPLAY FUNKTIONEN    ******************/
/************************************************************************/
  
/*Lösche LCD-Display*/
void clear_lcd(void);

/*Cursor auf bestimmte Position setzen*/
void cursor_x_y(int zeile, int spalte);

/*Cursor sichtbar oder unsichtbar machen
LCD_CURSOR_OFF , LCD_CURSOR_UNDERLINE , LCD_CURSOR_BLINK*/
void cursor_sichtbar(enum E_CURSOR x);

/*LCD-Beleuchtung an- oder ausschalten
LCD_ON , LCD_OFF*/
void lcd_light(int x);

/*einzelnes Zeichen an das LCD senden*/
void write_char(unsigned char c);

/*eigenes Zeichen an das LCD senden
num: Zeichennummer 128-135
data: Array mit 8 einträgen nach der Form 0b1xxDDDDD*/
void lcd_my_char(char num, char data [8]);

/*Zeichen-Kette an das LCD senden*/
void lprint(const char *s);

/************************************************************************/
/*************     RN-KEYLCD LCD-DISPLAY FUNKTIONEN ENDE.   **************/
/**********************************************************************/

/************************************************************************/
/*************      RN-KEYLCD TASTATUR FUNKTIONEN      ******************/
/************************************************************************/

/*Gedrückte Tasten einlesen*/
uint16_t read_key(void);


/*Lese Tasten ein und stelle sie am Display dar*/
void lcd_test_key(void);

/************************************************************************/
/*************      RN-KEYLCD TASTATUR FUNKTIONEN  ENDE.    *************/
/************************************************************************/

#endif
