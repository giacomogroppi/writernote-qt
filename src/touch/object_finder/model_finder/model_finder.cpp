#include "model_finder.h"
#include "touch/datastruct/datastruct.h"
#include "touch/object_finder/model/model.h"
#include "utils/platform.h"

#define THREAD_FINDER 3

constexpr bool debug_model = false;
constexpr double min_precision = 30;

static struct{
    double is[THREAD_FINDER];
} finder;

double (*function[])(const Stroke *) = {
    &model_line,
    &model_rect,
    &model_circle
};

void (*function_create[])(Stroke *) = {
    &model_line_create,
    &model_rect_create,
    &model_circle_create
};

static struct{
    pthread_t _thread[THREAD_FINDER];
    Stroke *_stroke;
} ctrl;

void __init__ init_finder(void)
{
    int i;

    for(i = 0; i < THREAD_FINDER; i++){
        finder.is[i] = min_precision * 2.;
    }
}

static void *model_finder(void *_index)
{
    size_t index;

    static_assert(sizeof(index)  == sizeof(_index));

    index = reinterpret_cast<size_t>(_index);
    //const long index = (long)_index;
    auto __function = function[index];

    //WDebug(debug_model, __FUNCTION__ << index);

    finder.is[index] = __function((const Stroke *)ctrl._stroke);

    return NULL;
}

static int get_index_most_prob(cdouble min_precision)
{
    double last_precision = min_precision * 2.;
    int index = -1;
    int i;

    for(i = 0; i < THREAD_FINDER; i++){
        const double prec = finder.is[i];

        // is not enough
        if(prec > min_precision){
            //WDebug(debug_model, __FUNCTION__ << "not enough" << prec);
            continue;
        }

        if(prec < last_precision){
            index = i;
            last_precision = prec;
        }
    }

    return index;
}

bool model::find(Stroke *stroke)
{
    long i;

    ctrl._stroke = stroke;
    for(i = 0; i < THREAD_FINDER; i++){
        pthread_create(&ctrl._thread[i], NULL, model_finder, (void *)i);
    }

    for(i = 0; i < THREAD_FINDER; i++){
        pthread_join(ctrl._thread[i], NULL);
    }

    i = get_index_most_prob(min_precision);
    WDebug(debug_model, i);

    if(i < 0){
        return false;
    }

    function_create[i](stroke);

    return true;
}
