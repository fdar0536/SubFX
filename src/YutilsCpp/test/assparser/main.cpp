#include <iostream>

#include "assdumper.hpp"

int main()
{
    auto dumper(AssDumper::create());
    if (dumper == nullptr)
    {
        return 1;
    }

    dumper->dumpMeta();
    dumper->dumpStyles();
    dumper->dumpDialogs();

    return 0;
}
