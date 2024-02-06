#pragma once
#include <string>
#include "shakki.h"

// Kuvaa aseman muutosta.
class Siirto
{
public:
	Siirto(int a_r, int a_l, int l_r, int l_l) :
		_a_r(a_r), _a_l(a_l), _l_r(l_r), _l_l(l_l)
	{}

	// Parsi siirto stringist�
	// esimn e2e4 tai g8f6
	Siirto(const std::string& s);

private:

	int _a_r; // Siirron alkuruudun rivi
	int _a_l; // Siirron alkuruudun linja (eli sarake)

	int _l_r; // Siirron loppuruudun rivi
	int _l_l; // Siirron loppuruudun linja (eli sarake)

	int _korotettava_nappula = NA; // esim. wQ, wR, bN jne.

	friend class Asema;
};