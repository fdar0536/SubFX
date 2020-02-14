#include "testingcase.hpp"

TestMath *TestMath::create()
{
    TestMath *ret(new (std::nothrow) TestMath());
    if (!ret)
    {
        return nullptr;
    }

    ret->m_math = Yutils::Math::create();
    if (ret->m_math == nullptr)
    {
        delete ret;
        return nullptr;
    }

    return ret;
}

int TestMath::testArcCurve()
{
    puts("Testing arc_curve");

    double testData[] = { 0., 720., 60. };
    std::string errMsg("");
    std::vector<std::pair<double, double>> ret;
    int i;
    size_t j;
    for (i = 0; i < 3; ++i)
    {
        ret = m_math->arc_curve(0., 1.0,
                                1.5, 2.0,
                                testData[i],
                                errMsg);
        if (i != 2)
        {
            if (errMsg.empty())
            {
                printf("Failed in arc_curve's case %d.",
                       i);
                return 1;
            }
        }
        else
        {
            if (!ret.size())
            {
                printf("Failed in arc_curve's case %d.",
                       i);
                return 1;
            }

            for (j = 0;
                 j < ret.size();
                 ++j)
            {
                printf("%lf, %lf\n",
                       ret.at(j).first,
                       ret.at(j).second);
            }
        } // end if (i != 2)

        ret.clear();
        errMsg.clear();
    } // end for (i = 0; i < 3; ++i)

    puts("arc_curve is pass");
    return 0;
} // end testArcCurve

#define TESTBEZIERINTERNAL(a, b, c, d, e) \
    input.push_back(std::make_tuple(b, c, d)); \
    m_math->bezier(a, input, 1, errMsg); \
    if (!errMsg.empty()) \
    { \
        printf("Fail in case %d\n", e); \
        return 1; \
    }

int TestMath::testBezier()
{
    // total 7 cases
    puts("Testing bezier");
    std::tuple<double, double, double> ret;
    std::vector<std::tuple<double, double, double>> input;
    std::string errMsg("");

    // case 1
    ret = m_math->bezier(1.2, input, 1, errMsg);
    if (errMsg.empty())
    {
        puts("Failed in case 1");
        return 1;
    }
    errMsg.clear();

    // case 2
    ret = m_math->bezier(0., input, 1, errMsg);
    if (errMsg.empty())
    {
        puts("Fail in case 2");
        return 1;
    }
    errMsg.clear();

    // case 3 to 6
    input.reserve(5);
    input.push_back(std::make_tuple(0.8, 0.6, 2.));
    TESTBEZIERINTERNAL(0.6,   0.99,  0.7,  0.3, 3)
    TESTBEZIERINTERNAL(0.39,  0.64, 0.88, 0.57, 4)
    TESTBEZIERINTERNAL(0.31,  0.48, 0.46, 0.79, 5)
    TESTBEZIERINTERNAL(0.24,   0.1, 0.89, 0.43, 6)

    puts("bezier is pass");
    return 0;
} // end testBezier

#undef TESTBEZIERINTERNAL

int TestMath::testDegree()
{
    puts("Testing degree");
    printf("%lf\n", m_math->degree(5., 6., 7., 8., 9., 10.));
    puts("degree is pass");
    return 0;
} // end testDegree

int TestMath::testDistance()
{
    puts("Testing distance");
    printf("%lf\n", m_math->distance(3., 4., 0.));
    puts("distance is pass");
    return 0;
} // end testDistance

int TestMath::testLineIntersect()
{
    puts("Testing line_intersect");
    std::string errMsg("");

    // 2 cases
    // case 1
    m_math->line_intersect(0., 0., 0., 0.,
                           0., 0., 0., 0., 1, errMsg);
    if (errMsg.empty())
    {
        puts("Fail in case 1");
        return 1;
    }
    errMsg.clear();

    // case 2
    m_math->line_intersect(1., 3., 5., 7.,
                           2., 8., 6., 12., 1, errMsg);
    if (!errMsg.empty())
    {
        puts("Fail in case 2");
        return 1;
    }

    puts("line_intersect is pass");
    return 0;
} // end testLineIntersect

int TestMath::testOrtho()
{
    puts("Testing ortho");

    m_math->ortho(5., 10., 15., 2., 4., 6.);

    puts("ortho is pass");
    return 0;
} // end testOrtho

int TestMath::testRandomsteps()
{
    puts("Testing randomsteps");
    std::string errMsg("");

    // 2 cases
    // case 1
    m_math->randomsteps(5., 3., -1., errMsg);
    if (errMsg.empty())
    {
        puts("Fail in case 1");
        return 1;
    }
    errMsg.clear();

    // case 2
    double ret = m_math->randomsteps(2., 3., 0.5, errMsg);
    if (!errMsg.empty())
    {
        puts("Fail in case 2");
        return 1;
    }

    printf("%lf\n", ret);

    puts("randomsteps is pass");
    return 0;
} // end testRandomsteps

int TestMath::testRound()
{
    puts("Testing Round");

    double ret = m_math->round(3.86456443435, 5.);
    printf("%lf\n", ret);

    ret = m_math->round(3.86456443435, 0.5);
    printf("%lf\n", ret);

    puts("Round is pass");
    return 0;
} // end testRound

int TestMath::testStretch()
{
    puts("Testing stretch");

    m_math->stretch(12., 126., 38., 75.);

    puts("stretch is pass");
    return 0;
} // end testStretch

int TestMath::testTrim()
{
    puts("Testing trim");
    std::string errMsg("");

    // 2 cases
    // case 1
    m_math->trim(3., 5., 2., errMsg);
    if (errMsg.empty())
    {
        puts("Fail in case 1");
        return 1;
    }
    errMsg.clear();

    // case 2
    double ret = m_math->trim(3., 0.5, 2., errMsg);
    if (!errMsg.empty())
    {
        puts("Fail in case 2");
        return 1;
    }

    printf("%lf\n", ret);

    puts("trim is pass");
    return 0;
} // end testTrim

int TestMath::testEllipse()
{
    puts("Testing ellipse");

    m_math->ellipse(2., 6., 3., 7., 5.);

    puts("ellipse is pass");
    return 0;
} // end testEllipse

int TestMath::testRandomway()
{
    puts("Testing randomway");
    int i;
    for (i = 0; i < 5; ++i)
    {
        printf("%lf\n", m_math->randomway());
    }

    puts("randomway is pass");
    return 0;
} // end testRandomway

int TestMath::testRotate()
{
    puts("Testing rotate");
    std::string errMsg("");
    std::tuple<double, double, double> input;

    // 5 cases
    // case 1
    m_math->rotate(input, "", 2., errMsg);
    if (errMsg.empty())
    {
        puts("Fail in case 1");
        return 1;
    }

    errMsg.clear();
    input = std::make_tuple(3., 4., 5.);

    // case 2 to 4
    const char strings[3][2] = {"x", "y", "z"};
    int i;
    for (i = 0; i < 3; ++i)
    {
        m_math->rotate(input, strings[i], 2., errMsg);
        if (!errMsg.empty())
        {
            printf("Fail in case %d\n", i + 2);
            return 1;
        }
    }

    puts("rotate is pass");
    return 0;
} // end testRotate