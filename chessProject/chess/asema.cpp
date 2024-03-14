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
#include <list>
#include <future>
#include <unordered_map>
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

float Asema::sotilaidenRakenneEvaluaatio() const{
	float sotilaanRakenneArvo = 0;
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			if (_lauta[i][j] == wP) {
				// Plussaa etenmisest‰
				if (i == 1) {
					sotilaanRakenneArvo += 10;
				}
				// Miinusta yksin j‰‰misest‰
				if ((j > 0 && _lauta[i][j - 1] != wP) && (j < 7 && _lauta[i][j + 1] != wP)) {
					sotilaanRakenneArvo -= 5;
				}
				// Miinusta p‰‰llekk‰isist‰
				for (int k = i + 1; k < 8; ++k) {
					if (_lauta[k][j] == wP) {
						sotilaanRakenneArvo -= 5;
						break;
					}
				}
			}
		}
	}
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			if (_lauta[i][j] == bP) {
				if (i == 6) {
					sotilaanRakenneArvo -= 10;
				}
				if ((j > 0 && _lauta[i][j - 1] != bP) && (j < 7 && _lauta[i][j + 1] != bP)) {
					sotilaanRakenneArvo += 5;
				}
				for (int k = i - 1; k >= 0; --k) {
					if (_lauta[k][j] == bP) {
						sotilaanRakenneArvo += 5;
						break;
					}
				}
			}
		}
	}
	return sotilaanRakenneArvo;
}

float Asema::kuninkaanTurvaEvaluaatio() const {
	float kuninkaanTurvaArvo = 0;

	int vKunkunRivi, vKunkunLinja;
	etsi_nappula(wK, vKunkunRivi, vKunkunLinja);
	int mKunkunRivi, mKunkunLinja;
	etsi_nappula(bK, mKunkunRivi, mKunkunLinja);

	// Miinusta jos sotilaÌta ei ole kuninkaan edess‰ 
	for (int i = vKunkunRivi + 1; i < 8; ++i) {
		if (_lauta[i][vKunkunLinja] == wP) {
			kuninkaanTurvaArvo += 5;
			break;
		}
	}
	for (int r = -1; r <= 1; ++r) {
		for (int l = -1; l <= 1; ++l) {
			int uusiRivi = vKunkunRivi + r;
			int uusiLinja = vKunkunLinja + l;
			if (on_rajojen_sisalla(uusiLinja) && on_rajojen_sisalla(uusiLinja) &&
				(r != 0 || l != 0) && _lauta[uusiRivi][uusiLinja] == wP) {
				kuninkaanTurvaArvo += 3;
			}
		}
	}

	for (int i = mKunkunRivi - 1; i >= 0; --i) {
		if (_lauta[i][mKunkunLinja] == bP) {
			kuninkaanTurvaArvo -= 5;
			break;
		}
	}
	for (int r = -1; r <= 1; ++r) {
		for (int l = -1; l <= 1; ++l) {
			int uusiRivi = mKunkunRivi + r;
			int uusiLinja = mKunkunLinja + l;
			if (on_rajojen_sisalla(uusiLinja) && on_rajojen_sisalla(uusiRivi) &&
				(r != 0 || l != 0) && _lauta[uusiRivi][uusiLinja] == bP) {
				kuninkaanTurvaArvo -= 3;
			}
		}
	}
	return kuninkaanTurvaArvo;
}


float Asema::evaluoi() const
{
	return 1.0f * materiaali() + 0.1f * mobiliteetti() + 0.1f * sotilaidenRakenneEvaluaatio();

	// TODO
	// t‰ydent‰k‰‰ halutessanne uusilla pisteytett‰vill‰ aseman piirteill‰
}

float Asema::laske_pelin_tila() const{
	int nappuloita = 0;
	for (int rivi = 0; rivi < 8; ++rivi) {
		for (int linja = 0; linja < 8; ++linja) {
			if (_lauta[rivi][linja] != NA)
				nappuloita++;
		}
	}
	// Game phase calculation (0: opening, 0.5: middlegame, 1: endgame)
	return min(1.0, max(0.0, 1.0 - nappuloita / 32.0));
}

int Asema::dynaaminenHakuSyvyys() const{
	float materiaaliArvo = materiaali();
	float pelinTila = laske_pelin_tila();

	int dynamicHaku = 2; // Default depth
	if (pelinTila < 0.5) {
		// Opening/Middlegame phase
		if (abs(materiaaliArvo) < 100) {
			dynamicHaku = 3;
		}
	}
	else {
		// Endgame phase
		dynamicHaku = 4; // Shallow search during endgame
	}
	return 2;
}

MinimaxArvo Asema::etsi_siirto() {
	constexpr float alpha = numeric_limits<float>::lowest();
	constexpr float beta = numeric_limits<float>::max();
	unordered_map<size_t, float> tTable;
	return minimax(dynaaminenHakuSyvyys(), alpha, beta, tTable);
}

MinimaxArvo Asema::minimax(int syvyys, float alpha, float beta, TranspositionTable& asemaTable)
{
	// Generoidaan aseman siirrot.
	vector<Siirto> siirrot;
	anna_siirrot(siirrot);


	// Rekursion kantatapaus 1:
	// peli on p‰‰ttynyt (ei yht‰‰n laillista siirtoa).
	if (siirrot.size() == 0)
	{
		return MinimaxArvo(pisteyta_lopputulos(), Siirto());
	}

	// Rekursion kantatapaus 2
	if (syvyys == 0)
	{
		pair<bool, float> tallennettuAsema = retrieveFromTranspositionTable(asemaTable);
		bool loydetty = tallennettuAsema.first;
		float asemanArvo = tallennettuAsema.second;
		if (loydetty) {
			return MinimaxArvo(asemanArvo, Siirto());
		}
		float evalArvo = evaluoi();
		storeInTranspositionTable(asemaTable, evalArvo);
		return MinimaxArvo(evalArvo, Siirto());
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
//		future<MinimaxArvo> fut = async(launch::async, &Asema::minimax, uusi, syvyys - 1, alpha, beta, asemaTable);
//		MinimaxArvo newz = fut.get();
		MinimaxArvo arvo = uusi.minimax(syvyys - 1, alpha, beta, asemaTable);
		if (arvo._arvo == NULL) {
			break;
		}

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

		// Alpha-beta prunaus, miksi tulee huonoja siirtoja?
		//if (_siirtovuoro == VALKEA) {
		//	alpha = max(alpha, arvo._arvo);
		//	if (beta <= alpha)
		//		break; // Beta karsinta
		//	if (arvo._arvo > paras_arvo) {
		//		paras_arvo = arvo._arvo;
		//		paras_siirto = s;
		//	}
		//}
		//else {
		//	beta = min(beta, arvo._arvo);
		//	if (beta <= alpha)
		//		break; // Alpha karsinta
		//	if (arvo._arvo < paras_arvo) {
		//		paras_arvo = arvo._arvo;
		//		paras_siirto = s;
		//	}
		//}
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
				siirrot.push_back(Siirto(7, 4, 7, 6));
			}
			if (_valkea_pitka_linna_sallittu && _lauta[7][1] == NA && _lauta[7][2] == NA && _lauta[7][3] == NA &&
				_lauta[7][6] == NA && !onko_ruutu_uhattu(7, 3, MUSTA)) {
				siirrot.push_back(Siirto(7, 4, 7, 2));
			}
		}
	}
	else {
		if (!onko_ruutu_uhattu(0, 4, VALKEA)) {
			if (_musta_lyhyt_linna_sallittu && _lauta[0][5] == NA && _lauta[0][6] == NA && !onko_ruutu_uhattu(0, 5, VALKEA)) {
				siirrot.push_back(Siirto(0, 4, 0, 6));
			}
			if (_musta_pitka_linna_sallittu && _lauta[0][1] == NA && _lauta[0][2] == NA && _lauta[0][3] == NA
				&& !onko_ruutu_uhattu(0, 3, VALKEA) && !onko_ruutu_uhattu(0, 2, VALKEA)) {
				siirrot.push_back(Siirto(0, 4, 0, 2));
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

	// P‰ivitet‰‰n aseman kaksoisaskel-rivi:
	// Jos nappula on sotilas, niin asetetaan ko. kaksoisaskelriviksi
	// Muuten -1
	if (nappula == wP || nappula == bP) {
		if (abs(s._a_r - s._l_r) == 2) {
			_kaksoisaskel_linjalla = s._a_l;
		}
		else {
			_kaksoisaskel_linjalla = -1;
		}
		if (s._a_l != s._l_l && _lauta[s._l_r][s._l_l] == NA) {
			_lauta[s._a_r][s._l_l] = NA;
		}
	}
	else {
		_kaksoisaskel_linjalla = -1;
	}

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

	if (s._korotettava_nappula != NA) {
		_lauta[s._l_r][s._l_l] = s._korotettava_nappula;
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
		if (rivi_nyt > 7 || rivi_nyt < 0 || linja_nyt > 7 || linja_nyt < 0)
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

bool Asema::on_rajojen_sisalla(int rivi_tai_linja) const{
	if (rivi_tai_linja > 7 || rivi_tai_linja < 0) {
		return false;
	}
	return true;
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
	int korotusRivi = pelaaja == VALKEA ? 0 : 7;
	if (rivi == alkuRivi) {
		anna_raakasiirrot_suunnassa(rivi, linja, suunta+suunta, 0, pelaaja, 1, false, false, siirrot);
	}else if (_kaksoisaskel_linjalla != -1 && rivi == enPassantRivi) {
		if (linja + 1 == _kaksoisaskel_linjalla) {
			anna_raakasiirrot_suunnassa(rivi, linja, suunta, 1, pelaaja, 1, false, false, siirrot);
			//siirrot.push_back(Siirto(rivi, linja, rivi + suunta, _kaksoisaskel_linjalla));
		}
		else if (linja - 1 == _kaksoisaskel_linjalla) {
			anna_raakasiirrot_suunnassa(rivi, linja, suunta, -1, pelaaja, 1, false, false, siirrot);
			//siirrot.push_back(Siirto(rivi, linja, rivi + suunta, _kaksoisaskel_linjalla));
		}
	}
	
	if (korotusRivi == (rivi + suunta)) {
		if (pelaaja == VALKEA) {
			if (_lauta[rivi + suunta][linja] == NA) {
				siirrot.push_back(Siirto(rivi, linja, rivi + suunta, linja, wQ));
				siirrot.push_back(Siirto(rivi, linja, rivi + suunta, linja, wB));
				siirrot.push_back(Siirto(rivi, linja, rivi + suunta, linja, wR));
				siirrot.push_back(Siirto(rivi, linja, rivi + suunta, linja, wN));
			}
			if (on_rajojen_sisalla(rivi + suunta) && on_rajojen_sisalla(linja + 1) 
				&& _lauta[rivi + suunta][linja + 1] != NA 
				&& nappulan_vari(_lauta[rivi + suunta][linja + 1]) == vastustaja(pelaaja)) {
				siirrot.push_back(Siirto(rivi, linja, rivi + suunta, linja + 1, wQ));
				siirrot.push_back(Siirto(rivi, linja, rivi + suunta, linja + 1, wB));
				siirrot.push_back(Siirto(rivi, linja, rivi + suunta, linja + 1, wR));
				siirrot.push_back(Siirto(rivi, linja, rivi + suunta, linja + 1, wN));
			}
			if (on_rajojen_sisalla(rivi + suunta) && on_rajojen_sisalla(linja - 1)
				&& _lauta[rivi + suunta][linja - 1] != NA 
				&& nappulan_vari(_lauta[rivi + suunta][linja - 1]) == vastustaja(pelaaja)) {
				siirrot.push_back(Siirto(rivi, linja, rivi + suunta, linja - 1, wQ));
				siirrot.push_back(Siirto(rivi, linja, rivi + suunta, linja - 1, wB));
				siirrot.push_back(Siirto(rivi, linja, rivi + suunta, linja - 1, wR));
				siirrot.push_back(Siirto(rivi, linja, rivi + suunta, linja - 1, wN));
			}
		}
		else {
			if (_lauta[rivi + suunta][linja] == NA) {
				siirrot.push_back(Siirto(rivi, linja, rivi + suunta, linja, bQ));
				siirrot.push_back(Siirto(rivi, linja, rivi + suunta, linja, bB));
				siirrot.push_back(Siirto(rivi, linja, rivi + suunta, linja, bR));
				siirrot.push_back(Siirto(rivi, linja, rivi + suunta, linja, bN));
			}
			if (on_rajojen_sisalla(rivi + suunta) && on_rajojen_sisalla(linja + 1)
				&& _lauta[rivi + suunta][linja + 1] != NA
				&& nappulan_vari(_lauta[rivi + suunta][linja + 1]) == vastustaja(pelaaja)) {
				siirrot.push_back(Siirto(rivi, linja, rivi + suunta, linja + 1, bQ));
				siirrot.push_back(Siirto(rivi, linja, rivi + suunta, linja + 1, bB));
				siirrot.push_back(Siirto(rivi, linja, rivi + suunta, linja + 1, bR));
				siirrot.push_back(Siirto(rivi, linja, rivi + suunta, linja + 1, bN));
			}
			if (on_rajojen_sisalla(rivi + suunta) && on_rajojen_sisalla(linja - 1)
				&& _lauta[rivi + suunta][linja - 1] != NA 
				&& nappulan_vari(_lauta[rivi + suunta][linja - 1]) == vastustaja(pelaaja)) {
				siirrot.push_back(Siirto(rivi, linja, rivi + suunta, linja - 1, bQ));
				siirrot.push_back(Siirto(rivi, linja, rivi + suunta, linja - 1, bB));
				siirrot.push_back(Siirto(rivi, linja, rivi + suunta, linja - 1, bR));
				siirrot.push_back(Siirto(rivi, linja, rivi + suunta, linja - 1, bN));
			}
		}

	}
	else {
		anna_raakasiirrot_suunnassa(rivi, linja, suunta, 0, pelaaja, 1, false, false, siirrot);
		anna_raakasiirrot_suunnassa(rivi, linja, suunta, 1, pelaaja, 1, true, true, siirrot);
		anna_raakasiirrot_suunnassa(rivi, linja, suunta, -1, pelaaja, 1, true, true, siirrot);
	}

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

size_t Asema::hashBoardState() const {
	size_t hash = 0;
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			hash ^= std::hash<int>{}(_lauta[i][j])+0x9e3779b9 + (hash << 6) + (hash >> 2);
		}
	}
	return hash;
}

void Asema::storeInTranspositionTable(TranspositionTable& table, float score) const {
	size_t hash = hashBoardState();
	table.insert(make_pair(hash, score));
}

bool Asema::isInTranspositionTable(const TranspositionTable& table) const {
	size_t hash = hashBoardState();
	return table.find(hash) != table.end();
}

pair<bool, float> Asema::retrieveFromTranspositionTable(const TranspositionTable& table) const {
	size_t hash = hashBoardState();
	auto it = table.find(hash);
	if (it != table.end()) {
		return { true, it->second };
	}
	return { false, 0 }; // Board state not found in the table
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