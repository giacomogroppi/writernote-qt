#include "StrokeLine.h"
#include "touch/dataTouch/page/Page.h"
#include "core/WLine.h"
#include "utils/common_error_definition.h"
#include "touch/dataTouch/stroke/StrokePre.h"
#include "StrokeComplexCommon.h"
#include "StrokeNormal.h"

StrokeLine::StrokeLine()
    : Stroke()
{
}

void StrokeLine::draw(QPainter &painter,
                      cbool is_rubber,
                      cint page,
                      QPen &pen,
                      cdouble prop) const
{
    const auto p = prop == PROP_RESOLUTION ? prop : 1.;

    painter.setPen(pen);
    set_press(pen, this->_data.press, p, is_rubber, this->getColor(1.));

    const auto _topLeft     = Page::at_translation(Point(this->_data.pt1), page).toQPointF(prop);
    const auto _bottomRight = Page::at_translation(Point(this->_data.pt2), page).toQPointF(prop);

    painter.setPen(pen);
    painter.drawLine(_topLeft, _bottomRight);
}

int StrokeLine::is_inside(const WLine &line, int from, int precision, cbool needToDeletePoint) const
{
    Q_UNUSED(needToDeletePoint);
    W_ASSERT(from == 0);
    WLine _line(this->_data.pt1, this->_data.pt2);
    return WLine::intersect(_line, line, precision);
}

bool StrokeLine::is_inside(const QRectF &area, double precision) const
{
    WLine lineOrizTop   (area.topLeft(),    area.topRight());
    WLine lineOrizBot   (area.bottomLeft(), area.bottomRight());
    WLine lineVertLeft  (area.topLeft(),    area.bottomLeft());
    WLine lineVertRig   (area.topRight(),   area.bottomRight());
    WLine _this         (_data.pt1, _data.pt2);

    // if the square passed to the function contains one of the two points
    if(area.contains(_data.pt1)){
        WDebug(debugLine, "Contains first line" << _data.pt1);
        return true;
    }
    if(area.contains(_data.pt2)){
        WDebug(debugLine, "contains second line" << _data.pt2);
        return true;
    }

    WDebug(debugLine, "intersect topLeft" << _data.pt1 << "bottomr" << _data.pt2 << "(rect)" << area);

    if(WLine::intersect(_this, lineOrizTop, precision)){
        WDebug(debugLine, "intersect 1");
        return true;
    }

    if(WLine::intersect(_this, lineOrizBot, precision)){
        WDebug(debugLine, "intersect 2")
        return true;
    }

    if(WLine::intersect(_this, lineVertLeft, precision)){
        WDebug(debugLine, "intersect 3");
        return true;
    }

    if(WLine::intersect(_this, lineVertRig, precision)){
        WDebug(debugLine, "intersect 4");
        return true;
    }

    return false;
}

void StrokeLine::append(const Point &point, pressure_t pressure)
{
    using namespace WCommonScript;
    const auto dist1 = distance_not_square(_data.pt1, point);
    const auto dist2 = distance_not_square(_data.pt2, point);

    Q_UNUSED(pressure);

    if(dist1 > dist2){
        _data.pt2 = point;
    }else{
        _data.pt1 = point;
    }
}

size_t StrokeLine::createControll() const
{
    return 0;
}

void StrokeLine::decreasePrecision()
{
}

void StrokeLine::adjust(double zoom)
{
    this->_data.pt1 /= zoom;
    this->_data.pt2 /= zoom;
    this->_data.press /= zoom;
}

std::shared_ptr<Stroke> StrokeLine::clone() const
{
    std::shared_ptr<StrokeLine> res(new StrokeLine);

    res->_data = this->_data;

    Stroke::clone(*res);

    return res;
}

int StrokeLine::how_much_decrese() const
{
    return 0;
}

void StrokeLine::makeNormalVertical(StrokeNormal *mergeTo, int from, int to) const
{
    Point tmp;
    W_ASSERT(_data.pt2.x() == _data.pt1.x());
    W_ASSERT(from <= to);

    tmp.rx() = _data.pt1.x();

    for(; from <= to; from ++){
        tmp.ry() = from;
        mergeTo->append(tmp, _data.press);
    }
}

void StrokeLine::makeNormalGeneric(StrokeNormal *mergeTo, int from, int to) const
{
    double m, p;
    Point point;

    W_ASSERT(from <= to);

    m =     (_data.pt1.y() - _data.pt2.y()) /
            (_data.pt1.x() - _data.pt2.x());

    p = _data.pt1.y() - _data.pt1.x() * m;

    for(; from <= to; from ++){
        const double x = (double(from) - p) / m;
        point.rx() = x;
        point.ry() = (double) from;
        mergeTo->append(point, _data.press);
    }
}

std::shared_ptr<Stroke> StrokeLine::makeNormal() const
{
    int from, to;
    std::shared_ptr<StrokeNormal> res(new StrokeNormal);

    W_ASSERT(res->isEmpty());

    from    = (int) _data.pt1.y();
    to      = (int) _data.pt2.y();

    W_ASSERT(from <= to);

    if(_data.pt2.x() == _data.pt1.x()){
        this->makeNormalVertical(res.get(), from, to);
    }else{
        this->makeNormalGeneric(res.get(), from, to);
    }

    return res;
}

bool StrokeLine::isEmpty() const
{
    return false;
}

void StrokeLine::scale(const QPointF &offset)
{
    this->_data.pt1 += offset;
    this->_data.pt2 += offset;
}

QRect StrokeLine::getBiggerPointInStroke() const
{
    return datastruct_rect(_data.pt1, _data.pt2).toRect();
}

bool StrokeLine::isInside(const QRectF &rect) const
{
    if (!this->getBiggerPointInStroke().intersects(rect.toRect()))
        return false;
    return this->is_inside(rect, 0.);
}

int StrokeLine::save(WZipWriterSingle &writer) const
{
    const auto res = Stroke::save(writer);

    if(res != OK)
        return res;

    static_assert(sizeof(QPointF) == sizeof(double) * 2);

    writer.write_object(this->_data);

    static_assert(sizeof(this->_data) == (sizeof(QPointF) * 2 + sizeof(float) + 4));

    return OK;
}

size_t StrokeLine::getSizeInMemory() const
{
    return 0;
}

int StrokeLine::load(WZipReaderSingle &reader)
{
    StrokeComplexCommon::current_ver ver;

    if (reader.read_object(ver) < 0) {
        return ERROR;
    }

    if(ver == 1) {
        if (reader.read_object(this->_data)) {
            return ERROR;
        }
    } else {
        return ERROR_VERSION_NEW;
    }

    return OK;
}

bool StrokeLine::operator==(const Stroke &other) const
{
    if(this == &other)
        return true;

    if(this->type() != other.type())
        return false;

    if (Stroke::operator!=(other))
        return false;

    const auto *s = dynamic_cast<const StrokeLine *>(&other);

    return  this->_data.pt1 == s->_data.pt1 and
            this->_data.pt2 == s->_data.pt2 and
            this->_data.press == s->_data.press;
}

bool StrokeLine::operator!=(const Stroke &other) const
{
    return !(*this == other);
}

int StrokeLine::type() const
{
    return Stroke::COMPLEX_LINE;
}

size_t StrokeLine::getSizeInFile() const
{
    static_assert(sizeof(StrokeComplexCommon::current_ver) == sizeof(uchar));
    static_assert(sizeof(this->_data) == (sizeof(QPointF) * 2 + sizeof(pressure_t) + 4));

    return sizeof(StrokeComplexCommon::current_ver) +
           sizeof(this->_data);
}
