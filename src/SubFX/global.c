#include "global.h"

static fDSA *fdsa = NULL;

bool globalInit()
{
    if (fdsa) return true;

    fdsa = fdsa_init();
    if (!fdsa) return false;

    return true;
}

void globalFin()
{
    if (fdsa) fdsa_destroy(fdsa);
    fdsa = NULL;
}

fDSA *getFDSA()
{
    return fdsa;
}
