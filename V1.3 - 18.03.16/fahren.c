/*
 * fahren.c
 *
 * Created: 19.02.2016 15:36
 *  Author: Aaron
 */
 
#include "fahren.h"

int16_t P;
int16_t I;
int16_t D;
int16_t PID;
int16_t PID_flaeche;
int16_t relativeTorrichtung_alt;
int8_t i = 0;
int16_t max = 0;
float geschwindigkeit[3];

void fahren(int richtung, int geschwindigkeit_max, int relativeTorrichtung) {
	int16_t P;
	int16_t I;
	int16_t D;
	int16_t PID;
	int16_t PID_flaeche;
	int16_t relativeTorrichtung_alt;
	int8_t i = 0;
	int16_t max = 0;
	float geschwindigkeit[3];

	/******************* FAHREN ***********************/
	 for(i=0;i<=2;i++) {
		geschwindigkeit[i] = ((sinus(richtung) * rechtsmotorfaktoren[i]) + (cosinus(richtung) * vorwaertsmotorfaktoren[i]))	* 255;
	 }
	 
	 /* Unnötig??!!!
	 max = 0;
	 for(i=0;i<=2;i++) {
		if(abs(geschwindigkeit[i]) > max) {
			max = abs(geschwindigkeit[i]);
		} 
	 }
	 if(max > 255) {
		 for(i=0;i<=2;i++) {
			geschwindigkeit[i] = geschwindigkeit[i] * 255 / max; 
		 }
	 }*/
     
	
	
	/******************* PID AUSRECHNEN ***************/
	//****************Proportionalregelung
	 P = p_faktor * relativeTorrichtung;
	 
	//****************Integralregelung 
	 PID_flaeche += deltaT * relativeTorrichtung;
	 
	 if(PID_flaeche > PID_flaeche_max) {
		 PID_flaeche = PID_flaeche_max;
	 }
	 if(PID_flaeche < -PID_flaeche_max) {
		 PID_flaeche = -PID_flaeche_max;
	 }
	  
	 if((relativeTorrichtung_alt * relativeTorrichtung) <= 0)
		 PID_flaeche *= -0,8;   //Experimenteller Wert ||nicht getestet
	 
	 I = p_faktor * PID_flaeche;
	 
	//****************Differenzialrechnung
     D = d_faktor * ((relativeTorrichtung_alt-relativeTorrichtung) / deltaT);	 
	 
	 
	PID = P + I + D;
		îf(PID > PID_max) {
			PID = PID_max;
		}
		if(PID < -PID_max) {
			PID = -PID_max;
		}
	
	for(i=0;i<=2;i++) {
		geschwindigkeit[i] = geschwindigkeit[i] + PID;
	}
	
	max = 0;
	 for(i=0;i<=2;i++) {
		if (abs(geschwindigkeit[i]) > max) {
			max = abs(geschwindigkeit[i]);
		} 
	 }
	 if (max > geschwindigkeit_max) {
		 for(i=0;i<=2;i++) {
			geschwindigkeit[i] = geschwindigkeit[i] * geschwindigkeit_max / max; 
		 }
	 }
	 
	 MotorPower(0,geschwindigkeit[0]);
	 MotorPower(1,geschwindigkeit[1]);
	 MotorPower(2,geschwindigkeit[2]);
	 
	 
	 
	 relativeTorrichtung_alt = relativeTorrichtung;
}