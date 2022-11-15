#include "StrokeNormal.h"

StrokeNormal::StrokeNormal()
{}

StrokeNormal::~StrokeNormal()
{}

int StrokeNormal::save(WZipWriterSingle &file) const
{

}

int StrokeNormal::load(WZipReaderSingle &reader, int version)
{

}

void StrokeNormal::append(const point_s &point, pressure_t pressure)
{
    _point.append(point);
    _pressure.append(pressure);
    this->modify();
}

void StrokeNormal::draw(QPainter &painter, cbool is_rubber, cint page, QPen &pen, cdouble prop) const
{
    W_ASSERT(0);
}

int StrokeNormal::is_inside(const WLine &rect, int from, int precision, cbool needToDeletePoint) const
{
    const point_s *p1, *p2;
    WLine tmp;

    int &i = from;

    W_ASSERT(from >= 0);

    const auto len = this->length();

    if(unlikely(!len))
        return -1;

    if(unlikely(i >= len))
        return -1;

    if(i == 0){
        const auto &ref = this->getBiggerPointInStroke();
        if(likely(!ref.intersects(rect.toRect().toRect())))
            return -1;
    }

    p1 = &_point.at(i);
    for(i++; i < len; i++){
        p2 = &_point.at(i);

        tmp = WLine(p1->toQPointF(1.), p2->toQPointF(1.));

        if(unlikely(WLine::intersect(tmp, rect, precision))){
            return i;
        }

        p1 = p2;
    }

    return -1;
}

size_t StrokeNormal::createControll() const
{
    size_t controll = 0;

    for(const auto &p : qAsConst(this->_point)){
        controll += WCommonScript::diff(p.x());
        controll += WCommonScript::diff(p.y());
    }

    for(const auto &p : qAsConst(this->_pressure)){
        controll += WCommonScript::diff(p);
    }

    return controll + Stroke::createControll();
}

QRect StrokeNormal::getBiggerPointInStroke() const
{
    if (likely(!needToUpdateBiggerData())) {
        return Stroke::getBiggerPointInStroke();
    }

    const auto b = Stroke::getBiggerPointInStroke(this->_point.constBegin(),
                                                  this->_point.constEnd(),
                                                  *this);

    this->setBiggerData(b);

    return b;
}

QRect StrokeNormal::getBiggerPointInStroke(QList<point_s>::const_iterator begin,
                                           QList<point_s>::const_iterator end,
                                           StrokePre s)
{
    QRect biggerData;

    if(unlikely(not s.is_normal())){
        biggerData = stroke_complex_bigger_data(&s);
        return biggerData;
    }

    if(unlikely(begin == end)){
        WWarning("Warning: Stroke empty");
        return QRect(0, 0, 0, 0);
    }

    QPoint topLeft = begin->toQPointF(1.).toPoint();
    QPoint bottomRight = topLeft;

    for(; begin != end; begin ++){
        const point_s &point = *begin;

        if(topLeft.x() > point.x())
            topLeft.setX(static_cast<int>(
                                 point.x()
                         ));

        if(topLeft.y() > point.y())
            topLeft.setY(static_cast<int>(
                                 point.y()
                         ));

        if(bottomRight.x() < point.x())
            bottomRight.setX(static_cast<int>(
                                     point.x())
            );

        if(bottomRight.y() < point.y())
            bottomRight.setY(static_cast<int>(
                                     point.y())
            );
    }

    W_ASSERT(topLeft.x() <= bottomRight.x());
    W_ASSERT(topLeft.y() <= bottomRight.y());

    biggerData = QRect(topLeft, bottomRight);

    return biggerData;
}
