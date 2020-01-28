#include "assinitbg.hpp"


AssInitBg *AssInitBg::create(QObject *parent)
{
    AssInitBg *ret(new (std::nothrow) AssInitBg(parent));
    if (!ret)
    {
        return nullptr;
    }

    return ret;
}

void AssInitBg::run()
{}
