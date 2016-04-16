// motor.h: Funktionen zur Motoransteuerung

//////////////////
// Definitionen //
//////////////////

// Relative Fahrtrichtung
#define FahrtrichtungR(alpha, speed) Fahrtrichtung(alpha, speed)
// Absolute Fahrtrichtung
#define FahrtrichtungA(alpha, speed) Fahrtrichtung((alpha)-phi_jetzt, speed)

////////////////
// Funktionen //
////////////////

// Runden// Runden
inline float Round(float r)
{
	return ((r>0.0) ? floor(r+0.5) : ceil(r-0.5));
}

// Motorgeschwindigkeit setzen
void MotorPower(uint8_t motor_id, int16_t power)
{
	// Power-Wert zwischen +-255, Vorzeichen gibt die Richtung an
    uint8_t p;
    if(abs(power)>255)p=255;
    else p=abs(power);
    
	// Motorsteuerung
	switch(motor_id)
	{
		// SCHWACHSTELLE!!!
		// <> Aufpassen
		case 0:
		{
			if(power < 0)
				PORT_M0 |= (1 << PM0);
			else
				PORT_M0 &= ~(1 << PM0);
		}
		OCR_M0 = p;
		break;
		case 1:
		{
			if(power < 0)
				PORT_M1 |= (1 << PM1);
			else
				PORT_M1 &= ~(1 << PM1);
		}
		OCR_M1 = p;
		break;
		case 2:
		{
			if(power > 0)
				PORT_M2 |= (1 << PM2);
			else
				PORT_M2 &= ~(1 << PM2);
		}
		OCR_M2 = p;
		break;
		case 3:
		{
			if(power > 0)
				PORT_M3 |= (1 << PM3);
			else
				PORT_M3 &= ~(1 << PM3);
		}
		OCR_M3 = p;
		break;
        
		
	}
}
