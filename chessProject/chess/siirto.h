#pragma once
#include <string>

// Kuvaa aseman muutosta.
class Siirto
{
public:
	Siirto(int a_r, int a_l, int l_r, int l_l) :
		_a_r(a_r), _a_l(a_l), _l_r(l_r), _l_l(l_l)
	{}

	// Parsi siirto stringistä
	// esimn e2e4 tai g8f6
	Siirto(const std::string& s) {
		const char a_r = s[1];
		const char a_l = s[0];
		const char l_r = s[3];
		const char l_l = s[2];

		_a_r = 56 - (unsigned int)a_r;
		_a_l = (unsigned int)a_l - 97;
		_l_r = 56 - (unsigned int)l_r;
		_l_l = (unsigned int)l_l - 97;
	}

private:

	int _a_r; // Siirron alkuruudun rivi
	int _a_l; // Siirron alkuruudun linja (eli sarake)

	int _l_r; // Siirron loppuruudun rivi
	int _l_l; // Siirron loppuruudun linja (eli sarake)

	friend class Asema;
};