/*
 * fahren.h
 *
 * Created: 15.02.2016 18:02:12
 *  Author: Aaron
 */ 
 
#ifndef FAHREN_H
#define FAHREN_H 

#include "sinus.h"
#include "cosinus.h"

#define deltaT 0.0096       //übernommen aus PID.nxc von TIM LUCHTERHAND
#define PID_flaeche_max 500 //übernommen aus PID.nxc von TIM LUCHTERHAND
#define PID_max 200         //höchstwert für PID Regelung
//++++++++ PID EINSTELLEN ++++++++
#define p_faktor 0.0
#define i_faktor 0.0
#define d_faktor 0.0
const float rechtsmotorfaktoren[3] = {
	-1,
	0,7,
	0,7
};
const float vorwaertsmotorfaktoren[3] = {
	0,
	1,
	-1
};
 

/************************     FAHREN COMMANDS    ************************/
void fahren(int richtung, int geschwindigkeit_max, int relativeTorrichtung);
/************************************************************************/

