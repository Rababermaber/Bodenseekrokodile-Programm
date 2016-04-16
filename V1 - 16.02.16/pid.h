/*
 * pid_regelung.h
 *
 * Created: 15.02.2016 18:02:12
 *  Author: Aaron
 */ 
 
#ifndef PID_H
#define PID_H 

#include "sinus.h"
#include "cosinus.h"

#define deltaT 0.0096       //übernommen aus PID.nxc von TIM LUCHTERHAND
#define PID_flaeche_max 500 //übernommen aus PID.nxc von TIM LUCHTERHAND
#define PID_max 200         //höchstwert für PID Regelung
 
/************************************************************************/
/*****************     PID REGELUNG FUNKTIONEN    ***********************/
/************************************************************************/

void pid(int p_faktor, int i_faktor, int d_faktor, int relativeTorrichtung);

/************************************************************************/
/***************    PID REGELUNG FUNKTIONEN ENDE   **********************/
/************************************************************************/
