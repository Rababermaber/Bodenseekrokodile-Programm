/*
 * wtdMOVEMENT.h
 *
 * Created: 27.02.2016 15:40
 *  Author: Aaron
 */
 
 #include "wtdIR.h"
 

int16_t CMPcorrection=0;
volatile uint16_t time = 0;
volatile int16_t time_total,time_old;
int16_t schuss = 0;

//test 
int16_t zeit_alt[MAX_SCHUSS];
uint8_t schuss_test[MAX_SCHUSS];
uint8_t schuss_total;
int16_t schussen;

void shoot_test() {
	while((time - zeit_alt[0]) > TIME_MAX_SCHUSS) {
		for(uint8_t z = 0;z < (MAX_SCHUSS-1);z++) {
			zeit_alt[z] = zeit_alt[(z+1)];
			schuss_test[z] = schuss_test[(z+1)];
		} 
		zeit_alt[(MAX_SCHUSS-1)] = time;
		schuss_test[(MAX_SCHUSS-1)] = 0;
	}
	
	
	if(schuss_test[0] == MAX_SCHUSS) {
		return; //eigentlich break??
	} else {
		schuss_total = schuss_test[0];
		for(uint8_t z = 0;z <= schuss_total;z++) {
			schuss_test[z] ++;
		}
		schuss_test[schuss_total] = time;
		
		if((time - schussen) > 100) {
			SETPORT(PORTB,0);
			_delay_ms(10);
			CLEARPORT(PORTB,0);
			schussen = time;
		}		
	}
	
	
	/*for(uint8_t z = 0;z < MAX_SCHUSS;z++) {
		if(schuss_test[(MAX_SCHUSS-(MAX_SCHUSS-z))] == MAX_SCHUSS) {
			break;
		} else if(schuss_test[(MAX_SCHUSS-(MAX_SCHUSS-z))] == 0) {
			zeit_alt[(MAX_SCHUSS-(MAX_SCHUSS-z))] = time;
			schuss_test[(MAX_SCHUSS-(MAX_SCHUSS-z))] = 1;
		}
	}*/
}


void doShoot() {
	if((time - time_old) > (142 * TIME_MAX_SCHUSS)) {
		time_total = time - time_old;
		while(time_total > (142 * TIME_MAX_SCHUSS)) {
			time_total -= 142 * TIME_MAX_SCHUSS;
		}
		schuss = 0;
	}
		
	if(time_total <= (142 * TIME_MAX_SCHUSS)) {
			if(schuss < MAX_SCHUSS) {
			SETPORT(PORTB,0);
			_delay_ms(10);
			CLEARPORT(PORTB,0);
			schuss ++;
		}
	}
		
		time_old = time;	
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