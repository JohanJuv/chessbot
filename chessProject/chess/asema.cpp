#include "asema.h"
#include <Windows.h>
#include <iostream>
#include <string>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <stdio.h>

using namespace std;

Nappula* Asema::wk = new Nappula(L"\u2654", 0, wK);
Nappula* Asema::wq = new Nappula(L"\u2655", 0, wQ);
Nappula* Asema::wr = new Nappula(L"\u2656", 0, wR);
Nappula* Asema::wb = new Nappula(L"\u2657", 0, wB);
Nappula* Asema::wn = new Nappula(L"\u2658", 0, wN);
Nappula* Asema::wp = new Nappula(L"\u2659", 0, wP);

Nappula* Asema::bk = new Nappula(L"\u265A", 1, bK);
Nappula* Asema::bq = new Nappula(L"\u265B", 1, bQ);
Nappula* Asema::br = new Nappula(L"\u265C", 1, bR);
Nappula* Asema::bb = new Nappula(L"\u265D", 1, bB);
Nappula* Asema::bn = new Nappula(L"\u265E", 1, bN);
Nappula* Asema::bp = new Nappula(L"\u265F", 1, bP);


Asema::Asema() {
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			_lauta[i][j] = NULL;
		}
	}
	for (int i = 0; i < 8; i++) {
		_lauta[1][i] = bp;
	}
	for (int i = 0; i < 8; i++) {
		_lauta[6][i] = wp;
	}
	_lauta[7][0] = wr, _lauta[7][7] = wr;
	_lauta[7][1] = wn, _lauta[7][6] = wn;
	_lauta[7][2] = wb, _lauta[7][5] = wb;
	_lauta[7][3] = wq, _lauta[7][4] = wk;

	_lauta[0][0] = br, _lauta[0][7] = br;
	_lauta[0][1] = bn, _lauta[0][6] = bn;
	_lauta[0][2] = bb, _lauta[0][5] = bb;
	_lauta[0][3] = bq, _lauta[0][4] = bk;
}

void Asema::tyhjenna()
{
	for (int rivi = 0; rivi < 8; rivi++){ 
		for (int linja = 0; linja < 8; linja++) {
			_lauta[rivi][linja] = NULL;
		}
	}
}

// Tekee annetun siirron laudalla. Voidaan olettaa, että
// Siirto on laillinen
void Asema::tee_siirto(const Siirto& s)
{
	// Otetaan Nappula talteen, tyhjennetään alkuruutu, sijoitetaan Nappula loppuruutuun
	Nappula* temp = _lauta[s._a_r][s._a_l];
	_lauta[s._a_r][s._a_l] = NULL;
	_lauta[s._l_r][s._l_l] = temp;
}


void Asema::anna_tornin_raakasiirrot(int rivi, int linja, int pelaaja,
	vector<Siirto>& siirrot) const
{

	const int x_suunnat[] = { -1, 1, 0, 0 };
	const int y_suunnat[] = { 0, 0, 1, -1 };

	for (int i = 0; i < 4; i++)
	{
		int x_suunta = x_suunnat[i];
		int y_suunta = y_suunnat[i];
		int uusiRivi = rivi + x_suunta;
		int uusiLinja = linja + y_suunta;

		while (uusiRivi >= 0 && uusiRivi <= 7 && uusiLinja >= 0 && uusiLinja <= 7)
		{
			if (_lauta[uusiRivi][uusiLinja] == nullptr)
			{
				siirrot.push_back(Siirto(rivi, linja, uusiRivi, uusiLinja));
			}
			else if (_lauta[uusiRivi][uusiLinja]->getVari() != pelaaja)
			{
				siirrot.push_back(Siirto(rivi, linja, uusiLinja, uusiRivi));
				break;
			}
			else
			{
				break;
			}

			uusiRivi += x_suunta;
			uusiLinja += y_suunta;
		}
	}

}


void Asema::tulosta() const{
	_setmode(_fileno(stdout), _O_U16TEXT);


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
			if (_lauta[i][j] == NULL)
			{
				wcout << "  ";
			}
			else
			{
				wcout << _lauta[i][j]->getUnicode() << " ";
			}
		}
		wcout << "\n";

	}

	wcout << "   A B C D E F G H\n\n";

}