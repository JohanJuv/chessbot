#include "asema.h"

void Asema::tyhjenna()
{
	for (int rivi = 0; rivi < 8; rivi++){ 
		for (int sarake = 0; sarake < 8; sarake++) {
			_lauta[rivi][sarake] = NA;
		}
	}
}