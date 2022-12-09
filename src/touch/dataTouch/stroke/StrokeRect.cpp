#include "StrokeRect.h"
#include <QPainter>
#include "touch/dataTouch/page/Page.h"
#include "utils/common_error_definition.h"
#include "touch/dataTouch/stroke/StrokeComplexCommon.h"

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

void StrokeRect::draw(QPainter &painter, cbool is_rubber, cint page, QPen &pen, cdouble prop) const
{
    painter.setPen(pen);

    set_press(pen, _data.press, prop, is_rubber, this->getColor(1.));

    const auto _topLeft     = Page::at_translation(
                point_s(
                    _data.rect.topLeft()),
                    page
                ).toQPointF(prop);

    const auto _bottomRight = Page::at_translation(
                point_s(
                    _data.rect.bottomRight()),
                    page
                ).toQPointF(prop);

    painter.drawRect(QRectF(_topLeft, _bottomRight));
}

int StrokeRect::is_inside(const WLine &line, int from, int precision, cbool needToDeletePoint) const
{
    Q_UNUSED(line);
    Q_UNUSED(precision);
    Q_UNUSED(needToDeletePoint);
    W_ASSERT(0);
}

bool StrokeRect::is_inside(const QRectF &rect, double precision) const
{
    Q_UNUSED(rect);
    Q_UNUSED(precision);
    W_ASSERT(0);
}

void StrokeRect::append(const point_s &point, pressure_t pressure)
{
    W_ASSERT(0);
}

void StrokeRect::scale(const QPointF &offset)
{
    this->_data.rect = QRectF(
                    _data.rect.topLeft() + offset,
                    _data.rect.bottomRight() + offset
                );
}

bool StrokeRect::operator==(const Stroke &other) const
{
    if (other.type() != this->type())
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
    const QPointF topLeft = _data.rect.topLeft() / zoom;
    const QPointF bottomRight = _data.rect.bottomRight() / zoom;
    _data.rect = QRectF(topLeft, bottomRight);
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
    W_ASSERT(0);
    return nullptr;
}

bool StrokeRect::isEmpty() const
{
    W_ASSERT(0);
    return false;
}

QRect StrokeRect::getBiggerPointInStroke() const
{
    return _data.rect.toRect();
}

bool StrokeRect::isInside(const QRectF &rect) const
{
    return this->_data.rect.intersects(rect);
}

int StrokeRect::save(WZipWriterSingle &file) const
{
    Q_UNUSED(file);
    W_ASSERT(0);
}

size_t StrokeRect::getSizeInMemory() const
{
    return 0;
}

size_t StrokeRect::getSizeInFile() const
{
    W_ASSERT(0);
}

void StrokeRect::decreasePrecision()
{
}
