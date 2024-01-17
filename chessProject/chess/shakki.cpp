#include "asema.h"
#include "siirto.cpp"

int main()
{
    Asema asema;
    
    //e2e4
    Siirto siirto1(6, 4, 4, 4);
    Siirto siirto("e2e4");
    asema.tee_siirto(siirto);

    return 0;
}
