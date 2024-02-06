#include <iostream>
#include <string>
#include <vector>
#include "asema.h"
#include "siirto.h"
using namespace std;

int main()
{
    Asema asema;
    asema.tulosta();
     
    vector<Siirto> siirrot;
    //asema.anna_kaikki_raakasiirrot(asema._siirtovuoro, siirrot);

    //e2e4
    Siirto siirto(7, 3, 4, 3);
    //Siirto siirto("a1d4");
    asema.tee_siirto(siirto);
    asema.tulosta();
    asema.anna_kaikki_raakasiirrot(asema._siirtovuoro, siirrot);

    int x;
    int y;
    asema.etsi_nappula(wR, x, y);

    asema.anna_tornin_raakasiirrot(4, 3, 0, siirrot);
    return 0;
}
