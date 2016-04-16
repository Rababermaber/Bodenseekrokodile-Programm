/*
 * i2c_display.c
 *
 * Created: 19.05.2013 22:31:38
 *  Author: Hannes
 */ 

#include "lcd03.h"

#include <stdlib.h>
#include <string.h>

void display_output_string (int zeile, int spalte, char text []){
	//unsigned char ret;   
	//ret = ;  // set device address and write mode
	if (!i2c_start(I2C_DISPLAY+I2C_WRITE)){ /* issuing start condition ok, device accessible */      
		uint32_t zahl; 
		i2c_write(0);                       // Befehl 27,79,x,y um Cursor an Position x,y zu fahren.
		i2c_write(3);
		i2c_write(zeile);
  		i2c_write(spalte);
		for (zahl=0; text[zahl]!=0; ++zahl){
			i2c_write(text[zahl]);	   
		}
		i2c_stop();                            // set stop conditon = release bus	                           
	}
}

void display_output_int (int zeile, int spalte, int zahl){
	char text [6];
    itoa(zahl,text,10);   //Letztes Argument gibt an, dass Konversion ins 10er-System gewünscht.
	display_output_string (zeile, spalte, text);
}

void display_output_num (int zeile, int spalte, int zahl, int num){
	char text [20];
    itoa(zahl,text,num);  
	display_output_string (zeile, spalte, text);
}

/************************************************************************/
/*************     RN-KEYLCD LCD-DISPLAY FUNKTIONEN    ******************/
/************************************************************************/
  
/*Lösche LCD-Display*/
void clear_lcd(void){
	if (!i2c_start(I2C_DISPLAY+I2C_WRITE)){
		i2c_write(0);                       // Befehl 0,1 für Home address
		i2c_write(12);
		i2c_stop();
	}
}

/*Cursor auf bestimmte Position setzen*/
void cursor_x_y(int zeile, int spalte){
    if (!i2c_start(I2C_DISPLAY+I2C_WRITE)){
		i2c_write(0);                       // Befehl 0,1 für Home address
		i2c_write(3);
		i2c_write(zeile);
		i2c_write(spalte);		
		i2c_stop(); 
	}
}

/*Cursor sichtbar oder unsichtbar machen*/
void cursor_sichtbar(enum E_CURSOR x){
    if (!i2c_start(I2C_DISPLAY+I2C_WRITE)){
		i2c_write(0);                       // Befehl 0,1 für Home address
		if(!x)i2c_write(4);
		else if(x==2)i2c_write(5);
		else i2c_write(6);
		i2c_stop();
	}
}

/*LCD-Beleuchtung an- oder ausschalten*/
void lcd_light(int x){
    if (!i2c_start(I2C_DISPLAY+I2C_WRITE)){
		i2c_write(0);                       // Befehl 0,1 für Home address
		if(x)i2c_write(19);
		else i2c_write(20);
		i2c_stop();  
	}
}

/*einzelnes Zeichen an das LCD senden*/
void write_char(unsigned char c){
	if (!i2c_start(I2C_DISPLAY+I2C_WRITE));     // set device address and write mode
    i2c_write(0);                            //Register 0
    i2c_write(c);
    i2c_stop();								// set stop condition = release bus 
}


void lcd_my_char(char num, char data [8]){
	if (!i2c_start(I2C_DISPLAY+I2C_WRITE)){
		i2c_write(0);
		i2c_write(27);
		i2c_write(num);
		for (int i=0; i<8;i++)
		i2c_write(data[i]);
		i2c_stop();
	}
}

/*Zeichen-Kette an das LCD senden*/
void lprint(const char *s){
	if (!i2c_start(I2C_DISPLAY+I2C_WRITE)){
		i2c_write(0);                            //Register 0
		for(int i=0; s[i]!='\0'; i++){
			i2c_write(s[i]);
		}
		i2c_stop();
	}
}

/************************************************************************/
/*************     RN-KEYLCD LCD-DISPLAY FUNKTIONEN ENDE.   **************/
/**********************************************************************/

/************************************************************************/
/*************      RN-KEYLCD TASTATUR FUNKTIONEN      ******************/
/************************************************************************/

/*einzelnes Zeichen der Tastatur lesen (Ascii-Modus)*/
uint16_t read_key(void){
	unsigned char low,high;
	if (!i2c_start(I2C_DISPLAY+I2C_WRITE)){
		i2c_write(1);
		i2c_start(I2C_DISPLAY+I2C_READ);
		low=i2c_readAck();
		high=i2c_readNak();
		i2c_stop();
		return (high*256)+low;
	}
	return 0xFFFF;
}

void lcd_test_key(void){
	display_output_string(1,1,"123456789*0#");
	cursor_x_y(2, 1);
	for (uint8_t i=0; i<16;i++) write_char(((read_key()&(1<<i))>>i)+'0');
}
/************************************************************************/
/*************      RN-KEYLCD TASTATUR FUNKTIONEN  ENDE.    *************/
/************************************************************************/
