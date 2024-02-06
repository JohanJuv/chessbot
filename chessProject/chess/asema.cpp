#include "asema.h"
#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <stdio.h>
using namespace std;


void Asema::anna_siirrot(vector<Siirto>& siirrot)
{
	int kuningas = _siirtovuoro == VALKEA ? wK : bK;
	int pelaaja = _siirtovuoro;
	int vihu = vastustaja(pelaaja);

	// Luodaan siirtovektori. osa siirroista saattaa
	// Jättää kuninkaan uhatuksi.
	std::vector<Siirto> raakasiirrot;
	anna_kaikki_raakasiirrot(pelaaja, raakasiirrot);

	// Testataan jokainen raakasiirto.
	for (Siirto& rs : raakasiirrot)
	{
		// Luodaan kopio nykyisestä asemasta.
		Asema testiAsema = *this;

		testiAsema.tee_siirto(rs);
		int x, y;
		testiAsema.etsi_nappula(kuningas, x, y);
		if (!testiAsema.onko_ruutu_uhattu(x, y, vihu)) {
			siirrot.push_back(rs);
		}

	}
}

void Asema::anna_kaikki_raakasiirrot(int pelaaja, std::vector<Siirto>& siirrot) const
{
	for (int rivi = 0; rivi < 8; ++rivi)
		for (int linja = 0; linja < 8; ++linja)
		{
			int nappula = _lauta[rivi][linja];

			// Tyhjä ruutu?
			if (nappula == NA)
				continue;

			// Vastustajan nappula?
			if (nappulan_vari(nappula) != pelaaja)
				continue;

			// Haetaan nappulan raakasiirrot.
			switch (nappula)
			{
			case wR: case bR:
				anna_tornin_raakasiirrot(rivi, linja, pelaaja, siirrot);
				break;
			case wQ: case bQ:
				anna_daamin_raakasiirrot(rivi, linja, pelaaja, siirrot);
				break;
			case wN: case bN:
				anna_ratsun_raakasiirrot(rivi, linja, pelaaja, siirrot);
				break;
			case wB: case bB:
				anna_lahetin_raakasiirrot(rivi, linja, pelaaja, siirrot);
				break;
			case wK: case bK:
				anna_kuninkaan_raakasiirrot(rivi, linja, pelaaja, siirrot);
				break;
			case wP: case bP:
				anna_sotilaan_raakasiirrot(rivi, linja, pelaaja, siirrot);
				break;
			}
		}
}


void Asema::tyhjenna()
{
	for (int rivi = 0; rivi < 8; ++rivi)
		for (int linja = 0; linja < 8; ++linja)
			_lauta[rivi][linja] = NA;
}


void Asema::tee_siirto(const Siirto& s)
{
	// Otetaan alkuruudussa oleva nappula talteen.
	int nappula = _lauta[s._a_r][s._a_l];

	// Tyhjennetään alkuruutu.
	_lauta[s._a_r][s._a_l] = NA;

	// Sijoitetaan loppuruutuun alkuperäinen nappula.
	_lauta[s._l_r][s._l_l] = nappula;

	// Vaihdetaan siirtovuoro.
	_siirtovuoro = vastustaja(_siirtovuoro);
}

void Asema::anna_raakasiirrot_suunnassa(int rivi, int linja, int rivi_muutos, int linja_muutos,
	int pelaaja, int max_askeleet, bool voi_lyoda, bool pakko_lyoda, std::vector<Siirto>& siirrot) const
{
	int rivi_nyt = rivi;
	int linja_nyt = linja;
	int askeleet = 0;
	while (askeleet < max_askeleet)
	{
		rivi_nyt += rivi_muutos;
		linja_nyt += linja_muutos;

		// Ulkona laudalta?
		if (rivi_nyt < 0 || rivi_nyt > 7 || linja_nyt < 0 || linja_nyt > 7)
			break;

		// Tyhjä ruutu?
		if (_lauta[rivi_nyt][linja_nyt] == NA)
		{
			if (pakko_lyoda)
				break;
			siirrot.push_back(Siirto(rivi, linja, rivi_nyt, linja_nyt));
			askeleet++;
			continue;
		}

		// Törmätään omaan nappulaan?
		if (nappulan_vari(_lauta[rivi_nyt][linja_nyt]) == pelaaja)
			break;

		// Lyödään vastustajan nappula.
		if (voi_lyoda)
			siirrot.push_back(Siirto(rivi, linja, rivi_nyt, linja_nyt));
		break;
	}
}

void Asema::anna_tornin_raakasiirrot(int rivi, int linja, int pelaaja,
	vector<Siirto>& siirrot) const
{
	anna_raakasiirrot_suunnassa(rivi, linja, -1, 0, pelaaja, 7, true, false, siirrot);
	anna_raakasiirrot_suunnassa(rivi, linja, 1, 0, pelaaja, 7, true, false, siirrot);
	anna_raakasiirrot_suunnassa(rivi, linja, 0, -1, pelaaja, 7, true, false, siirrot);
	anna_raakasiirrot_suunnassa(rivi, linja, 0, 1, pelaaja, 7, true, false, siirrot);
}

void Asema::anna_lahetin_raakasiirrot(int rivi, int linja, int pelaaja,
	vector<Siirto>& siirrot) const
{
	anna_raakasiirrot_suunnassa(rivi, linja, 1, 1, pelaaja, 7, true, false, siirrot);
	anna_raakasiirrot_suunnassa(rivi, linja, 1, -1, pelaaja, 7, true, false, siirrot);
	anna_raakasiirrot_suunnassa(rivi, linja, -1, -1, pelaaja, 7, true, false, siirrot);
	anna_raakasiirrot_suunnassa(rivi, linja, -1, 1, pelaaja, 7, true, false, siirrot);
}

void Asema::anna_daamin_raakasiirrot(int rivi, int linja, int pelaaja,
	vector<Siirto>& siirrot) const
{
	anna_tornin_raakasiirrot(rivi, linja, pelaaja, siirrot);
	anna_lahetin_raakasiirrot(rivi, linja, pelaaja, siirrot);
}

void Asema::anna_kuninkaan_raakasiirrot(int rivi, int linja, int pelaaja,
	vector<Siirto>& siirrot) const
{
	anna_raakasiirrot_suunnassa(rivi, linja, 1, 0, pelaaja, 1, true, false, siirrot);
	anna_raakasiirrot_suunnassa(rivi, linja, -1, 0, pelaaja, 1, true, false, siirrot);
	anna_raakasiirrot_suunnassa(rivi, linja, 0, 1, pelaaja, 1, true, false, siirrot);
	anna_raakasiirrot_suunnassa(rivi, linja, 0, -1, pelaaja, 1, true, false, siirrot);
	anna_raakasiirrot_suunnassa(rivi, linja, 1, 1, pelaaja, 1, true, false, siirrot);
	anna_raakasiirrot_suunnassa(rivi, linja, -1, 1, pelaaja, 1, true, false, siirrot);
	anna_raakasiirrot_suunnassa(rivi, linja, 1, -1, pelaaja, 1, true, false, siirrot);
	anna_raakasiirrot_suunnassa(rivi, linja, -1, -1, pelaaja, 1, true, false, siirrot);
}

void Asema::anna_ratsun_raakasiirrot(int rivi, int linja, int pelaaja,
	vector<Siirto>& siirrot) const
{
	anna_raakasiirrot_suunnassa(rivi, linja, 2, 1, pelaaja, 1, true, false, siirrot);
	anna_raakasiirrot_suunnassa(rivi, linja, 2, -1, pelaaja, 1, true, false, siirrot);
	anna_raakasiirrot_suunnassa(rivi, linja, -2, 1, pelaaja, 1, true, false, siirrot);
	anna_raakasiirrot_suunnassa(rivi, linja, -2, -1, pelaaja, 1, true, false, siirrot);
	anna_raakasiirrot_suunnassa(rivi, linja, 1, 2, pelaaja, 1, true, false, siirrot);
	anna_raakasiirrot_suunnassa(rivi, linja, 1, -2, pelaaja, 1, true, false, siirrot);
	anna_raakasiirrot_suunnassa(rivi, linja, -1, 2, pelaaja, 1, true, false, siirrot);
	anna_raakasiirrot_suunnassa(rivi, linja, -1, -2, pelaaja, 1, true, false, siirrot);
}

void Asema::anna_sotilaan_raakasiirrot(int rivi, int linja, int pelaaja,
	vector<Siirto>& siirrot) const
{
	int alkuRivi = pelaaja == VALKEA ? 6 : 1;
	int suunta = pelaaja == VALKEA ? -1 : 1;
	int enPassantRivi = pelaaja == VALKEA ? 3 : 4;
	if (rivi == alkuRivi) {
		anna_raakasiirrot_suunnassa(rivi, linja, suunta + suunta, 0, pelaaja, 1, true, false, siirrot);
	}else if (rivi == enPassantRivi) { // Voiko en passantissa syödä ns. kaksi nappulaa jos pawnin takana on toinen nappula?
		if (linja + 1 <= 7 && _lauta[rivi][linja + 1] == vastustaja(pelaaja)) {
			anna_raakasiirrot_suunnassa(rivi, linja, suunta, 1, pelaaja, 1, false, false, siirrot);
		}
		if (linja - 1 >= 0 && _lauta[rivi][linja - 1] == vastustaja(pelaaja)) {
			anna_raakasiirrot_suunnassa(rivi, linja, suunta, -1, pelaaja, 1, false, false, siirrot);
		}
	}
	anna_raakasiirrot_suunnassa(rivi, linja, suunta, 0, pelaaja, 1, false, false, siirrot);
	anna_raakasiirrot_suunnassa(rivi, linja, suunta, 1, pelaaja, 1, true, true, siirrot);
	anna_raakasiirrot_suunnassa(rivi, linja, suunta, -1, pelaaja, 1, true, true, siirrot);
}


void Asema::etsi_nappula(int nappula, int& rivi, int& linja) const {
	for (int x = 0; x < 8; ++x) {
		for (int y = 0; y < 8; ++y) {
			// Tyhjä ruutu?
			if (_lauta[x][y] == nappula) {
				rivi = x;
				linja = y;
				break;
			}
				
		}
	}
}

bool Asema::onko_ruutu_uhattu(int rivi, int linja, int uhkaava_pelaaaja) {
	vector<Siirto> siirrot;
	anna_kaikki_raakasiirrot(uhkaava_pelaaaja, siirrot);
	for(int i = 0; i < siirrot.size(); i++)
	{
		if (siirrot[i]._l_r == rivi && siirrot[i]._l_l == linja) {
			return true;
		}
	}
	return false;
}


void Asema::tulosta() const{
	_setmode(_fileno(stdout), _O_U16TEXT);

	const wstring nappulat[] =
	{ L"\u2656", L"\u2658", L"\u2657", L"\u2655", L"\u2654", L"\u2659",
		L"\u265C", L"\u265E", L"\u265D", L"\u265B", L"\u265A", L"\u265F"};

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (j == 0)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
				wcout << 8 - i << " ";
			}
			if ((i % 2 == 0 && (j % 2) - 1 == 0) || ((i % 2) - 1 == 0 && j % 2 == 0))
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_INTENSITY | 0x0030);
			}
			else
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
			}
			if (_lauta[i][j] == NA)
			{
				wcout << "  ";
			}
			else
			{
				wcout << nappulat[_lauta[i][j]] << " ";
			}
		}
		wcout << "\n";

	}

	wcout << "   A B C D E F G H\n\n";

}