#include "StrokeLine.h"
#include "touch/dataTouch/page/Page.h"
#include "core/WLine.h"
#include "utils/common_error_definition.h"
#include "touch/dataTouch/stroke/StrokePre.h"
#include "StrokeComplexCommon.h"
#include "StrokeNormal.h"
#include "touch/TabletUtils.h"

StrokeLine::StrokeLine()
    : Stroke()
    , _data()
{
}

void StrokeLine::draw(WPainter &painter, bool is_rubber, int page, double prop, const WColor &color) const
{
    WPen pen;

    pen.setWidthF(_data.press * (is_rubber ? deltaColorNull : 1.0) * prop);
    pen.setColor(color);

    painter.setPen(pen);

    const auto _topLeft     = Page::at_translation(this->_data.pt1, page) * prop;
    const auto _bottomRight = Page::at_translation(this->_data.pt2, page) * prop;

    painter.setPen(pen);
    painter.drawLine(_topLeft, _bottomRight);
}

void StrokeLine::draw(WPainter &painter,
                      bool is_rubber,
                      int page,
                      double prop) const
{
    return this->draw(painter, is_rubber, page, prop, getColor(1.0));
}

int StrokeLine::is_inside(const WLine &line, int from, int precision, cbool needToDeletePoint) const
{
    (void)(needToDeletePoint);

    W_ASSERT(from == 0);

    WLine _line(this->_data.pt1, this->_data.pt2);
    return WLine::intersect(_line, line, precision);
}

bool StrokeLine::is_inside(const RectF &area, double precision) const
{
    WLine lineOrizTop   (area.topLeft(),    area.topRight());
    WLine lineOrizBot   (area.bottomLeft(), area.bottomRight());
    WLine lineVertLeft  (area.topLeft(),    area.bottomLeft());
    WLine lineVertRig   (area.topRight(),   area.bottomRight());
    WLine _this         (_data.pt1, _data.pt2);

    // if the Square passed to the function contains one of the two points
    if (area.contains(_data.pt1)) {
        WDebug(debugLine, "Contains first line" << _data.pt1);
        return true;
    }
    if (area.contains(_data.pt2)) {
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

void StrokeLine::append(const PointF &point, pressure_t pressure)
{
    using namespace WUtils;
    const auto dist1 = distance_not_square(_data.pt1, point);
    const auto dist2 = distance_not_square(_data.pt2, point);

    (void)(pressure);

    if (dist1 > dist2) {
        _data.pt2 = point;
    } else {
        _data.pt1 = point;
    }
}

size_t StrokeLine::createControl() const
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

std::unique_ptr<Stroke> StrokeLine::clone() const
{
    std::unique_ptr<StrokeLine> res(new StrokeLine);

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
    PointF tmp;
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
    PointF point;

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

std::unique_ptr<Stroke> StrokeLine::makeNormal() const
{
    int from, to;
    std::unique_ptr<StrokeNormal> res(new StrokeNormal);

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

void StrokeLine::scale(const PointF &offset)
{
    this->_data.pt1 += offset;
    this->_data.pt2 += offset;
}

RectF StrokeLine::getBiggerPointInStroke() const
{
    return datastruct_rect(_data.pt1, _data.pt2);
}

bool StrokeLine::isInside(const RectF &rect) const
{
    if (!this->getBiggerPointInStroke().intersects(rect))
        return false;
    return this->is_inside(rect, 0.);
}

auto StrokeLine::save(WritableAbstract &writer) const -> int
{
    const auto res = Stroke::save(writer);

    if(res != OK)
        return res;

    if (auto err = PointF::write(writer, _data.pt1))
        return ERROR;

    if (auto err = PointF::write(writer, _data.pt2))
        return ERROR;

    if (auto err = pressure_t::write(writer, _data.press))
        return ERROR;

    return OK;
}

size_t StrokeLine::getSizeInMemory() const
{
    return 0;
}

int StrokeLine::load(WZipReaderSingle &reader)
{
    StrokeComplexCommon::current_ver ver;

    if (reader.readObject(ver) < 0) {
        return ERROR;
    }

    if(ver == 1) {
        if (reader.readObject(this->_data)) {
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
    static_assert(sizeof(StrokeComplexCommon::current_ver) == sizeof(unsigned char));
    static_assert(sizeof(this->_data) == (sizeof(PointF) * 2 + sizeof(pressure_t) + 4));

    return sizeof(StrokeComplexCommon::current_ver) +
           sizeof(this->_data);
}

auto StrokeLine::loadPtr(const VersionFileController &versionController,
                         ReadableAbstract &readable) -> WPair<Error, StrokeLine *>
{
    std::unique_ptr<StrokeLine> d(new StrokeLine);
    if (versionController.getVersionStrokeLine() != 0)
        return {Error::makeErrVersion(), nullptr};
    {
        auto [res, point] = PointF::load(versionController, readable);
        if (res)
            return {res, nullptr};
        d->_data.pt1 = std::move(point);
    }

    {
        auto [res, point] = PointF::load(versionController, readable);
        if (res)
            return {res, nullptr};
        d->_data.pt2 = std::move(point);
    }

    {
        auto [res, pressure] = pressure_t::load(versionController, readable);
        if (res)
            return {res, nullptr};
        d->_data.press = std::move(pressure);
    }

    return {Error::makeOk(), d.release()};
}

void StrokeLine::append (WListFast<PointF> &&points, WVector<pressure_t> &&pressures)
{
    W_ASSERT(points.size() == pressures.size());
    for (int i = 0; i < points.size(); i++) {
        this->append(points.at(i), pressures.at(i));
    }
}
