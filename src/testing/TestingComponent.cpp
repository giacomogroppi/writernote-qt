#include "utils/common_script.h"
#include "core/wline.h"


#ifdef DEBUGINFO

static void test1_WLine(void)
{
    qDebug() << "\nRun test 1";

    W_ASSERT(
        WLine::intersect(WLine(1, 140, 70, 47), WLine(45, 90, 100, 120), 10, NULL)
    );
}

static void test2_WLine(void)
{
    qDebug() << "\nRun test 2";

    WLine vert(100., 100., 100., 1000.);
    WLine tmp1(50., 50., 110., 150.);

    W_ASSERT(
        WLine::intersect(vert, tmp1, 0, NULL)
    );
}

static void test3_WLine(void)
{
    qDebug() << "\nRun test 3";

    WLine vert(100., 100., 100., 1000.);
    WLine tmp1(50., 50., 90., 150.);

    W_ASSERT(
        !WLine::intersect(vert, tmp1, 0, NULL)
    );
}

void __init__ testingWLine(void)
{
    WLine line1(QPointF(100., 100.), QPointF(100., 200.));
    WLine line2(QPointF(95., 90.), QPointF(101., 175.));
    WLine line3(QPointF(95., 110.), QPointF(101., 110.));

    W_ASSERT(WLine::intersect(line1, line2, 0.));
    W_ASSERT(WLine::intersect(line1, line2, 0.));

    W_ASSERT(WLine::intersect(line2, line1, 0.));
    W_ASSERT(WLine::intersect(line3, line1, 0.));
    test1_WLine();
    test2_WLine();
    test3_WLine();


    qDebug() << "\n";
}

void __init__ testingBet(void)
{
    W_ASSERT(is_between(0., 5., 6.));
    W_ASSERT(!is_between(10., 5., 6.));
    W_ASSERT(is_between(5., 5., 6.));
}

#endif //DEBUGINFO
