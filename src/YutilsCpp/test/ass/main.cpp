#include <cstdio>

#include "YutilsCpp"

#define StringToColorAlpha(x, y) \
    x = ass->stringToColorAlpha(y); \
    printf("r: %d, g: %d, b: %d, a: %d\n", \
           std::get<0>(x), std::get<1>(x), \
           std::get<2>(x), std::get<3>(x));

#define ColorAlphaToString(x, y) \
    x = ass->colorAlphaToString(y); \
    puts(x.c_str()); \
    y.clear();

int main()
{
    puts("Testing ass.hpp ...");
    std::shared_ptr<PROJ_NAMESPACE::Yutils::Ass>
            ass(PROJ_NAMESPACE::Yutils::Ass::create());
    if (ass == nullptr)
    {
        puts("Fail in initializing");
        return 1;
    }

    std::string tmpString("0:00:51.97");
    uint64_t ms;
    try
    {
        ms = ass->stringToMs(tmpString);
    }
    catch (std::invalid_argument &)
    {
        puts("Fail in testing stringToMs");
        return 1;
    }

#ifdef _WIN32
    printf("case 1's result is: %lld\n", ms);
#else
    printf("case 1's result is: %ld\n", ms);
#endif

    tmpString = ass->msToString(ms);
    printf("case 2's result is: %s\n", tmpString.c_str());

    std::tuple<uint8_t, uint8_t, uint8_t, uint8_t> res;
    tmpString = "&H22&";
    StringToColorAlpha(res, tmpString)
    tmpString = "&Ha9b8c7&";
    StringToColorAlpha(res, tmpString)
    tmpString = "&HDA3255E6";
    StringToColorAlpha(res, tmpString)

    std::vector<uint8_t> input;
    input.reserve(4);
    input.push_back(49);
    ColorAlphaToString(tmpString, input)

    input.push_back(88);
    input.push_back(254);
    input.push_back(215);
    ColorAlphaToString(tmpString, input)

    input.push_back(143);
    input.push_back(35);
    input.push_back(78);
    input.push_back(236);
    ColorAlphaToString(tmpString, input)

    puts("All done!");
    return 0;
}
