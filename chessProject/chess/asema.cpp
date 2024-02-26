#include "asema.h"
#define NOMINMAX
#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <stdio.h>
#include <map>
using namespace std;


void Asema::anna_siirrot(vector<Siirto>& siirrot)
{
	int kuningas = _siirtovuoro == VALKEA ? wK : bK;
	int pelaaja = _siirtovuoro;
	int vihu = vastustaja(pelaaja);

	// Luodaan siirtovektori. osa siirroista saattaa
	// J‰tt‰‰ kuninkaan uhatuksi.
	std::vector<Siirto> raakasiirrot;
	anna_kaikki_raakasiirrot(pelaaja, raakasiirrot);
	anna_linnoitukset(pelaaja, raakasiirrot);

	// Testataan jokainen raakasiirto.
	for (Siirto& rs : raakasiirrot)
	{
		// Luodaan kopio nykyisest‰ asemasta.
		Asema testiAsema = *this;

		testiAsema.tee_siirto(rs);
		int x, y;
		testiAsema.etsi_nappula(kuningas, x, y);
		if (!testiAsema.onko_ruutu_uhattu(x, y, vihu)) {
			siirrot.push_back(rs);
		}

	}
}

float Asema::pisteyta_lopputulos() const
{
	if (_siirtovuoro == VALKEA)
	{
		// Etsit‰‰n valkean kuningas
		int rivi, linja;
		etsi_nappula(wK, rivi, linja);

		// Onko valkean kuningas uhattu?
		if (onko_ruutu_uhattu(rivi, linja, MUSTA))
			return -1000000; // musta on tehnyt matin
		else
			return 0; // patti
	}
	else
	{
		// Etsit‰‰n valkean kuningas
		int rivi, linja;
		etsi_nappula(bK, rivi, linja);

		// Onko valkean kuningas uhattu?
		if (onko_ruutu_uhattu(rivi, linja, VALKEA))
			return 1000000; // musta on tehnyt matin
		else
			return 0; // patti
	}
}

float Asema::evaluoi() const
{
	return 1.0f * materiaali() + 0.1f * mobiliteetti();

	// TODO
	// t‰ydent‰k‰‰ halutessanne uusilla pisteytett‰vill‰ aseman piirteill‰
}

MinimaxArvo Asema::minimax(int syvyys)
{
	// Generoidaan aseman siirrot.
	vector<Siirto> siirrot;
	anna_siirrot(siirrot);

	if (siirrot.size() == 0)
	{
		// Rekursion kantatapaus 1:
		// peli on p‰‰ttynyt (ei yht‰‰n laillista siirtoa).
		return MinimaxArvo(pisteyta_lopputulos(), Siirto());
	}

	if (syvyys == 0)
	{
		// Rekursion kantatapaus 2:
		// ollaan katkaisusyvyydess‰.
		return MinimaxArvo(evaluoi(), Siirto());
	}

	// Siirtoja on j‰ljell‰ ja ei olla katkaisusyvyydess‰,
	// joten kokeillaan yksitellen mahdollisia siirtoja,
	// ja kutsutaan minimax:a kullekin seuraaja-asemalle.
	// Otetaan paras minimax-arvo talteen (alustetaan
	// paras_arvo mahdollisimman huonoksi siirtovuoroisen
	// pelaajan kannalta).
	float paras_arvo = _siirtovuoro == VALKEA ?
		numeric_limits<float>::lowest() : numeric_limits<float>::max();
	Siirto paras_siirto;
	for (Siirto& s : siirrot)
	{
		Asema uusi = *this;
		uusi.tee_siirto(s);

		// Rekursioasekel: kutsutaan minimax:ia seuraaja-asemalle.
		MinimaxArvo arvo = uusi.minimax(syvyys - 1);

		// Jos saatiin paras arvo, otetaan se talteen.
		if (_siirtovuoro == VALKEA && arvo._arvo > paras_arvo)
		{
			paras_arvo = arvo._arvo;
			paras_siirto = s;
		}
		else if (_siirtovuoro == MUSTA && arvo._arvo < paras_arvo)
		{
			paras_arvo = arvo._arvo;
			paras_siirto = s;
		}
	}

	// Palautetaan paras arvo.
	return MinimaxArvo(paras_arvo, paras_siirto);
}

float Asema::materiaali() const
{
	// Liitet‰‰n nappulatyyppeihin niiden arvot.
	static map<int, float> nappuloiden_arvot = {
		{wP,  1.0f}, {wN,  3.0f}, {wB,  3.0f}, {wR,  5.0f}, {wQ,  9.0f},
		{bP, -1.0f}, {bN, -3.0f}, {bB, -3.0f}, {bR, -5.0f}, {bQ, -9.0f},
		{NA,  0.0f}
	};

	float arvo = 0;
	for (int rivi = 0; rivi < 8; ++rivi)
		for (int linja = 0; linja < 8; ++linja)
		{
			int nappula = _lauta[rivi][linja];
			arvo += nappuloiden_arvot[nappula];
		}
	return arvo;
}

float Asema::mobiliteetti() const
{
	vector<Siirto> valkean_siirrot;
	vector<Siirto> mustan_siirrot;

	// Funktion totetus on raskas, koska generoidaan raakasiirtoja.
	// TODO: voisiko optimoida optimoida jotenkin?
	anna_kaikki_raakasiirrot(VALKEA, valkean_siirrot);
	anna_kaikki_raakasiirrot(MUSTA, mustan_siirrot);

	return (float)valkean_siirrot.size() - (float)mustan_siirrot.size();
}

void Asema::anna_kaikki_raakasiirrot(int pelaaja, std::vector<Siirto>& siirrot) const
{
	for (int rivi = 0; rivi < 8; ++rivi)
		for (int linja = 0; linja < 8; ++linja)
		{
			int nappula = _lauta[rivi][linja];

			// Tyhj‰ ruutu?
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


void Asema::anna_linnoitukset(int pelaaja, std::vector<Siirto>& siirrot) const {
	if (pelaaja == VALKEA) {
		// Onko valkea kuningas uhattu
		if (!onko_ruutu_uhattu(7, 4, MUSTA)) {
			if (_valkea_lyhyt_linna_sallittu && _lauta[7][5] == NA && _lauta[7][6] == NA && !onko_ruutu_uhattu(7, 5, MUSTA)) {
				siirrot.push_back(Siirto("e1g1"));
			}
			if (_valkea_pitka_linna_sallittu && _lauta[7][1] == NA && _lauta[7][2] == NA && _lauta[7][3] == NA &&
				_lauta[7][6] == NA && !onko_ruutu_uhattu(7, 3, MUSTA)) {
				siirrot.push_back(Siirto("e1c1"));
			}
		}
	}
	else {
		if (!onko_ruutu_uhattu(0, 4, VALKEA)) {
			if (_musta_lyhyt_linna_sallittu && _lauta[0][5] == NA && _lauta[0][6] == NA && !onko_ruutu_uhattu(0, 5, VALKEA)) {
				siirrot.push_back(Siirto("e8g8"));
			}
			if (_musta_pitka_linna_sallittu && _lauta[0][1] == NA && _lauta[0][2] == NA && _lauta[0][3] == NA &&
				_lauta[0][6] == NA && !onko_ruutu_uhattu(0, 3, VALKEA)) {
				siirrot.push_back(Siirto("e8c8"));
			}
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

	// Tyhjennet‰‰n alkuruutu.
	_lauta[s._a_r][s._a_l] = NA;

	// Sijoitetaan loppuruutuun alkuper‰inen nappula.
	_lauta[s._l_r][s._l_l] = nappula;

	// Tutkitaan, oliko siirto linnoitus. Jos oli, niin pit‰‰
	// Siirt‰‰ myˆs tornia. Huom! Linnoitussiirron alku- ja loppukoordinaatit
	// ovat kuninkaan alku- ja loppukoordinaatit (esim. "e1g1").
	if (nappula == wK && s._a_r == 7 && s._a_l == 4 && s._l_r == 7 && s._l_l == 6) {
		// Valkean lyhyt linnoitus
		_lauta[7][7] = NA;
		_lauta[7][5] = wR;
	}
	else if (nappula == wK && s._a_r == 7 && s._a_l == 4 && s._l_r == 7 && s._l_l == 2) {
		// Valkean pitk‰ linnoitus
		_lauta[7][0] = NA;
		_lauta[7][3] = wR;
	}else if (nappula == bK && s._a_r == 0 && s._a_l == 4 && s._l_r == 0 && s._l_l == 6) {
		// Mustan lyhyt linnoitus
		_lauta[0][7] = NA;
		_lauta[0][5] = bR;
	}
	else if (nappula == bK && s._a_r == 0 && s._a_l == 4 && s._l_r == 0 && s._l_l == 2) {
		// Mustan pitk‰ linnoitus
		_lauta[0][0] = NA;
		_lauta[0][3] = bR;
	}

	// P‰ivitet‰‰n aseman linnoitus-flagit:
	// Jos nappula on kuningas, niin linnoitus ei en‰‰ sallittu
	//
	// Jos siirron alku- tai loppuruutu on h1 (tai a1, h8, a8), niin
	// linnoitus ko. suuntaan ei an‰‰ sallittu
	//
	// Siis: p‰ivitt‰k‰‰ _valkea_lyhyt_linna_sallittu jne.
	// (tarpeen mukaan)
	if (nappula == wK) {
		_valkea_lyhyt_linna_sallittu = false;
		_valkea_pitka_linna_sallittu = false;
	}
	else if (_lauta[7][0] != wR) {
		_valkea_pitka_linna_sallittu = false;
	}
	else if (_lauta[7][7] != wR) {
		_valkea_lyhyt_linna_sallittu = false;
	}

	if (nappula == bK) {
		_musta_lyhyt_linna_sallittu = false;
		_musta_pitka_linna_sallittu = false;
	}
	else if (_lauta[0][0] != bR) {
		_musta_pitka_linna_sallittu = false;
	}
	else if (_lauta[0][7] != bR) {
		_valkea_lyhyt_linna_sallittu = false;
	}

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

		// Tyhj‰ ruutu?
		if (_lauta[rivi_nyt][linja_nyt] == NA)
		{
			if (pakko_lyoda)
				break;
			siirrot.push_back(Siirto(rivi, linja, rivi_nyt, linja_nyt));
			askeleet++;
			continue;
		}

		// Tˆrm‰t‰‰n omaan nappulaan?
		if (nappulan_vari(_lauta[rivi_nyt][linja_nyt]) == pelaaja)
			break;

		// Lyˆd‰‰n vastustajan nappula.
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
		anna_raakasiirrot_suunnassa(rivi, linja, suunta, 0, pelaaja, 2, false, false, siirrot);
	}else if (rivi == enPassantRivi) { // Voiko en passantissa syˆd‰ ns. kaksi nappulaa jos pawnin takana on toinen nappula?
		if (linja + 1 <= 7 && _lauta[rivi][linja + 1] == vastustaja(pelaaja)) {
			anna_raakasiirrot_suunnassa(rivi, linja, suunta, 1, pelaaja, 1, false, false, siirrot);
		}
		if (linja - 1 >= 0 && _lauta[rivi][linja - 1] == vastustaja(pelaaja)) {
			anna_raakasiirrot_suunnassa(rivi, linja, suunta, -1, pelaaja, 1, false, false, siirrot);
		}
	}
	else {
		anna_raakasiirrot_suunnassa(rivi, linja, suunta, 0, pelaaja, 1, false, false, siirrot);
	}
	
	anna_raakasiirrot_suunnassa(rivi, linja, suunta, 1, pelaaja, 1, true, true, siirrot);
	anna_raakasiirrot_suunnassa(rivi, linja, suunta, -1, pelaaja, 1, true, true, siirrot);
}


void Asema::etsi_nappula(int nappula, int& rivi, int& linja) const {
	for (int x = 0; x < 8; ++x) {
		for (int y = 0; y < 8; ++y) {
			// Tyhj‰ ruutu?
			if (_lauta[x][y] == nappula) {
				rivi = x;
				linja = y;
				break;
			}
				
		}
	}
}

bool Asema::onko_ruutu_uhattu(int rivi, int linja, int uhkaava_pelaaaja) const{
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