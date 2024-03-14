#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "asema.h"
#include "siirto.h"
using namespace std;

int main()
{
	int valkea;
	wcout << "Pelaako valkeilla ihminen \"0\" vai tietokone \"1\"" << endl;
	cin >> valkea;
	int musta;
	wcout << "Pelaako mustilla ihminen \"0\" vai tietokone \"1\"" << endl;
	cin >> musta;

	Asema asema;
	vector<Asema> historia;
	vector<Siirto> siirrot;
	asema.anna_siirrot(siirrot);


	while (siirrot.size() > 0)
	{
		asema.tulosta();

		if ((asema._siirtovuoro == VALKEA && valkea == 0 )|| (asema._siirtovuoro == MUSTA && musta == 0)) {

			// TODO 3
			// kysy pelaajan siirto tyyliin "e2e4" (huom! jos siirto on korotus, niin esim. "a7a8Q").
			string siirto;
			wcout << "Anna siirto tyyliin \"e2e4\", jos on korotus, niin esim. \"a7a8Q\" \"undo\" = undo" << endl;
			cin >> siirto;
			if (siirto ==  "undo") {
				if (!historia.empty()) {
					asema = historia.back();
					historia.pop_back();
					siirrot.clear();
					asema.anna_siirrot(siirrot);
					continue;
				}
				else {
					wcout << "Siirtohistoria on tyhjä." << endl;
					continue;
				}
			}
			else if (siirto == "siirrot") {
				// tulostaa mahdolliset siirrot
				for (Siirto siirto : siirrot)
				{
					siirto.print_siirto();
				}
			}
			wcout << asema.evaluoi() << endl;
			Siirto opponentSiirto = Siirto(siirto, asema._siirtovuoro);
			auto it = find(siirrot.begin(), siirrot.end(), opponentSiirto);
			if (it == siirrot.end()) {
				wcout << "Siirtoa ei löytynyt siirtovektorista." << endl;
				continue;
			}
			historia.push_back(asema);
			asema.tee_siirto(opponentSiirto);

		}
		else {
			wcout << siirrot.size() << endl;
			//for (Siirto siirto : siirrot)
			//{
			//	siirto.print_siirto();
			//}
			MinimaxArvo mxArvo = asema.etsi_siirto();
			wcout << mxArvo._siirto.muodosta_string_siirrosta() << " arvo on: " << asema.evaluoi() << endl;
			asema.tee_siirto(mxArvo._siirto);
		}

		siirrot.clear();
		asema.anna_siirrot(siirrot);
	}
	asema.tulosta();

    return 0;
}
