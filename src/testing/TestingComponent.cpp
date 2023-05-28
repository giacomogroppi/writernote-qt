#include "utils/WCommonScript.h"
#include "core/WLine.h"


#ifdef DEBUGINFO

static void test1_WLine(void)
{
    WDebug(true, "Run test 1");

    W_ASSERT(
        WLine::intersect(WLine(1, 140, 70, 47), WLine(45, 90, 100, 120), 10, NULL)
    );
}

static void test2_WLine(void)
{
    WDebug(true, "Run test 1");

    WLine vert(100., 100., 100., 1000.);
    WLine tmp1(50., 50., 110., 150.);

    W_ASSERT(
        WLine::intersect(vert, tmp1, 0, NULL)
    );
}

static void test3_WLine(void)
{
    WDebug(true, "Run test 3");

    WLine vert(100., 100., 100., 1000.);
    WLine tmp1(50., 50., 90., 150.);

    W_ASSERT(
        !WLine::intersect(vert, tmp1, 0, NULL)
    );
}

void __init__ testingWLine(void)
{
    WLine line1(PointF(100., 100.), PointF(100., 200.));
    WLine line2(PointF(95., 90.), PointF(101., 175.));
    WLine line3(PointF(95., 110.), PointF(101., 110.));

    W_ASSERT(WLine::intersect(line1, line2, 0.));
    W_ASSERT(WLine::intersect(line1, line2, 0.));

    W_ASSERT(WLine::intersect(line2, line1, 0.));
    W_ASSERT(WLine::intersect(line3, line1, 0.));
    test1_WLine();
    test2_WLine();
    test3_WLine();
}

void __init__ testingBet(void)
{
    W_ASSERT(WCommonScript::is_between(0., 5., 6.));
    W_ASSERT(!WCommonScript::is_between(10., 5., 6.));
    W_ASSERT(WCommonScript::is_between(5., 5., 6.));
}

#endif //DEBUGINFO
