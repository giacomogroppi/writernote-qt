#include "StrokeRect.h"
#include "core/WPainter/WPainter.h"
#include "touch/dataTouch/page/Page.h"
#include "utils/common_error_definition.h"
#include "touch/dataTouch/stroke/StrokeComplexCommon.h"
#include "StrokeNormal.h"

StrokeRect::StrokeRect()
    : Stroke()
{
}

int StrokeRect::load(WZipReaderSingle &reader)
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

int StrokeRect::type() const
{
    return Stroke::COMPLEX_RECT;
}

void StrokeRect::draw(WPainter &painter, cbool is_rubber, cint page, WPen &pen, cdouble prop) const
{
    painter.setPen(pen);

    set_press(pen, _data.press, prop, is_rubber, this->getColor(1.));

    const auto _topLeft     = Page::at_translation(
                Point(
                    _data.rect.topLeft()),
                    page
                ).toPointF(prop);

    const auto _bottomRight = Page::at_translation(
                Point(
                    _data.rect.bottomRight()),
                    page
                ).toPointF(prop);

    painter.drawRect(RectF(_topLeft, _bottomRight));
}

int StrokeRect::is_inside(const WLine &line, int from, int precision, cbool needToDeletePoint) const
{
    (void)(line);
    (void)(precision);
    (void)(needToDeletePoint);
    W_ASSERT(0);
}

bool StrokeRect::is_inside(const RectF &rect, double precision) const
{
    const auto r = RectF(
                rect.topLeft() - PointF(precision, precision),
                rect.bottomRight() + PointF(precision, precision)
                );
    return this->_data.rect.intersects(r);
}

void StrokeRect::append(const Point &point, pressure_t pressure)
{
    using namespace WCommonScript;
    const auto distanceTopLeft = distance(point, _data.rect.topLeft());
    const auto distanceBottomRight  = distance(point, _data.rect.bottomRight());
    const auto distanceTopRight  = distance(point, _data.rect.topRight());
    const auto distanceBottomLeft   = distance(point, _data.rect.bottomLeft());

    if(         distanceTopLeft < distanceTopRight and
                distanceTopLeft < distanceBottomLeft and
                distanceTopLeft < distanceBottomRight) {
        _data.rect.setTopLeft(point);
    } else if ( distanceTopRight < distanceTopLeft and
                distanceTopRight < distanceBottomRight and
                distanceTopRight < distanceBottomLeft) {
        _data.rect.setTopRight(point);
    } else if ( distanceBottomLeft < distanceTopLeft and
                distanceBottomLeft < distanceTopRight and
                distanceBottomLeft < distanceBottomRight) {
        _data.rect.setBottomLeft(point);
    } else if ( distanceBottomRight < distanceBottomLeft and
                distanceBottomRight < distanceTopLeft and
                distanceBottomRight < distanceTopRight) {
        _data.rect.setBottomRight(point);
    } else {
        W_ASSERT(0);
    }
}

void StrokeRect::scale(const PointF &offset)
{
    this->_data.rect = RectF(
                    _data.rect.topLeft() + offset,
                    _data.rect.bottomRight() + offset
                );
}

bool StrokeRect::operator==(const Stroke &other) const
{
    if(this == &other)
        return true;

    if (other.type() != this->type())
        return false;

    if (Stroke::operator!=(other))
        return false;

    return  this->_data == dynamic_cast<const StrokeRect &>(other)._data and
            Stroke::operator==(other);
}

bool StrokeRect::operator!=(const Stroke &other) const
{
    return !(*this == other);
}

size_t StrokeRect::createControll() const
{
    return 0;
}

void StrokeRect::adjust(double zoom)
{
    const PointF topLeft = _data.rect.topLeft() / zoom;
    const PointF bottomRight = _data.rect.bottomRight() / zoom;
    _data.rect = RectF(topLeft, bottomRight);
}

std::shared_ptr<Stroke> StrokeRect::clone() const
{
    std::shared_ptr<StrokeRect> res(new StrokeRect);

    res->_data = this->_data;
    res->setMetadata(this->getMetadata());

    return res;
}

int StrokeRect::how_much_decrese() const
{
    return 0;
}

std::shared_ptr<Stroke> StrokeRect::makeNormal() const
{
    std::shared_ptr<StrokeNormal> res(new StrokeNormal);

    res->append(this->_data.rect.topLeft(), this->_data.press);
    res->append(this->_data.rect.topRight(), this->_data.press);
    res->append(this->_data.rect.bottomRight(), this->_data.press);
    res->append(this->_data.rect.bottomLeft(), this->_data.press);

    Stroke::clone(*res);

    return res;
}

bool StrokeRect::isEmpty() const
{
    return false;
}

Rect StrokeRect::getBiggerPointInStroke() const
{
    return _data.rect.toRect();
}

bool StrokeRect::isInside(const RectF &rect) const
{
    return this->_data.rect.intersects(rect);
}

int StrokeRect::save(WZipWriterSingle &file) const
{
    const auto res = Stroke::save(file);

    if(res != OK)
        return res;

    static_assert(sizeof(PointF) == sizeof(double) * 2);

    file.write_object(this->_data);

    // 4 for alignment
    static_assert(sizeof(this->_data) == (sizeof(RectF) + sizeof(pressure_t) + 4));

    return OK;
}

size_t StrokeRect::getSizeInMemory() const
{
    return 0;
}

size_t StrokeRect::getSizeInFile() const
{
    static_assert(sizeof(StrokeComplexCommon::current_ver) == sizeof(uchar));
    static_assert(sizeof(this->_data) == (sizeof(PointF) * 2 + sizeof(pressure_t) + 4));

    return sizeof(StrokeComplexCommon::current_ver) +
           sizeof(this->_data);
}

void StrokeRect::decreasePrecision()
{
}
