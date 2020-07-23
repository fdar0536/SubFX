#include <iostream>
#include <cstdlib>
#include <ctime>

#include "testingcase.hpp"

#define TESTING(x) \
    if (x) \
    { \
        return 1; \
    }

int main()
{
    puts("Testing math.hpp");
#ifdef _WIN32
    srand(static_cast<unsigned int>(time(nullptr)));
#else
    srand(time(nullptr));
#endif // _WIN32

    TESTING(testArcCurve())
    TESTING(testBezier())
    TESTING(testDegree())
    TESTING(testDistance())
    TESTING(testLineIntersect())
    TESTING(testOrtho())
    TESTING(testRandomsteps())
    TESTING(testRound())
    TESTING(testStretch())
    TESTING(testTrim())
    TESTING(testEllipse())
    TESTING(testRandomway())
    TESTING(testRotate())

    std::cout << "All done!" << std::endl;
    return 0;
}
