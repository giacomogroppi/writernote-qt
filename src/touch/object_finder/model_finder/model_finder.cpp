#include "model_finder.h"
#include "touch/dataTouch/stroke/StrokePre.h"
#include "touch/dataTouch/stroke/StrokeCircleGenerator.h"
#include "touch/dataTouch/stroke/StrokeLineGenerator.h"
#include "touch/dataTouch/stroke/StrokeRectGenerator.h"
#include "Scheduler/Scheduler.h"

#define THREAD_FINDER 3

constexpr bool debug_model = true;
constexpr double _min_precision = 30;

static struct{
    double is[THREAD_FINDER];
} finder;

double (*functions[])(const WListFast<PointF>& points, const WVector<pressure_t>& pressures, const RectF& area) = {
        &StrokeLineGenerator    ::model_near,
        &StrokeRectGenerator    ::model_near,
        &StrokeCircleGenerator  ::model_near
};

UniquePtr<Stroke> (*function_create[])(
            const WListFast<PointF>& points,
            const WVector<pressure_t>& pressures,
            const RectF& area
        ) = {
        &StrokeLineGenerator    ::make,
        &StrokeRectGenerator    ::make,
        &StrokeCircleGenerator  ::make
};

class ArgsFinder
{
public:
    const WListFast<PointF> &points;
    const WListFast<pressure_t> &pressures;
    const RectF &rect;

    ArgsFinder(
                const WListFast<PointF>& points,
                const WListFast<pressure_t>& pressures,
                const RectF& rect
            )
            : points(points)
            , pressures(pressures)
            , rect (rect) {}
};

void __init__ init_finder()
{
    int i;

    for (i = 0; i < THREAD_FINDER; i++) {
        finder.is[i] = _min_precision * 2.;
    }
}

static int get_index_most_prob(cdouble min_precision)
{
    double last_precision = min_precision * 2.;
    int index = -1;
    int i;

    for (i = 0; i < THREAD_FINDER; i++) {
        const double prec = finder.is[i];

        // is not enough
        if (prec > min_precision){
            WDebug(debug_model, __FUNCTION__ << "not enough" << prec);
            continue;
        }

        if (prec < last_precision){
            index = i;
            last_precision = prec;
        }
    }

    return index;
}

auto model::find(const WListFast<PointF> &points, const WVector<pressure_t> &pressures, const RectF &area) -> UniquePtr<Stroke>
{
    std::unique_ptr<Stroke> res;

    W_ASSERT(!points.isEmpty());

    WVector<Scheduler::Ptr<WTask>> tasks;

    auto method = [=] (int i) {
        auto function = functions[i];
        finder.is[i] = function(points, pressures, area);
    };

    for (int i = 0; i < THREAD_FINDER; i++) {
        auto task = Scheduler::Ptr<WTask>(new WTaskFunction(nullptr, false, std::bind(method, i)));
        tasks.append(task);
        Scheduler::getInstance().addTaskGeneric(std::move(task));
    }

    tasks.forAll(&WTask::join);
    tasks.forAll(&Scheduler::Ptr<WTask>::release);

    const auto index = get_index_most_prob(_min_precision);
    WDebug(debug_model, index);

    if (index < 0) {
        return nullptr;
    }

    W_ASSERT(index < THREAD_FINDER);

    res = function_create[index](points, pressures, area);

    W_ASSERT(res != nullptr);

    return res;
}
