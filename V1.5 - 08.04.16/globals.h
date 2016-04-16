/*******************PID**********************/
	int16_t motorWinkel[]={330,90,210};	//winkel der Motoren für Funktion epicFunction in wtdMOVEMENT
	#define dt 0.0096					//übernommen aus pid.nxc von TIM LUCHTERHAND
	#define PIDsurface_max 500			//maximale flache für I (limitierung)
	#define PID_max 100 				//PID maximieren (limitierung)
	#define P 0.5//3					//P-Faktor
	#define I 1.9//1.6					//I-Faktor
	#define D 0.3//0.3					//D-Faktor
	
/*****************MOVEMENT*******************/
	#define IRMIN_ANGLE 100				//minimum wert für ausrechnung des ballwinkels
	#define IRMIN_DIRECTION 600			//grenze zwischen direkt anfahren und kreis fahren
	#define IR_CIRCLE 50				//wird +/- auf den ballwinkel für kreisanfahrt
	
/*******************IR***********************/
	int16_t irGrad[] = {23,68,113,158,203,248,293,338};  //Position der IR Sensoren
	
/*****************SCHUSS*********************/	
	#define TIME_MAX_SCHUSS 1 			//gibt die zeit an in der MAX_SCHUSS schüsse gemacht werden dürfen in sekunden an