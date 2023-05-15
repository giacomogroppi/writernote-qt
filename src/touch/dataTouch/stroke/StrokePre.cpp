#include "StrokePre.h"

#include <memory>
#include "core/core.h"
#include "touch/dataTouch/stroke/stroke_drawer.h"
#include "touch/dataTouch/stroke/StrokeNormal.h"
#include "touch/dataTouch/page/Page.h"

StrokePre::StrokePre()  :
    _img(1, true),
    _stroke(),
    _last_draw_point(nullptr),
    _last_draw_press(nullptr),
    _min(0., 0., false),
    _max(0., 0., false)
{
    _stroke = std::make_shared<StrokeNormal>();

    W_ASSERT(_stroke->isEmpty());
    //W_ASSERT(this->isImageEmpty());
    //W_ASSERT(QImage(100, 100, QImage::Format_ARGB32) == QImage(100, 100, QImage::Format_ARGB32));
}

StrokePre::~StrokePre() = default;

std::shared_ptr<Stroke> StrokePre::merge()
{
    using namespace WCommonScript;
    W_ASSERT(this->already_merge == false);
    W_ASSERT(_point.length() == _pressure.length());
    W_ASSERT(this->_stroke != nullptr);

#ifdef DEBUGINFO
    already_merge = true;
#endif // DEBUGINFO

    if (this->_stroke->type() != Stroke::COMPLEX_NORMAL) {
        W_ASSERT(this->_point.isEmpty());
        std::shared_ptr<Stroke> res = this->_stroke;
        this->_stroke = nullptr;
        return res;
    }

    W_ASSERT(_stroke->isEmpty());

    _stroke->preappend(_point.length());

    while (not _point.isEmpty()) {
        const auto *data_point = _point.get_first();
        const auto *data_press = _pressure.get_first();

        _stroke->append(*data_point, *data_press);

        WDelete(data_point);
        WDelete(data_press);
    }

    {
        std::shared_ptr<Stroke> res = this->_stroke;
        this->_stroke = nullptr;
        return res;
    }
}

void StrokePre::adjust(const PointF &delta)
{
    if (_stroke->isEmpty()) {
        for (auto &p : this->_point) {
            p -= delta;
        }
    } else {
        _stroke->scale(-delta);
    }
}

void StrokePre::setAlfaColor(int alfa)
{
    W_ASSERT(alfa >= 0 and alfa <= 255);
    _stroke->setAlfaColor(alfa);
}

void StrokePre::setTime(int time)
{
    W_ASSERT( time == -1 or time >= 0 );
    _stroke->setPositioneAudio(time);
}

Rect StrokePre::getBiggerPointInStroke() const
{
    /** TODO --> define a cache */
    const auto res = (_stroke->isEmpty()) ?
                    StrokeNormal::getBiggerPointInStroke(
                                                this->_point.constBegin(),
                                                this->_point.constEnd()) :
                    _stroke->getBiggerPointInStroke();

    return res;
}

Rect StrokePre::getFirstAndLast() const
{
    const auto &first = *_point.constBegin();
    const auto &last  = _point.last();

    return {
            first.toPoint(),
            last.toPoint()
    };
}

pressure_t StrokePre::getPressure() const
{
    W_ASSERT(!this->_pressure.isEmpty());
    return *this->_pressure.constBegin();
}

void StrokePre::reset_img()
{
    _img = WPixmap(1, false);
    _img.fill(Qt::transparent);
}

void StrokePre::setStrokeComplex(std::shared_ptr<Stroke> stroke)
{
    W_ASSERT(stroke->type() != Stroke::COMPLEX_NORMAL);
    this->_stroke = stroke;
    this->_img = WPixmap();

    this->_point.clear();
    this->_pressure.clear();
}

colore_s StrokePre::getColor(double division) const
{
    return _stroke->getColor(division);
}

StrokePre &StrokePre::operator=(const StrokePre &other)
{
    if (this == &other)
        return *this;

    _stroke = other._stroke->clone();

    _img = other._img;
    this->_point = other._point;
    this->_pressure = other._pressure;
    this->_max = other._max;
    this->_min = other._min;

#ifdef DEBUGINFO
    this->already_merge = other.already_merge;
#endif // DEBUGINFO

    W_ASSERT(*_stroke == *other._stroke);
    W_ASSERT(_img == other._img);
    W_ASSERT(_point == other._point);

    return *this;
}

void StrokePre::append(const Point &point, const pressure_t &press, WPen &_pen, double prop)
{
    const auto normal = (_stroke->type() == Stroke::COMPLEX_NORMAL);

    if (normal) {
        Define_PAINTER_p(painter, _img);
        Define_PEN(pen);

        _point.append(point);
        _pressure.append(press);

        painter.setPen(pen);

        if (un(_point.length() == 1)) {
            _last_draw_point = this->_point.constBegin();
            _last_draw_press = this->_pressure.constBegin();

            _max = _point.first();
            _max = true;
            _min = _max;
            _max_pressure = _pressure.last();
        } else {
            StrokeNormal::drawData<WList<Point>::const_iterator,
                    WList<pressure_t>::const_iterator> data = {
                .begin_point = this->get_last_point(),
                .end_point   = this->_point.constEnd(),
                .begin_press = this->get_last_press(),
                .end_press   = this->_pressure.constEnd(),
                .press_null  = false
            };

            if (this->_point.last().y() > this->_max.y())
                _max.setY(_point.last().y() + 1.);
            if (this->_point.last().x() > this->_max.x())
                _max.setX(_point.last().x() + 1.);

            if (this->_point.last().y() < this->_min.y())
                this->_min.setY(_point.last().y() - 1.);
            if (this->_point.last().x() < this->_min.x())
                this->_min.setX(_point.last().x() - 1.);

            if (_max_pressure < _pressure.last()) {
                _max_pressure = _pressure.last();
            }

            StrokeNormal::draw(painter, false, 0, pen, prop, this->_stroke->getColor(1.), data);

            this->_last_draw_press ++;
            this->_last_draw_point ++;
        }

        painter.end();

    } else {
        W_ASSERT(_point.isEmpty());
        W_ASSERT(_pressure.isEmpty());

        _stroke->append(point.toPointF(1.), press);
    }
}

inline bool StrokePre::isImageEmpty() const
{
    return this->_img == WPixmap(1, true);
}
