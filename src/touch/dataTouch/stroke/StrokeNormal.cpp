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