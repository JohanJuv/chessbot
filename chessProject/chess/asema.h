#pragma once
#include <vector>
#include "shakki.h"
#include "siirto.h"
#include "nappula.h"


class Asema
{

public:
	void tyhjenna();
	void tee_siirto(const Siirto& s);

	// Läksy tulosta lauta ascii-grafiikkana
	Nappula* _lauta[8][8];

	void tulosta() const;
	static Nappula* wk, * wq, * wr, * wb, * wn, * wp;
	static Nappula* bk, * bq, * br, * bb, * bn, * bp;	

	void anna_tornin_raakasiirrot(int rivi, int linja, int pelaaja,
		std::vector<Siirto>& siirrot) const;

	Asema();

private:

	// Laudan Nappulat. Indeksointi [rivi][linja], esim
	//
	// [7][0] : vasen alanurkka ("a1")
	// [7][7] : oikea alanurkka ("h1")
	//

	int _siirtovuoro = VALKEA;
};
