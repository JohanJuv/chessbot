#include <cassert>
#include <map>
#include "siirto.h"
using namespace std;

Siirto::Siirto(const std::string& s)
{
	_a_r = 56 - (unsigned int)s[1];
	_a_l = (unsigned int)s[0] - 97;
	_l_r = 56 - (unsigned int)s[3];
	_l_l = (unsigned int)s[2] - 97;
}