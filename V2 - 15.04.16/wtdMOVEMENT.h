/*
 * wtdMOVEMENT.h
 *
 * Created: 27.02.2016 15:40
 *  Author: Aaron
 */
 
 #include "wtdIR.h"
 

int8_t CMPcorrection=0;
uint8_t x,y;
	
void getLocation(int16_t US_l,int16_t US_r,int16_t US_h)	{    
	/** Spielfeld wird in 6*9 Abschnitte unterteilt es wird in xy übergeben (bsp: "return 58;")	**/ 
	/**                                                -> return 58 -> x = 5 / y = 8            **/
	/**                 -> 6 Abschnitte in x Richtung und 9 Abschnitte in y Richtung 			**/
	/** 				-> Insgesamt 54 Abschnitte 												**/
	/** 				-> Es wird bei 1 angefangen zu zählen (kein x/y = 0)					**/
	
	if(abs((US_l + US_r + ROBOTER_WIDTH) - FIELD_WIDTH) <= MAX_DIFFERENCE) {
		x = (US_l + (ROBOTER_WIDTH / 2)) / (FIELD_WIDTH / 6);
	}
	y = (US_h + (ROBOTER_WIDTH / 2)) / (FIELD_LENGHT / 9);
	
	//return (x * 10 + y);   //fkt für return bei getLocation, dennoch werden dann x und y bei jedem aufruf von getLocation resetet -> x/y jetzt global
}
	
int8_t getShootangle(int16_t US_l,int16_t US_r,int16_t US_h) {
	float angle_xy = 0;
	
	/*xy = getLocation(US_l,US_r,US_h);  //fkt für return bei getLocation, dennoch werden dann x und y bei jedem aufruf von getLocation resetet -> x/y jetzt global
	for(uint8_t i = 0;i <= 6;i++) {
		if((xy - i * 10) < 10) {
			x = i;
			y = xy;
			i = 7;
		}
	}*/
	
	switch(x) {
		case 1: angle_xy= 15;  break;
		case 2: angle_xy= 10;  break;
		case 3: angle_xy= 5;   break;
		case 4: angle_xy= -5;  break;
		case 5: angle_xy= -10; break;
		case 6: angle_xy= -15; break;
	}
	
	for(int8_t z = 0;z < y;z++) {
		angle_xy *= 1.2;
	}
	return (int8_t)angle_xy;
	
}

int16_t getPID(int16_t CMPdifference, int16_t angVelocity) {
	float PIDsurface = 0;
	int16_t CMPdifference_old = 0;
	
	CMPdifference += CMPcorrection;
	if(CMPdifference > 180) {
		CMPdifference -= 360;
	} else if(CMPdifference < -180) {
		CMPdifference += 360;
	}
	
	PIDsurface += dt * CMPdifference;
	if(PIDsurface > PIDsurface_max) {
		PIDsurface = PIDsurface_max;
	} else if(PIDsurface < -PIDsurface_max) {
		PIDsurface = -PIDsurface_max;
	}
	
	if(CMPdifference * CMPdifference_old <= 0) {
		PIDsurface *= -0,8;
	}

	float PID = (P * CMPdifference) + (-I * PIDsurface) - (D * angVelocity);
	
	if(PID > PID_max) {
		PID = PID_max;
	} else if(PID < -PID_max) {
		PID = -PID_max;
	}
	CMPdifference_old = CMPdifference;
	return (int16_t)PID;
}

int16_t epicFunction(uint8_t motorNo, int16_t leistung, int16_t winkel) {
	leistung = leistung % 256;
	winkel += 180;
	if(winkel >= 360) {
		winkel -= 360;
	}
	winkel = winkel % 360;
	float fahrleistung = cosinus(winkel - motorWinkel[motorNo]) * leistung;
	return (int16_t)fahrleistung;
}


void doDriveIR(int16_t CMPdifference, int16_t angVelocity, int16_t v_max) {
	int16_t power[3]={0};
	int16_t PID = getPID(CMPdifference,angVelocity);
	int16_t IR_direction = getIR_direction();
	for(uint8_t i=0; i<3; i++) {
		power[i] = epicFunction(i,v_max,IR_direction);
		power[i] += PID; 
		MotorPower(i,i==1?-power[i]:power[i]);
	}
	
	
}