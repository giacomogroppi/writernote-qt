#pragma once

#include "utils/WCommonScript.h"
#include "touch/dataTouch/point.h"
#include "core/WZipWriterSingle.h"

#include <QPainter>

class StrokeComplex {
public:
    virtual void draw(QPainter &painter, QPen &pen, double zoom) = 0;
    virtual void save() = 0;
    virtual QRect bigger_data() = 0;
    [[nodiscard]] virtual bool cmp(const StrokeComplex &stroke) const = 0;
    virtual void adjust(cdouble zoom) = 0;
    [[nodiscard]] virtual size_t get_size_save() const = 0;
    [[nodiscard]] virtual int save(WZipWriterSingle &file) const = 0;
    virtual void append(const QPointF &point) = 0;
    [[nodiscard]] virtual bool is_inside(const class WLine &line, double precision) const = 0;
    [[nodiscard]] virtual bool is_inside(const QRectF &area, double precision) const = 0;
    virtual void make_normal(class Stroke *to) = 0;

protected:
    [[nodiscard]] virtual int get_type() const = 0;
};
