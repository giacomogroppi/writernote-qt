#pragma once

#include "touch/tools/InsertTools.h"
#include "touch/dataTouch/datastruct/utils_datastruct.h"
#include "touch/dataTouch/stroke/StrokePre.h"

class LaserMethod: public InsertTools
{
public:
    LaserMethod(Fn<pressure_t(double)> getPress,
                Fn<void(const StrokePre &stroke)> appent_to, WPen &pen, WColor &color);

    virtual ~LaserMethod() = default;
    auto touchBegin(const PointF& point, double size, class Document &doc) -> UpdateEvent final;
    auto touchUpdate(const PointF& point, double size, class Document &doc) -> UpdateEvent final;
    auto touchEnd(const PointF& point, class Document &doc) -> UpdateEvent final;
private:
    /**
     * La funzione passata da costruttore deve essere in grado di gestire l'inserimento del
     * dato Stroke in qualche punto, non nella Page.
     * */
    Fn<void(const StrokePre &stroke)> _append_to;
    unsigned char getAlfa() const final;
};

