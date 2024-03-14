#pragma once
#include <vector>
#include "shakki.h"
#include "siirto.h"
#include <chrono>
#include <unordered_map>


// Minimax-funktion palauttama arvo. Sis�lt�� sek�
// minimax-arvon ett� sit� vastaavan parhaan siirron.
class MinimaxArvo
{
public:
	MinimaxArvo(float arvo, Siirto siirto) :
		_arvo(arvo), _siirto(siirto)
	{}

	float	_arvo;
	Siirto	_siirto;
};


class Asema
{
public:
	using TranspositionTable = std::unordered_map<size_t, float>;

	std::size_t hashBoardState() const;

	void storeInTranspositionTable(TranspositionTable& table, float score) const; 

	bool isInTranspositionTable(const TranspositionTable& table) const; 

	std::pair<bool, float> retrieveFromTranspositionTable(const TranspositionTable& table) const;

	// Tyhjent�� laudan.
	void tyhjenna();

	// Tekee annetun siirron laudalla. Voidaan olettaa, ett�
	// siirto on laillinen.
	void tee_siirto(const Siirto& s);

	// Tulostaa lauta ascii-grafiikkana.
	void tulosta() const;

	// Etsii annetun nappulan rivin ja linjan
	void etsi_nappula(int nappula, int& rivi, int& linja)const;

	// SIirtogeneraattori
	// Tuottaa aseman kaikki lailliset siirrot.
	void anna_siirrot(std::vector<Siirto>& siirrot);
	
	// Pisteytt�� pelin lopputuloksen seuraavasti:
	//
	// Valkea tehnyt matin			1000000
	// Tasapeli (patti)				0
	// Musta tehnyt matin		   -1000000
	//
	// Funktiota kutsutaan, kun asemassa ei ole en�� yht��n laillista
	// siirtoa (anna_siirrot on palattanyt tyhj�n siirtovektorin).
	//
	float pisteyta_lopputulos() const;

	// Pisteytt�� shakkiaseman heuristisesti.
	float evaluoi() const;

	//
	// Palauttaa aseman minimax-arvon. Syvyys m��ritt��,
	// kuinka monta asekelta syvemm�lle pelipuuta k�yd��n l�pi.
	//
	// Testaaminen esim. p��ohjelmasta:
	//
	// Asema asema;
	// MinimaxArvo arvo = asema.minimax(4);
	// 
	// Nyt tietokoneen siirto saadaan pelattua n�in:
	// asema.tee_siirto(arvo._siirto);
	//
	MinimaxArvo minimax(int syvyys, float alpha, float beta, TranspositionTable& transpositionTable);

	MinimaxArvo etsi_siirto();

	// Laskee materiaalitasapainon (valkean nappuloiden arvo - mustan nappuloiden arvo).
	// Nappuloiden arvot:
	//
	// sotilas		1
	// ratsu		3
	// l�hetti		3
	// torni		5
	// daami		9
	//
	float materiaali() const;


	// Palauttaa valkean ja mustan (raaka)siirtojen lukum��rien erotuksen.
	float mobiliteetti() const;

	int dynaaminenHakuSyvyys() const;

	void anna_linnoitukset(int pelaaja, std::vector<Siirto>& siirrot) const;

	bool on_rajojen_sisalla(int rivi_tai_linja) const;
	
	float laske_pelin_tila() const;
	// Generoidaan uhkaavan pelaaja raakasiirrot

	// Tutkitaan, onko jonkin raakasiirron loppukoordinaatit == rivi, linka
	// jos on, palauta true (muuten false)
	bool onko_ruutu_uhattu(int rivi, int linja, int uhkaava_pelaaaja) const;

	// Antaa aseman kaikki raakasiirrot
	void anna_kaikki_raakasiirrot(int pelaaja, std::vector<Siirto>& siirrot) const;

	// Antaa tornin raakasiirrot (shakeista ei v�litet�).
	void anna_tornin_raakasiirrot(int rivi, int linja, int pelaaja,
		std::vector<Siirto>& siirrot) const;
	void anna_lahetin_raakasiirrot(int rivi, int linja, int pelaaja,
		std::vector<Siirto>& siirrot) const;
	void anna_daamin_raakasiirrot(int rivi, int linja, int pelaaja,
		std::vector<Siirto>& siirrot) const;
	void anna_ratsun_raakasiirrot(int rivi, int linja, int pelaaja,
		std::vector<Siirto>& siirrot) const;
	void anna_kuninkaan_raakasiirrot(int rivi, int linja, int pelaaja,
		std::vector<Siirto>& siirrot) const;
	void anna_sotilaan_raakasiirrot(int rivi, int linja, int pelaaja,
		std::vector<Siirto>& siirrot) const;

	/// Anna nappulan mahdolliset raakasiirrot tiettyyn suuntaan.
	///
	/// \param rivi			nappulan l�ht�rivi
	/// \param linja		nappulan l�ht�linja
	/// \param rivi_muutos	rivien koordinaattimuutos (esim. -1 on "yl�s")
	/// \param linja_muutos	linjojen koordinaattimuutos (esim. 1 on "oikea")
	/// \param pelaaja		nappulan v�ri (VALKEA, MUSTA)
	/// \param max_askeleet	maksimiaskeleet ko. suuntaan
	/// \param voi_lyoda	voiko nappula ly�d� ko. suuntaan?
	/// \param pakko_lyoda	onko nappulan pakko ly�d� liikkuessaan ko. suuntaan?
	/// \param siirrot		siirtovektori joka t�ytet��n mahdollisilla raakasiirroilla
	///
	void anna_raakasiirrot_suunnassa(int rivi, int linja, int rivi_muutos, int linja_muutos,
		int pelaaja, int max_askeleet, bool voi_lyoda, bool pakko_lyoda,
		std::vector<Siirto>& siirrot) const;

	// Laudan nappulat. Indeksointi [rivi][linja], esim.
	//
	// [0][0] : vasen yl�nurkka ("a8")
	// [7][0] : vasen alanurkka ("a1")
	// [7][7] : oikea alanurkka ("h1")
	//
	int _lauta[8][8] = {
		{ bR, bN, bB, bQ, bK, bB, bN, bR },
		{ bP, bP, bP, bP, bP, bP, bP, bP },
		{ NA, NA, NA, NA, NA, NA, NA, NA },
		{ NA, NA, NA, NA, NA, NA, NA, NA },
		{ NA, NA, NA, NA, NA, NA, NA, NA },
		{ NA, NA, NA, NA, NA, NA, NA, NA },
		{ wP, wP, wP, wP, wP, wP, wP, wP },
		{ wR, wN, wB, wQ, wK, wB, wN, wR }
	};
	// Testausta varten
	//int _lauta[8][8] = {
	//{ bR, NA, NA, NA, bK, NA, NA, bR },
	//{ bP, bP, bP, bP, bP, bP, bP, bP },
	//{ NA, NA, wP, NA, NA, NA, NA, NA },
	//{ wP, NA, NA, wP, NA, NA, NA, NA },
	//{ NA, NA, NA, NA, NA, NA, NA, NA },
	//{ NA, bP, NA, NA, NA, NA, NA, NA },
	//{ wP, wP, wP, wP, wP, wP, wP, wP },
	//{ NA, NA, NA, NA, wK, NA, NA, wR }
	//};

	float sotilaidenRakenneEvaluaatio() const;

	float kuninkaanTurvaEvaluaatio() const;

	int _siirtovuoro = VALKEA;

	// Kirjanpito siit�, onko kuningaas tai torni liikkunut.
	// Asetetaan tarpeen mukaan falseksi.
	bool _valkea_lyhyt_linna_sallittu = true;
	bool _valkea_pitka_linna_sallittu = true;
	bool _musta_lyhyt_linna_sallittu = true;
	bool _musta_pitka_linna_sallittu = true;

	int _kaksoisaskel_linjalla = -1;
};

