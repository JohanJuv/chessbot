#pragma once
#include <vector>
#include "shakki.h"
#include "siirto.h"


class Asema
{
public:



	// Tyhjentää laudan.
	void tyhjenna();

	// Tekee annetun siirron laudalla. Voidaan olettaa, että
	// siirto on laillinen.
	void tee_siirto(const Siirto& s);

	// Tulostaa lauta ascii-grafiikkana.
	void tulosta() const;

	// Etsii annetun nappulan rivin ja linjan
	void etsi_nappula(int nappula, int& rivi, int& linja)const;

	// SIirtogeneraattori
	// Tuottaa aseman kaikki lailliset siirrot.
	void anna_siirrot(std::vector<Siirto>& siirrot);

	void anna_linnoitukset(int pelaaja, std::vector<Siirto>& siirrot) const;
	

	// Generoidaan uhkaavan pelaaja raakasiirrot

	// Tutkitaan, onko jonkin raakasiirron loppukoordinaatit == rivi, linka
	// jos on, palauta true (muuten false)
	bool onko_ruutu_uhattu(int rivi, int linja, int uhkaava_pelaaaja) const;

	// Antaa aseman kaikki raakasiirrot
	void anna_kaikki_raakasiirrot(int pelaaja, std::vector<Siirto>& siirrot) const;

	// Antaa tornin raakasiirrot (shakeista ei välitetä).
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
	/// \param rivi			nappulan lähtörivi
	/// \param linja		nappulan lähtölinja
	/// \param rivi_muutos	rivien koordinaattimuutos (esim. -1 on "ylös")
	/// \param linja_muutos	linjojen koordinaattimuutos (esim. 1 on "oikea")
	/// \param pelaaja		nappulan väri (VALKEA, MUSTA)
	/// \param max_askeleet	maksimiaskeleet ko. suuntaan
	/// \param voi_lyoda	voiko nappula lyödä ko. suuntaan?
	/// \param pakko_lyoda	onko nappulan pakko lyödä liikkuessaan ko. suuntaan?
	/// \param siirrot		siirtovektori joka täytetään mahdollisilla raakasiirroilla
	///
	void anna_raakasiirrot_suunnassa(int rivi, int linja, int rivi_muutos, int linja_muutos,
		int pelaaja, int max_askeleet, bool voi_lyoda, bool pakko_lyoda,
		std::vector<Siirto>& siirrot) const;

	// Laudan nappulat. Indeksointi [rivi][linja], esim.
	//
	// [0][0] : vasen ylänurkka ("a8")
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
	//{ NA, NA, NA, NA, NA, NA, NA, NA },
	//{ NA, NA, NA, NA, NA, NA, NA, NA },
	//{ NA, NA, NA, NA, NA, NA, NA, NA },
	//{ NA, NA, NA, NA, NA, NA, NA, NA },
	//{ NA, NA, NA, NA, NA, bR, NA, NA },
	//{ NA, NA, NA, NA, NA, NA, NA, NA },
	//{ NA, NA, NA, NA, NA, NA, NA, NA },
	//{ NA, NA, NA, NA, wK, NA, NA, wR }
	//};

	int _siirtovuoro = VALKEA;

	// Kirjanpito siitä, onko kuningaas tai torni liikkunut.
	// Asetetaan tarpeen mukaan falseksi.
	bool _valkea_lyhyt_linna_sallittu = true;
	bool _valkea_pitka_linna_sallittu = true;
	bool _musta_lyhyt_linna_sallittu = true;
	bool _musta_pitka_linna_sallittu = true;

	int _kaksoisaskel_linjalla = -1;
};

