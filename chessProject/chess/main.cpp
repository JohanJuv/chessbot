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
    asema.tulosta();
    asema.anna_siirrot(siirrot);

    for (Siirto siirto : siirrot)
    {
        siirto.print_siirto();
    }
    siirrot.clear();
    return 0;
}
