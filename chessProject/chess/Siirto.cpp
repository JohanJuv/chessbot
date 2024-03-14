#include <cassert>
#include <map>
#include <iostream>
#include <string>
#include "siirto.h"
using namespace std;

Siirto::Siirto(const std::string& s, int vari)
{
	_a_r = 56 - (int)s[1];
	_a_l = (int)s[0] - 97;
	_l_r = 56 - (int)s[3];
	_l_l = (int)s[2] - 97;


	if ((char)s[4] != NULL) {
		char nappula = s[4];
		if (vari == VALKEA) {
			if (nappula == 'Q')
				_korotettava_nappula = wQ;
			else if (nappula == 'B')
				_korotettava_nappula = wB;
			else if (nappula == 'R')
				_korotettava_nappula = wR;
			else if (nappula == 'N')
				_korotettava_nappula = wN;
		}
		else {
			if (nappula == 'Q')
				_korotettava_nappula = bQ;
			else if (nappula == 'B')
				_korotettava_nappula = bB;
			else if (nappula == 'R')
				_korotettava_nappula = bR;
			else if (nappula == 'N')
				_korotettava_nappula = bN;
		}
	}
	else {
		_korotettava_nappula = NA;
	}

}


wstring Siirto::muodosta_string_siirrosta()const
{
	wstring nappulat[] = {L"R", L"N", L"B", L"Q", L"K", L"P", L"R", L"N", L"B", L"Q", L"K", L"P", L"NA"};

	wstring liike = wstring(1, (wchar_t)(_a_l + 97))
		+ wstring(1, (wchar_t)(56 - _a_r))
		+ wstring(1, (wchar_t)(_l_l + 97))
		+ wstring(1, (wchar_t)(56 - _l_r));

	if (_korotettava_nappula != NA) {
		liike += nappulat[_korotettava_nappula];
	}
	return liike;
}

void Siirto::print_siirto()const {
	wcout << muodosta_string_siirrosta() << endl;
}