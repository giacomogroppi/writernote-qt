#include "model_finder.h"
#include "touch/dataTouch/stroke/StrokePre.h"
#include "touch/object_finder/model/model.h"
#include "touch/dataTouch/stroke/StrokeCircleGenerator.h"
#include "touch/dataTouch/stroke/StrokeLineGenerator.h"
#include "touch/dataTouch/stroke/StrokeRectGenerator.h"

#define THREAD_FINDER 3

constexpr bool debug_model = true;
constexpr double _min_precision = 30;

static struct{
    double is[THREAD_FINDER];
} finder;

double (*functions[])(const StrokePre &) = {
    &StrokeLineGenerator    ::model_near,
    &StrokeRectGenerator    ::model_near,
    &StrokeCircleGenerator  ::model_near
};

std::shared_ptr<Stroke> (*function_create[])(const StrokePre *) = {
    &StrokeLineGenerator    ::make,
    &StrokeRectGenerator    ::make,
    &StrokeCircleGenerator  ::make
};

static struct{
    pthread_t _thread[THREAD_FINDER];
    const StrokePre *_stroke;
} ctrl;

void __init__ init_finder()
{
    int i;

    for (i = 0; i < THREAD_FINDER; i++) {
        finder.is[i] = _min_precision * 2.;
    }
}

static void *model_finder(void *_index)
{
    const auto index = (intptr_t)(_index);
    auto function = functions[index];

    finder.is[index] = function(*ctrl._stroke);

    WDebug(debug_model, "index: " << index << finder.is[index]);

    return nullptr;
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

bool model::find(StrokePre &stroke)
{
    unsigned long i;
    const auto color = stroke.getColor();
    std::shared_ptr<Stroke> res;

    W_ASSERT(!stroke.isEmpty());

    if (!stroke._stroke->isEmpty())
        return false;

    ctrl._stroke = &stroke;
    for (i = 0; i < THREAD_FINDER; i++) {
        pthread_create(&ctrl._thread[i], nullptr, model_finder, (void *)i);
    }

    for (i = 0; i < THREAD_FINDER; i++) {
        pthread_join(ctrl._thread[i], nullptr);
    }

    const auto index = get_index_most_prob(_min_precision);
    WDebug(debug_model, index);

    if (index < 0){
        return false;
    }

    W_ASSERT(index < THREAD_FINDER);

    res = function_create[index](&stroke);

    W_ASSERT(res != nullptr);

    stroke.setStrokeComplex(res);

    stroke.setColor(color);

    return true;
}
