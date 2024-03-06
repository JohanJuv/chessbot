#include <cassert>
#include <map>
#include <iostream>
#include <string>
#include "siirto.h"
using namespace std;

Siirto::Siirto(const std::string& s)
{
	_a_r = 56 - (int)s[1];
	_a_l = (int)s[0] - 97;
	_l_r = 56 - (int)s[3];
	_l_l = (int)s[2] - 97;
	
}


wstring Siirto::muodosta_string_siirrosta()const
{
	wstring nappulat[] = {L"wR", L"wN", L"wB", L"wQ", L"wK", L"wP", L"bR", L"bN", L"bB", L"bQ", L"bK", L"bP", L"NA"};

	wstring liike = wstring(1, (wchar_t)(_a_l + 97))
		+ wstring(1, (wchar_t)(56 - _a_r))
		+ wstring(1, (wchar_t)(_l_l + 97))
		+ wstring(1, (wchar_t)(56 - _l_r));

	if (_korotettava_nappula != NA) {
		liike += L" Korotettava nappula: " + nappulat[_korotettava_nappula];
	}
	return liike;
}

void Siirto::print_siirto()const {
	wcout << muodosta_string_siirrosta() << endl;
}