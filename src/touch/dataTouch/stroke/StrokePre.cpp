#include "StrokePre.h"

#include <memory>
#include "core/core.h"
#include "touch/dataTouch/stroke/stroke_drawer.h"
#include "touch/dataTouch/stroke/StrokeNormal.h"
#include "touch/dataTouch/page/Page.h"

StrokePre::StrokePre(Fn<void()> callUpdate)
    : WObject(nullptr)
    , _img(1, true)
    , _stroke(new StrokeNormal)
    , _timer(new WTimer(this, this->_timerEndLambda, _timerTime))
    , _callUpdate(std::move(callUpdate))
    , _last_draw_point(_point.constBegin())
    , _last_draw_press(_pressure.constBegin())
    , _max_pressure(0.)
    , _min({0., 0.}, false)
    , _max({0., 0.}, false)
{
    _timer->setSingleShot(timerIsSingleShot);
    W_ASSERT(_stroke->isEmpty());
    //W_ASSERT(this->isImageEmpty());
    //W_ASSERT(QImage(100, 100, QImage::Format_ARGB32) == QImage(100, 100, QImage::Format_ARGB32));
}

StrokePre::~StrokePre() noexcept = default;

void StrokePre::timerEnd()
{
    if (isEmpty() or _stroke->type() != Stroke::COMPLEX_NORMAL)
        return;

    UniquePtr<Stroke> stroke = model::find(_point, _pressure, getBiggerPointInStroke());

    if (stroke != nullptr) {
        this->_stroke = std::move(stroke);

        this->_stroke = std::move(stroke);
        this->_img = WPixmap();

        this->_point.clear();
        this->_pressure.clear();

        _callUpdate();
    }
}

auto StrokePre::merge() -> UniquePtr<Stroke>
{
    using namespace WCommonScript;
    W_ASSERT(this->already_merge == false);
    W_ASSERT(_point.size() == _pressure.size());
    W_ASSERT(this->_stroke != nullptr);

#ifdef DEBUGINFO
    already_merge = true;
#endif // DEBUGINFO

    if (this->_stroke->type() != Stroke::COMPLEX_NORMAL) {
        W_ASSERT(this->_point.isEmpty());
        UniquePtr<Stroke> res = std::move(this->_stroke);

        W_ASSERT(_stroke == nullptr);

        return res;
    }

    W_ASSERT(_stroke->isEmpty());

    _stroke->append(std::move(_point), std::move(_pressure));
    
    W_ASSERT(_point.isEmpty());
    W_ASSERT(_pressure.isEmpty());
    
    {
        UniquePtr<Stroke> res = std::move(this->_stroke);
        W_ASSERT(_stroke == nullptr);
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
        _stroke->scale(delta * -1.);
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
    _stroke->setPositionAudio(time);
}

auto StrokePre::getBiggerPointInStroke() const -> RectF
{
    // TODO: define a cache
    const auto res = (_stroke->isEmpty()) ?
                    StrokeNormal::getBiggerPointInStroke(
                                                this->_point.constBegin(),
                                                this->_point.constEnd()) :
                    _stroke->getBiggerPointInStroke();

    return res;
}

RectF StrokePre::getFirstAndLast() const
{
    const auto &first = *_point.constBegin();
    const auto &last  = _point.last();

    return {
            first,
            last
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
    _img.fill({color_transparent});
}

WColor StrokePre::getColor(double division) const
{
    return _stroke->getColor(division);
}

StrokePre &StrokePre::operator=(const StrokePre &other)
{
    if (this == &other)
        return *this;

    _stroke = other._stroke->clone();

    this->_img = other._img;
    this->_point = other._point;
    this->_pressure = other._pressure;
    this->_max = other._max;
    this->_min = other._min;

#ifdef DEBUGINFO
    this->already_merge = other.already_merge;
#endif // DEBUGINFO

    W_ASSERT(*_stroke == *other._stroke);
    W_ASSERT(_point == other._point);

    return *this;
}

StrokePre::StrokePre(const StrokePre &other) noexcept
    : WObject(nullptr)
    , _img(other._img)
    , _stroke(other._stroke->clone())
    , _timer(new WTimer(this, _timerEndLambda, _timerTime))
    , _point(other._point)
    , _pressure(other._pressure)
    , _last_draw_point(other._last_draw_point)
    , _last_draw_press(other._last_draw_press)
    , _max_pressure(other._max_pressure)
    , _min(other._min)
    , _max(other._max)
{
    _timer->setSingleShot(timerIsSingleShot);
}

StrokePre::StrokePre(StrokePre &&other) noexcept
    : WObject(nullptr)
    , _img(std::move(other._img))
    , _stroke(std::move(other._stroke))
    , _timer(new WTimer(this, this->_timerEndLambda, _timerTime))
    , _point(std::move(other._point))
    , _pressure(std::move(other._pressure))
    , _last_draw_point(other._last_draw_point)
    , _last_draw_press(other._last_draw_press)
    , _max_pressure(other._max_pressure)
    , _min(other._min)
    , _max(other._max)
{
    _timer->setSingleShot(timerIsSingleShot);
}

void StrokePre::append(const PointF &point, const pressure_t &press, double prop)
{
    const auto normal = (_stroke->type() == Stroke::COMPLEX_NORMAL);

    W_ASSERT(normal);
    
    if (normal) {
        WPainterUnsafe painter;
        WPen pen;
        
        painter.begin(&_img);

        _point.append(point);
        _pressure.append(press);

        painter.setPen(pen);
        painter.setAntialiasing();

        if (_point.size() == 1) {
            _last_draw_point = this->_point.constBegin();
            _last_draw_press = this->_pressure.constBegin();

            _max = _point.first();
            _max = true;
            _min = _max;
            _max_pressure = _pressure.last();
        } else {
            auto data = StrokeNormal::drawData<List<PointF>::const_iterator, List<pressure_t>::const_iterator> 
            (
                this->_last_draw_point,
                this->_point.constEnd(),
                this->_last_draw_press,
                false,
                _last_draw_point.operator long()
            );

            // TODO: consider pressure
            _max.setY(_point.last().y() > _max.y() ? _point.last().y() + 1. : _max.y());
            _max.setX(_point.last().x() > _max.x() ? _point.last().x() + 1. : _max.x());

            _min.setY(_point.last().y() < _min.y() ? _point.last().y() - 1. : _min.y());
            _min.setX(_point.last().x() < _min.x() ? _point.last().x() - 1. : _min.x());

            if (_max_pressure < _pressure.last()) {
                _max_pressure = _pressure.last();
            }

            StrokeNormal::draw(painter, false, 0, pen, prop, this->_stroke->getColor(1.), data);
            
            this->_last_draw_press ++;
            this->_last_draw_point ++;
        }

        painter.end();

        timerReset(point);
    } else {
        W_ASSERT(_point.isEmpty());
        W_ASSERT(_pressure.isEmpty());

        _stroke->append(point, press);
    }
}

inline bool StrokePre::isImageEmpty() const
{
    return this->_img == WPixmap(1, true);
}

void StrokePre::timerReset(const PointF &point) noexcept
{
    if(_timerPoint.isSet()){
        // if the point is equal we don't have to stop the timer
        if(WCommonScript::is_near(_timerPoint, point, 1.)){
            return;
        }

        // if the point is different we need to write it and restart the timer
        _timerPoint = point;
    }else{
        _timerPoint.set(true);
        _timerPoint = point;
    }

    if(_timer->isActive()){
        _timer->stop();
    }

    _timer->start(this->_timerTime);
}
