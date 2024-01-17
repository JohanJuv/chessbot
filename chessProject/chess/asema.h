#pragma once
#include "shakki.h"
#include "siirto.h"

class Asema
{

public:
	void tyhjenna();
	void tee_siirto(const Siirto& s);

	// Läksy tulosta lauta ascii-grafiikkana
	void tulosta() const;

private:

	// Laudan nappulat. Indeksointi [rivi][linja], esim
	//
	// [7][0] : vasen alanurkka ("a1")
	// [7][7] : oikea alanurkka ("h1")
	//
	int _lauta[8][8] = {
		{bR, bN, bB, bQ, bK, bB, bN, bR},
		{bP, bP, bP, bP, bP, bP, bP, bP},
		{NA, NA, NA, NA, NA, NA, NA, NA},
		{NA, NA, NA, NA, NA, NA, NA, NA},
		{NA, NA, NA, NA, NA, NA, NA, NA},
		{NA, NA, NA, NA, NA, NA, NA, NA},
		{wP, wP, wP, wP, wP, wP, wP, wP},
		{wR, wN, wB, wQ, wK, wB, wN, wR}
	};

	int _siirtovuoro = VALKEA;
};
