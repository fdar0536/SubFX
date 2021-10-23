#include "global.h"

static fDSA fdsa;

bool globalInit()
{
    if (fdsa_init(&fdsa) == fdsa_failed) return false;

    return true;
}

fDSA *getFDSA()
{
    return &fdsa;
}
