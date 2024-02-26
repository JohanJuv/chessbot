#pragma once
#include <string>
#include "shakki.h"

// Kuvaa aseman muutosta.
class Siirto
{
public:

	Siirto()
	{}

	Siirto(int a_r, int a_l, int l_r, int l_l, int korotettava = NA) :
		_a_r(a_r), _a_l(a_l), _l_r(l_r), _l_l(l_l), _korotettava_nappula(korotettava)
	{}

	// Parsi siirto stringistä
	// esimn e2e4 tai g8f6
	Siirto(const std::string& s);

	// Muodostaa merkkijonon siirrosta.
	std::wstring muodosta_string_siirrosta() const;

	void print_siirto() const;

	// Vertailuoperaattori: ovatko kaksi siirto-oliota identtisiä?
	bool operator==(const Siirto& rhs)
	{
		if (_a_r == rhs._a_r && _a_l == rhs._a_l && _l_r == rhs._l_r && _l_l == rhs._l_l) {
			return true;
		}
		return false;
	}

private:

	int _a_r; // Siirron alkuruudun rivi
	int _a_l; // Siirron alkuruudun linja (eli sarake)

	int _l_r; // Siirron loppuruudun rivi
	int _l_l; // Siirron loppuruudun linja (eli sarake)

	int _korotettava_nappula = NA; // esim. wQ, wR, bN jne.
	bool _ohestalyonti = false;

	friend class Asema;
};