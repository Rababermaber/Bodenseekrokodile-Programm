/*
 * wtdIR.h
 *
 * Created: 27.02.2016 15:40
 *  Author: Aaron
 */


int16_t getIR_max() {
	int16_t max = 0;
	for(uint8_t i=0;i<=7;i++) {
		if(ADC_Werte[i] > max) {
			max = ADC_Werte[i];
		}
	} 
	return max;
}

int16_t getIR_angle() {
	int16_t xDistance = 0;
	int16_t yDistance = 0;
	
	for(uint8_t i=0;i<8;i++) {
		if(ADC_Werte[i] > IRMIN_ANGLE) {
			xDistance += ADC_Werte[i] * cosinus(irGrad[i]);
			yDistance += ADC_Werte[i] * sinus(irGrad[i]);
		}		
	}
	int16_t ret = atan2(yDistance,xDistance) * 180 / 3.14159265358979323846264338327950;
			if(ret < 0) {
				ret += 360;
			}
	return ret;
}

int16_t getIR_direction() {
	int16_t ret;
	int16_t IR_angle = getIR_angle();
	if(IR_angle > 180) {
		IR_angle -= 360;
	}
	if(getIR_max() < IRMIN_DIRECTION) {
		ret = IR_angle;
	} else {
		if(abs(IR_angle) < 90) {
			ret = IR_angle * 2;
		} else {
			ret=IR_angle + (IR_angle<0?-IR_CIRCLE:IR_CIRCLE);
		}
	}
	return ret;
}