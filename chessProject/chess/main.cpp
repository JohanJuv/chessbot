#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "asema.h"
#include "siirto.h"
using namespace std;

int main()
{
	int siirtoVuoro;
	wcout << "Kirjoita 0 jos valkea ja 1 jos musta 1" << endl;
	cin >> siirtoVuoro;
	Asema asema;

	vector<Siirto> siirrot;
	asema.anna_siirrot(siirrot);


	while (siirrot.size() > 0)
	{
		asema.tulosta();

		// tulostaa mahdolliset siirrot
		for (Siirto siirto : siirrot)
		{
			siirto.print_siirto();
		}

		// tulosta laillisten siirtojen lukum‰‰r‰
		wcout << siirrot.size() << endl;

		if (siirtoVuoro == asema._siirtovuoro) {

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

		}
		else {
			MinimaxArvo mxArvo = asema.minimax(3);
			wcout << mxArvo._siirto.muodosta_string_siirrosta() << " arvo on: " << mxArvo._arvo << endl;
			asema.tee_siirto(mxArvo._siirto);
		}

		siirrot.clear();
		asema.anna_siirrot(siirrot);
	}
    return 0;
}
