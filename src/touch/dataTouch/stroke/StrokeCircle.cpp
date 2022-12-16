#include "StrokeCircle.h"
#include "touch/dataTouch/page/Page.h"
#include "touch/dataTouch/stroke/StrokeComplexCommon.h"
#include "utils/common_error_definition.h"

int StrokeCircle::load(WZipReaderSingle &reader)
{
    W_ASSERT(0);
}

StrokeCircle::StrokeCircle()
{

}

double StrokeCircle::distanceFromCenter(const QPointF &point) const
{
    using namespace WCommonScript;
    return qSqrt(
        Power(_data.x - point.x(), 2) +
        Power(_data.y - point.y(), 2)
                );
}

int StrokeCircle::isInternal(double distance, double precision) const
{
    const auto raggio = qSqrt(_data.r);
    const auto real_dist1 = qSqrt(distance);
    return real_dist1 <= raggio + precision;
}

bool StrokeCircle::oneSide(double inside, double outside, double prec) const
{
    return  isInternal(inside, prec) and
            not isInternal(outside, prec);
}

void StrokeCircle::draw(QPainter &painter, cbool is_rubber, cint page, QPen &pen, cdouble prop_) const
{
    constexpr bool not_used debCircle = false;
    point_s point;
    QPointF tmp;
    const auto press = _data.press;
    double y, x;
    cdouble prop = prop_ == PROP_RESOLUTION ? prop_ : 1.;

    point = point_s(_data.x, _data.y);

    point = Page::at_translation(point, page);
    tmp = point.toQPointF(prop);

    WDebug(debCircle, "prop: " << prop);

    y = tmp.y();
    x = tmp.x();

    set_press(pen, press, prop, is_rubber, this->getColor(1.));
    painter.setPen(pen);

    painter.drawEllipse(QPointF(x, y), _data.r * prop, _data.r * prop);

    WDebug(debCircle, _data.x << _data.y << _data.r);
}

int StrokeCircle::is_inside(const WLine &line, int from, int precision, cbool needToDeletePoint) const
{
    QPointF tl, br;

    constexpr bool not_used debug = true;

    W_ASSERT(_data.x >= 0.);
    W_ASSERT(_data.r >= 0.);
    W_ASSERT(_data.y >= 0.);
    W_ASSERT(_data.press >= 0.);

    line.get_point(tl, br);

    cdouble distance1 = distanceFromCenter(tl);
    cdouble distance2 = distanceFromCenter(br);

    WDebug(debug, distance1 << distance2 << tl << br);

    if(     WCommonScript::is_near(distance1, _data.r, precision) or
            WCommonScript::is_near(distance2, _data.r, precision))
        return true;

    cbool res = oneSide(distance1, distance2, precision) ||
                oneSide(distance2, distance1, precision);

    return res;
}

/**
 * Questa funzione viene chiamata se e solo se
 * il biggerData dello stroke è all'interno
 * dell'area in cui stiamo cercando.
 */
bool StrokeCircle::is_inside(const QRectF &area, double precision) const
{
    int internal = 0;

    W_ASSERT(area.intersects(getBiggerPointInStroke()));
    W_ASSERT(area.topLeft().x() <= area.bottomRight().x());
    W_ASSERT(area.topLeft().y() <= area.bottomRight().y());

    if(area.contains(QPointF(_data.x, _data.y)))
        return true;

    const auto dist1 = distanceFromCenter(area.topLeft());
    const auto dist2 = distanceFromCenter(area.bottomLeft());
    const auto dist3 = distanceFromCenter(area.topRight());
    const auto dist4 = distanceFromCenter(area.bottomRight());

    internal += isInternal(dist1, precision);
    internal += isInternal(dist2, precision);
    internal += isInternal(dist3, precision);
    internal += isInternal(dist4, precision);

    return internal && internal < 4;
}

void StrokeCircle::append(const point_s &point, pressure_t pressure)
{
    Q_UNUSED(pressure);
    _data.r = WCommonScript::distance(QPointF(_data.x, _data.y), point);
}

void StrokeCircle::adjust(double zoom)
{
    _data.r /= zoom;
    _data.x /= zoom;
    _data.y /= zoom;
}

std::shared_ptr<Stroke> StrokeCircle::clone() const
{
    std::shared_ptr<StrokeCircle> res(new StrokeCircle);
    res->_data = this->_data;
    res->setMetadata(this->getMetadata());
    W_ASSERT(*res == *this);
    return res;
}

int StrokeCircle::how_much_decrese() const
{
    return 0;
}

std::shared_ptr<Stroke> StrokeCircle::makeNormal() const
{
    double from, to;
    point_s tmp;
    pressure_t press;
    QVector<point_s> _pointLeft, _pointRigth;
    std::shared_ptr<StrokeNormal> _to(new StrokeNormal);

    const auto appendToStroke = [&_to](
            const QVector<point_s> &point,
            const pressure_t press) {
        for (const auto &p : qAsConst(point)) {
            _to->append(p, press);
        }
    };

    press = _data.press;
    from =  _data.y - WCommonScript::Power(_data.r, 1);
    to =    _data.y + WCommonScript::Power(_data.r, 1);

    _pointLeft.reserve(to - from);
    _pointRigth.reserve(to - from);

    // from is the top of the circle
    W_ASSERT(from <= to);

    for(; from <= to;){
        const auto _res = WCommonScript::Power(double(from) - _data.y, 2);   // = y^2
        const double res1 = WCommonScript::Power(_data.r, 2) - _res;         // = x^2

        W_ASSERT(res1 >= 0.);

        const double res2 = qSqrt(res1);                        // = mod(x)

        tmp = point_s(_data.x + res2, from);                   // = x + radius
        _pointLeft.append(tmp);

        tmp.rx() = _data.x - res2;                               // = x - radius
        _pointRigth.insert(0, tmp);

        from += 1.;
    }

    appendToStroke(_pointLeft, press);
    appendToStroke(_pointRigth, press);

    return _to;
}

bool StrokeCircle::isEmpty() const
{
    return false;
}

void StrokeCircle::scale(const QPointF &offset)
{
    this->_data.x += offset.x();
    this->_data.y += offset.y();
}

bool StrokeCircle::operator==(const Stroke &other) const
{
    if (this == &other)
        return true;

    if (this->type() != other.type())
        return false;

    if (Stroke::operator!=(other))
        return false;

    const StrokeCircle &tmp = dynamic_cast<const StrokeCircle &>(other);

    return  this->_data == tmp._data and
            Stroke::operator==(other);
}

bool StrokeCircle::operator!=(const Stroke &other) const
{
    return !(*this == other);
}

int StrokeCircle::type() const
{
    return Stroke::COMPLEX_CIRCLE;
}

QRect StrokeCircle::getBiggerPointInStroke() const
{
    const auto topLeft = QPoint(_data.x - _data.r, _data.y - _data.r);
    const auto bottomRight = QPoint(_data.x + _data.r, _data.y + _data.r);
    return QRect(topLeft, bottomRight);
}

bool StrokeCircle::isInside(const QRectF &rect) const
{
    if (this->getBiggerPointInStroke().intersects(rect.toRect()))
        return true;
    return this->is_inside(rect, 0);
}

void StrokeCircle::clearAudio()
{
}

int StrokeCircle::save(WZipWriterSingle &writer) const
{
    const auto res = Stroke::save(writer);

    if(res != OK)
        return res;

    static_assert(sizeof(QPointF) == sizeof(double) * 2);

    writer.write_object(this->_data);

    static_assert(sizeof(this->_data) == (sizeof(double) * 3 + sizeof(float) + 4));

    return OK;
}

size_t StrokeCircle::getSizeInMemory() const
{
    return 0;
}

size_t StrokeCircle::getSizeInFile() const
{
    static_assert(sizeof(StrokeComplexCommon::current_ver) == sizeof(uchar));

    return  sizeof(StrokeComplexCommon::current_ver) +
            sizeof(this->_data);
}

void StrokeCircle::decreasePrecision()
{
}

size_t StrokeCircle::createControll() const
{
    return 0;
}

