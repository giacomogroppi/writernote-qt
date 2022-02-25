#include "model_finder.h"
#include "touch/datastruct/datastruct.h"
#include "touch/object_finder/model/model.h"

#define THREAD_FINDER 3

static struct{
    bool is[THREAD_FINDER];
} finder;

bool (*function[])(const stroke *) = {
    model_line,
    model_rect,
    model_circle
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
    const int index = (long)_index;
    auto __function = function[index];

    finder.is[index] = __function((const stroke *)ctrl._stroke);

    return NULL;
}

bool model::find(stroke *stroke)
{
    long i;

    ctrl._stroke = stroke;
    for(i = 0; i < THREAD_FINDER; i++){
        pthread_create(&ctrl._thread[i], NULL, model_finder, (void *)i);
    }

    for(i = 0; i < THREAD_FINDER; i++){
        pthread_join(ctrl._thread[i], NULL);
    }

    return false;
}
