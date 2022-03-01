#ifndef WLINE_H
#define WLINE_H

#include "utils/common_script.h"
#include <QPointF>
#include <QPoint>
#include <QRectF>

class WLine
{
private:
    double _m, _p;
    double _xt, _yt;
    double _xb, _yb;
public:
    WLine() = default;
    WLine(const WLine &line);
    WLine(const QPointF &topLeft, const QPointF &bottomRigth);

    bool intersect(const WLine &line, cint precision);
    bool is_in_domain(const QPointF& point, cdouble precision) const;
    QRectF toRect() const;

    WLine &operator=(const WLine &other);
};

force_inline QRectF WLine::toRect() const
{
    return QRectF(QPointF(_xt, _yt), QPointF(_xb, _yb));
}

force_inline WLine &WLine::operator=(const WLine &other)
{
    if(unlikely(&other == this)){
        return *this;
    }
    memcpy(this, &other, sizeof(*this));

    return *this;
}

#endif // WLINE_H
