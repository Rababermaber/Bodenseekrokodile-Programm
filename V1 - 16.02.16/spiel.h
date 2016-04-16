// spiel.h: Spielfunktionen

////////////////
// Funktionen //
////////////////

// Spielfunktion B-Feld
inline void spielB1(void) 
{
    if(ballda) { // Wenn Ball da
		if(soll_phi != 0 || (US_Werte[3] > 30 && US_Werte[3] < 40 && ((US_Werte[0] >= 25 && US_Werte[0] <= 35) || (US_Werte[2] >= 25 && US_Werte[2] <= 35)) && US_Werte[1] > 120 && US_Werte[4] > 120)) {
			Fahrtrichtung(0, 0);
			if(US_pos[0] < 91) { // links
				soll_phi = -75;
				if(phi_jetzt < -60)
					Kick();
			}
			else { // rechts
				soll_phi = 75;
				if(phi_jetzt > 60)
					Kick();
			}
		}
        else if(US_Werte[0] + US_Werte[2] < 120) {
            if(US_Werte[3] < 50 || US_Werte[1] < 30) {
                if(US_Werte[0] < US_Werte[2]) {
                    FahrtrichtungB(90, SPEED_SEITE);
                }
                else {
                    FahrtrichtungB(-90, SPEED_SEITE);
                }
            }
            else {
                #ifdef _SCHUSS
                    FahrtrichtungB(0, 0);
                    if(BETRAG(phi_jetzt) < 5)
                        Kick();
                #else
                    FahrtrichtungB(0, SPEED_WEIT);
                #endif
            }
        }
        else if(US_Werte[0] < 68) { // zu weit rechts
            FahrtrichtungB(90, SPEED_SEITE);
        }
        else if(US_Werte[2] < 68) { // zu weit links
            FahrtrichtungB(-90, SPEED_SEITE);
        }
        else {
            #ifdef _SCHUSS
                FahrtrichtungB(0, 0);
                if(BETRAG(phi_jetzt) < 5)
                    Kick();
            #else
                FahrtrichtungB(0, SPEED_WEIT);
            #endif
        }
    }
	else if(soll_phi != 0) {
		soll_phi = 1;
		if(BETRAG(phi_jetzt) < 5) {
			soll_phi = 0;
			wdt_delay(200);
		}
	}
	#ifdef _SCHUSS
		else if(ballIntens > 1700 && BETRAG(ball_Winkel) < 15) { // Nahbereich-Anfahrt verlangsamen
			FahrtrichtungB(2*ball_Winkel, SPEED_SEHRNAH);
		}
	#endif
    else if(ballIntens > 1000) { // Nahbereich-Anfahrt
        if(ball_Winkel > 90) // zwischen Ball und Tor rechts
            FahrtrichtungB(-270+ball_Winkel, SPEED_KREIS);
        else if(ball_Winkel < -90) // zwischen Ball und Tor links
            FahrtrichtungB(270+ball_Winkel, SPEED_KREIS);
        else // hinter Ball								
            FahrtrichtungB(2*ball_Winkel, SPEED_NAH);
    }
    else if(ballIntens < 30) { // Ball nicht erkennbar
        Fahrtrichtung_XY(90, 50);
    }
    else { // Fernbereich-Anfahrt
        FahrtrichtungB(ball_Winkel, SPEED_WEIT);
    }
}

// Spielfunktion B-Feld (Drehung zum Tor)
inline void spielB2(void)
{
	if(ballda) {
		#ifdef _SCHUSS
			FahrtrichtungB(0, 0);
			if(soll_phi == 0 && (US_Werte[2]+US_Werte[0] >= 140))
				soll_phi = (int16_t)Round(atan2(US_pos[0]-91, US_Werte[3]+30)*180.0/M_PI);
			if(BETRAG(phi_jetzt-soll_phi) < 5)
				Kick();
		#else
			if(soll_phi == 0 && (US_Werte[2]+US_Werte[0] >= 140))
				soll_phi = (int16_t)Round(atan2(US_pos[0]-91, US_Werte[3]+10)*180.0/M_PI);
			FahrtrichtungB(0, SPEED_WEIT);
		#endif
	}
	#ifdef _SCHUSS
		else if(ballIntens > 1700 && BETRAG(ball_Winkel) < 60) { // Nahbereich-Anfahrt verlangsamen
			FahrtrichtungB(2*ball_Winkel, SPEED_SEHRNAH);
			soll_phi = 0;
		}
	#endif
	else if(ballIntens > 1000) { // Nahbereich-Anfahrt
		if(ball_Winkel > 90) // zwischen Ball und Tor rechts
			FahrtrichtungB(-270+ball_Winkel, SPEED_KREIS);
		else if(ball_Winkel < -90) // zwischen Ball und Tor links
			FahrtrichtungB(270+ball_Winkel, SPEED_KREIS);
		else // hinter Ball								
			FahrtrichtungB(2*ball_Winkel, SPEED_NAH);
		soll_phi = 0;
	}
	else if(ballIntens < 30) { // Ball nicht erkennbar
		Fahrtrichtung_XY(90, 45);
		soll_phi = 0;
	}
	else { // Fernbereich-Anfahrt
		FahrtrichtungB(ball_Winkel, SPEED_WEIT);
		soll_phi = 0;
	}
}

// Torwartfunktion B-Feld
static inline void torwartB(void) 
{
	#if _TORWART==2
		// Variablen
		static int8_t raus = 0;
		
		// Funktion
		if(US_pos[1] > 30) {
			if(raus == -1)
				Fahrtrichtung_XY(106, 44);
			else if(raus == 1)
				Fahrtrichtung_XY(76, 15);
			else
				Fahrtrichtung_XY(91, 15);
		}
		else if(US_Werte[4] > US_Werte[1]+15) { // links raus
			raus = -1;
			FahrtrichtungB(-90, SPEED_TORWART);
		}
		else if(US_Werte[1] > US_Werte[4]+15) { // rechts raus
			raus = 1;
			FahrtrichtungB(90, SPEED_TORWART);
		}
		else { // mittig
			raus = 0;
			//if(ballIntens
			if(US_Werte[0]+10 < US_Werte[2]) {
				Fahrtrichtung_Y(SPEED_TORWART, 15);
			}
			else if(US_Werte[0] > US_Werte[2]+10) {
				Fahrtrichtung_Y(-SPEED_TORWART, 15);
			}
			else {
				Fahrtrichtung_XY(91, 15);
			}
		}
	#else
		if(ballda) {
			#ifdef _SCHUSS
				FahrtrichtungB(0, 0);
				if(BETRAG(phi_jetzt) < 5)
					Kick();
			#else
				FahrtrichtungB(0, 0);
			#endif
		}
		else if(ballIntens > 1700 && BETRAG(ball_Winkel) < 25 && US_pos[1] < 30) { // Nahbereich-Anfahrt verlangsamen
			FahrtrichtungB(2*ball_Winkel, SPEED_TORWART);
		}
		else if(((BETRAG(ball_Winkel) > 60 && ballIntens > 650) || ballIntens > 900) && US_pos[1] < 45) { // Anfahrt auf Ball
			if(ball_Winkel > 25) {
				FahrtrichtungB((ball_Winkel+20>90)?85:ball_Winkel+20, SPEED_TORWART);
			}
			else if(ball_Winkel < -25) {
				FahrtrichtungB((ball_Winkel-20<-90)?-85:ball_Winkel-20, SPEED_TORWART);
			}
			else {
				FahrtrichtungB(ball_Winkel, SPEED_TORWART);
			}
		}
		else { // eigentliche Torwartaktion
			#if _TORWART==1
				// Bewegung
				if (ballIntens < 30)
					torwart_x = 91;
				else if(BETRAG(ball_Winkel) > 2)
					torwart_x = US_pos[0] - sinus(ball_Winkel)*65;
				// Rand-Begrenzung
				if(torwart_x < 75)
					torwart_x = 75;
				else if(torwart_x > 107)
					torwart_x = 107;
				// Randbegrenzung über zwei US
				if(US_Werte[4]+20 < US_Werte[1]) { // rechts raus
					if(torwart_x+5 > US_pos[0])
						torwart_x = US_pos[0]-5;
				}
				else if(US_Werte[1]+20 < US_Werte[4]) { // links raus
					if(torwart_x-5 < US_pos[0])
						torwart_x = US_pos[0]+5;
				}
				// Positionsfahrt
				Fahrtrichtung_XY_offset(torwart_x, 15, 80);
			#else
				// Torposition
				if(BETRAG(ball_Winkel) < 15 || ballIntens < 17) 
					Fahrtrichtung_XY(91, 20);
				else if(ball_Winkel < 0)
					Fahrtrichtung_XY(107, 20);
				else
					Fahrtrichtung_XY(75, 20);
				// Randbegrenzung über zwei US
				if(US_Werte[4]+20 < US_Werte[1]) { // rechts raus
					FahrtrichtungB(90, SPEED_KREIS);
				}
				else if(US_Werte[1]+20 < US_Werte[4]) { // links raus
					FahrtrichtungB(-90, SPEED_KREIS);
				}
			#endif
		}
	#endif
}

// Spielfunktion A-Feld (in Mitte fahren)
inline void spielA1(void)
{
	if(ballda) { // Wenn Ball da
		if(US_Werte[0] + US_Werte[2] < 80) {
			if(US_Werte[3] < 30 || US_Werte[1] < 30) {
				if(US_Werte[0] < US_Werte[2]) {
					Fahrtrichtung(90, SPEED_SEITE);
				}
				else {
					Fahrtrichtung(-90, SPEED_SEITE);
				}
			}
			else {
				#ifdef _SCHUSS
					Fahrtrichtung(0, 0);
					if(BETRAG(phi_jetzt) < 25)
						Kick();
				#else
					Fahrtrichtung(0, SPEED_WEIT);
				#endif
			}
		}
		else if(US_Werte[0] < 40) { // zu weit rechts
			Fahrtrichtung(90, SPEED_SEITE);
		}
		else if(US_Werte[2] < 40) { // zu weit links
			Fahrtrichtung(-90, SPEED_SEITE);
		}
		else {
			#ifdef _SCHUSS
				Fahrtrichtung(0, 0);
				if(BETRAG(phi_jetzt) < 25)
					Kick();
			#else
				Fahrtrichtung(0, SPEED_WEIT);
			#endif
		}
	}
	#ifdef _SCHUSS
		else if(ballIntens > 1700 && BETRAG(ball_Winkel) < 15) { // Nahbereich-Anfahrt verlangsamen
			Fahrtrichtung(2*ball_Winkel, SPEED_SEHRNAH);
		}
	#endif
	else if(ballIntens > 1000) { // Nahbereich-Anfahrt
		if(ball_Winkel > 90) // zwischen Ball und Tor rechts
			Fahrtrichtung(-270+ball_Winkel, SPEED_KREIS);
		else if(ball_Winkel < -90) // zwischen Ball und Tor links
			Fahrtrichtung(270+ball_Winkel, SPEED_KREIS);
		else // hinter Ball								
			Fahrtrichtung(2*ball_Winkel, SPEED_NAH);
	}
	else if(ballIntens < 30) { // Ball nicht erkennbar
		Fahrtrichtung_XY(60, 45);
	}
	else { // Fernbereich-Anfahrt
		Fahrtrichtung(ball_Winkel, SPEED_WEIT);
	}
}

// Spielfunktion A-Feld (Drehung zum Tor)
inline void spielA2(void)
{
	if(ballda) {
		#ifdef _SCHUSS
			Fahrtrichtung(0, 0);
			if(soll_phi == 0 && (US_Werte[2]+US_Werte[0] >= 140))
				soll_phi = (int16_t)Round(atan2(US_pos[0]-61, US_Werte[3]+10)*180.0/M_PI);
			if(BETRAG(phi_jetzt-soll_phi) < 10)
				Kick();
		#else
			if(soll_phi == 0 && (US_Werte[2]+US_Werte[0] >= 140))
				soll_phi = (int16_t)Round(atan2(US_pos[0]-61, US_Werte[3]+10)*180.0/M_PI);
			Fahrtrichtung(0, SPEED_WEIT);
		#endif
	}
	#ifdef _SCHUSS
		else if(ballIntens > 1700 && BETRAG(ball_Winkel) < 15) { // Nahbereich-Anfahrt verlangsamen
			Fahrtrichtung(2*ball_Winkel, SPEED_SEHRNAH);
			soll_phi = 0;
		}
	#endif
	else if(ballIntens > 1000) { // Nahbereich-Anfahrt
		if(ball_Winkel > 90) // zwischen Ball und Tor rechts
			Fahrtrichtung(-270+ball_Winkel, SPEED_KREIS);
		else if(ball_Winkel < -90) // zwischen Ball und Tor links
			Fahrtrichtung(270+ball_Winkel, SPEED_KREIS);
		else // hinter Ball								
			Fahrtrichtung(2*ball_Winkel, SPEED_NAH);
		soll_phi = 0;
	}
	else if(ballIntens < 30) { // Ball nicht erkennbar
		Fahrtrichtung_XY(60, 45);
		soll_phi = 0;
	}
	else { // Fernbereich-Anfahrt
		Fahrtrichtung(ball_Winkel, SPEED_WEIT);
		soll_phi = 0;
	}
}

// Torwartfunktion A-Feld
inline void torwartA(void)
{
	if(ballda) {
		#ifdef _SCHUSS
			Fahrtrichtung(0, 0);
			if(BETRAG(phi_jetzt) < 10)
				Kick();
		#else
			Fahrtrichtung(0, 0);
		#endif
	}
	#ifdef _SCHUSS
		else if(ballIntens > 1700 && BETRAG(ball_Winkel) < 15 && US_Werte[1] < 30) { // Nahbereich-Anfahrt verlangsamen
			Fahrtrichtung(2*ball_Winkel, SPEED_SEHRNAH);
		}
	#endif
	else if(ballIntens > 1200 && US_Werte[1] < 30) { // Anfahrt auf Ball
		if(ball_Winkel > 25) {
			Fahrtrichtung(ball_Winkel+20, SPEED_KREIS);
		}
		else if(ball_Winkel < -25) {
			Fahrtrichtung(ball_Winkel-20, SPEED_KREIS);
		}
		else {
			Fahrtrichtung(ball_Winkel, SPEED_KREIS);
		}
	}
	else { // eigentliche Torwartaktion
		#if _TORWART==1
			// Bewegung
			if (ballIntens < 30)
				torwart_x = 61;
			else if(BETRAG(ball_Winkel) > 2)
				torwart_x = US_pos[0] - sinus(ball_Winkel)*120;
			// Rand-Begrenzung
			if(torwart_x < 45)
				torwart_x = 45;
			else if(torwart_x > 75)
				torwart_x = 75;
			// Positionsfahrt
			Fahrtrichtung_XY(torwart_x, 22);
		#else
			if(BETRAG(ball_Winkel) < 20 || ballIntens < 10) 
				Fahrtrichtung_XY(61, 25);
			else if(ball_Winkel < 0)
				Fahrtrichtung_XY(80, 20);
			else
				Fahrtrichtung_XY(42, 20);
		#endif
	}
}

// Nullprogramm
inline void PIDprogramm(void)
{
	soll_phi = 0;
	Fahrtrichtung(0, 0);
}
