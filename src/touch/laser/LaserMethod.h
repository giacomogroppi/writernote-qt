#pragma once

#include "touch/tools/InsertTools.h"
#include "touch/dataTouch/datastruct/utils_datastruct.h"
#include "touch/dataTouch/stroke/StrokePre.h"

class LaserMethod: public InsertTools
{
public:
    LaserMethod(std::function<pressure_t(double)> getPress,
                std::function<void(const QPointF&)> objectMove,
                std::function<void(const StrokePre &stroke)> appent_to,
                QPen &pen, QColor &color);

    virtual ~LaserMethod() = default;
    int touchBegin(const QPointF& point, double size, class Document &doc) final;
    int touchUpdate(const QPointF& point, double size, class Document &doc) final;
    int touchEnd(const QPointF& point, class Document &doc) final;
private:
    /**
     * La funzione passata da costruttore deve essere in grado di gestire l'inserimento del
     * dato Stroke in qualche punto, non nella Page.
     * */
    std::function<void(const StrokePre &stroke)> _append_to;
    uchar getAlfa() const final;
};

