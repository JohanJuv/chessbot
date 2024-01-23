#include "asema.h"
#include <vector>

int main()
{
    Asema asema;
    asema.tulosta();
    //e2e4
    Siirto siirto1(6, 4, 4, 4);
    Siirto siirto("a1d4");
    asema.tee_siirto(siirto);
    asema.tulosta();

    std::vector<Siirto> siirrot;

    asema.anna_tornin_raakasiirrot(4, 3, 0, siirrot);

    return 0;
}
