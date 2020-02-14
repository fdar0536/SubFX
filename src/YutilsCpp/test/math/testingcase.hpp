#pragma once

#include "../../YutilsCpp.hpp"

class TestMath
{
public:

    static TestMath *create();

    int testArcCurve();

    int testBezier();

    int testDegree();

    int testDistance();

    int testLineIntersect();

    int testOrtho();

    int testRandomsteps();

    int testRound();

    int testStretch();

    int testTrim();

    int testEllipse();

    int testRandomway();

    int testRotate();

private:

    TestMath() {}

    std::shared_ptr<Yutils::Math> m_math;
};
