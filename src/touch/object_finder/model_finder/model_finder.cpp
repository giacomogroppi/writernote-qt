#include "model_finder.h"
#include "touch/datastruct/datastruct.h"
#include "touch/object_finder/model/model.h"
#include "utils/platform.h"

#define THREAD_FINDER 3

static struct{
    double is[THREAD_FINDER];
} finder;

double (*function[])(const stroke *) = {
    &model_line,
    &model_rect,
    &model_circle
};

void (*function_create[])(stroke *) = {
    &model_line_create,
    &model_rect_create,
    &model_circle_create
};

static struct{
    pthread_t _thread[THREAD_FINDER];
    stroke *_stroke;
} ctrl;

void __init__ init_finder(void)
{
    memset(&finder, 0, sizeof(finder));
}

static void *model_finder(void *_index)
{
    size_t index;

    static_assert(sizeof(index)  == sizeof(_index));

    index = reinterpret_cast<size_t>(_index);
    //const long index = (long)_index;
    auto __function = function[index];

    finder.is[index] = __function((const stroke *)ctrl._stroke);

    return NULL;
}

static int get_index_most_prob(cdouble min_precision)
{
    double last_precision = min_precision * 2.;
    int index = -1;
    int i;

    for(i = 0; i < THREAD_FINDER; i++){
        const double prec = finder.is[i];

        if(prec > min_precision)
            continue;

        if(prec < last_precision){
            index = i;
            last_precision = prec;
        }
    }

    return index;
}

bool model::find(stroke *stroke)
{
    long i;
    constexpr double min_precision = 5;

    ctrl._stroke = stroke;
    for(i = 0; i < THREAD_FINDER; i++){
        pthread_create(&ctrl._thread[i], NULL, model_finder, (void *)i);
    }

    for(i = 0; i < THREAD_FINDER; i++){
        pthread_join(ctrl._thread[i], NULL);
    }

    i = get_index_most_prob(min_precision);

    if(i < 0){
        return false;
    }

    function_create[i](stroke);

    return true;
}
