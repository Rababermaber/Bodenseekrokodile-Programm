/*
 * pid_regelung.c
 *
 * Created: 15.02.2016 18:02:12
 *  Author: Aaron
 */
 
#include "pid.h"

int16_t P;
int16_t I;
int16_t D;
int16_t PID;
int16_t PID_flaeche;
int16_t relativeTorrichtung_alt;

void pid(int p_faktor, int i_faktor, int d_faktor, int relativeTorrichtung) {
	//****************Proportionalregelung
	 P = p_faktor * relativeTorrichtung;
	 
	//****************Integralregelung 
	 PID_flaeche += deltaT * relativeTorrichtung;
	 
	 if (PID_flaeche > PID_flaeche_max) {
		 PID_flaeche = PID_flaeche_max;
	 }
	 if (PID_flaeche < -PID_flaeche_max) {
		 PID_flaeche = -PID_flaeche_max;
	 }
	  
	 if ((relativeTorrichtung_alt * relativeTorrichtung) <= 0)
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
		
	 
	 
	 
	 relativeTorrichtung_alt = relativeTorrichtung;
	 return PID;
}