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

int StrokeRect::type() const
{
    return Stroke::COMPLEX_RECT;
}

void StrokeRect::draw(WPainter &painter, bool is_rubber, int page, double prop, const WColor &color) const
{
    WPen pen;

    pen.setWidthF(_data.press * prop * (is_rubber ? deltaColorNull : 1.0));
    pen.setColor(color);

    painter.setPen(pen);

    const auto _topLeft     = Page::at_translation(
            PointF(
                    _data.rect.topLeft()),
            page
    ) * prop;

    const auto _bottomRight = Page::at_translation(
            PointF(
                    _data.rect.bottomRight()),
            page
    ) * prop;

    painter.drawRect(RectF(_topLeft, _bottomRight));
}

void StrokeRect::draw(WPainter &painter, bool is_rubber, int page, double prop) const
{
    return draw(painter, is_rubber, page, prop, getColor(1.0));
}

int StrokeRect::is_inside(const WLine &line, int from, int precision, cbool needToDeletePoint) const
{
    (void)(line);
    (void)(precision);
    (void)(needToDeletePoint);
    W_ASSERT(0);
    return -1;
}

bool StrokeRect::is_inside(const RectF &rect, double precision) const
{
    const auto r = RectF(
                rect.topLeft() - PointF(precision, precision),
                rect.bottomRight() + PointF(precision, precision)
                );
    return this->_data.rect.intersects(r);
}

void StrokeRect::append(const PointF &point, pressure_t pressure)
{
    using namespace WUtils;
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

size_t StrokeRect::createControl() const
{
    return 0;
}

void StrokeRect::adjust(double zoom)
{
    const PointF topLeft = _data.rect.topLeft() / zoom;
    const PointF bottomRight = _data.rect.bottomRight() / zoom;
    _data.rect = RectF(topLeft, bottomRight);
}

std::unique_ptr<Stroke> StrokeRect::clone() const
{
    std::unique_ptr<StrokeRect> res(new StrokeRect);

    res->_data = this->_data;
    res->setMetadata(this->getMetadata());

    return res;
}

int StrokeRect::how_much_decrese() const
{
    return 0;
}

std::unique_ptr<Stroke> StrokeRect::makeNormal() const
{
    std::unique_ptr<StrokeNormal> res(new StrokeNormal);

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

RectF StrokeRect::getBiggerPointInStroke() const
{
    return _data.rect;
}

bool StrokeRect::isInside(const RectF &rect) const
{
    return this->_data.rect.intersects(rect);
}

int StrokeRect::save(WritableAbstract &file) const
{
    const auto res = Stroke::save(file);

    if(res != OK)
        return res;

    if (auto err = RectF::write(file, this->_data.rect))
        return ERROR;

    if (auto err = pressure_t::write(file, this->_data.press))
        return ERROR;

    return OK;
}

auto StrokeRect::getSizeInMemory() const -> size_t
{
    return 0;
}

auto StrokeRect::getSizeInFile() const -> size_t
{
    static_assert(sizeof(StrokeComplexCommon::current_ver) == sizeof(unsigned char));
    static_assert(sizeof(this->_data) == (sizeof(PointF) * 2 + sizeof(pressure_t) + 4));

    return sizeof(StrokeComplexCommon::current_ver) +
           sizeof(this->_data);
}

void StrokeRect::decreasePrecision()
{
}

auto StrokeRect::loadPtr(const VersionFileController &versionController,
                         ReadableAbstract &readable) -> WPair<Error, StrokeRect *>
{
    std::unique_ptr<StrokeRect> d(new StrokeRect);

    if (versionController.getVersionStrokeRect() != 0)
        return {Error::makeErrVersion(), nullptr};

    {
        auto [res, data] = RectF::load(versionController, readable);
        if (res)
            return {res, nullptr};
        d->_data.rect = data;
    }

    {
        auto [res, data] = pressure_t::load(versionController, readable);
        if (res)
            return {res, nullptr};
        d->_data.press = data;
    }

    return {Error::makeOk(), d.release()};
}

void StrokeRect::append (WListFast<PointF> &&points, WVector<pressure_t> &&pressures)
{
    W_ASSERT(points.size() == pressures.size());
    for (int i = 0; i < points.size(); i++) {
        this->append(points.at(i), pressures.at(i));
    }
}
