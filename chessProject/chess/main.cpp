#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "asema.h"
#include "siirto.h"
using namespace std;

int main()
{
    //Asema asema;
    //asema.tulosta();
    //vector<Siirto> siirrot;
    //asema.tulosta();
    //asema.anna_siirrot(siirrot);

    //for (Siirto siirto : siirrot)
    //{
    //    siirto.print_siirto();
    //}
    //siirrot.clear();

	// Alkuasema.
	Asema asema;

	vector<Siirto> siirrot;
	asema.anna_kaikki_raakasiirrot(asema._siirtovuoro, siirrot);

	while (siirrot.size() > 0)
	{
		asema.tulosta();

		// tulosta mahdolliset siirrot muodossa
		for (Siirto siirto : siirrot)
		{
			siirto.print_siirto();
		}

		// tulosta laillisten siirtojen lukum‰‰r‰, esim.
		wcout << siirrot.size() << endl;

		// TODO 3
		// kysy pelaajan siirto tyyliin "e2e4" (huom! jos siirto on korotus, niin esim. "a7a8Q").
		string siirto;
		wcout << "Anna siirto tyyliin \"e2e4\", jos on korotus, niin esim. \"a7a8Q\"" << endl;
		cin >> siirto;

		Siirto opponentSiirto = Siirto(siirto);
		auto it = find(siirrot.begin(), siirrot.end(), opponentSiirto);
		if (it == siirrot.end()) {
			wcout << "Siirtoa ei lˆytynyt siirtovektorista." << endl;
			continue;
		}

		asema.tee_siirto(opponentSiirto);

		siirrot.clear();
		asema.anna_kaikki_raakasiirrot(asema._siirtovuoro, siirrot);
	}
    return 0;
}
