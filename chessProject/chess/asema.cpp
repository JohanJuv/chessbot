#include "asema.h"

void Asema::tyhjenna()
{
	for (int rivi = 0; rivi < 8; rivi++){ 
		for (int linja = 0; linja < 8; linja++) {
			_lauta[rivi][linja] = NA;
		}
	}
}

// Tekee annetun siirron laudalla. Voidaan olettaa, että
// Siirto on laillinens
void Asema::tee_siirto(const Siirto& s)
{
	// Otetaan nappula talteen, tyhjennetään alkuruutu, sijoitetaan nappula loppuruutuun
	int temp = _lauta[s._a_r][s._a_l];
	_lauta[s._a_r][s._a_l] = NA;
	_lauta[s._l_r][s._l_l] = temp;
}