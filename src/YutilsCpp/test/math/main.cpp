#include <iostream>

#include "testingcase.hpp"

#define TESTING(x) \
    if (x) \
    { \
        return 1; \
    }

int main()
{
    puts("Testing math.hpp");

    TestMath *test(TestMath::create());
    if (!test)
    {
        std::cout << "Fail in initializing" << std::endl;
        return 1;
    }

    TESTING(test->testArcCurve())
    TESTING(test->testBezier())
    TESTING(test->testDegree())
    TESTING(test->testDistance())
    TESTING(test->testLineIntersect())
    TESTING(test->testOrtho())
    TESTING(test->testRandomsteps())
    TESTING(test->testRound())
    TESTING(test->testStretch())
    TESTING(test->testTrim())
    TESTING(test->testEllipse())
    TESTING(test->testRandomway())
    TESTING(test->testRotate())

    delete test;
    std::cout << "All done!" << std::endl;
    return 0;
}
