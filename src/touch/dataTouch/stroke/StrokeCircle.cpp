#include "StrokeCircle.h"
#include "touch/dataTouch/page/Page.h"
#include "touch/dataTouch/stroke/StrokeComplexCommon.h"
#include "utils/common_error_definition.h"
#include "touch/TabletUtils.h"

int StrokeCircle::load(WZipReaderSingle &reader)
{
    W_ASSERT(0);
    return -1;
}

StrokeCircle::StrokeCircle()
{

}

double StrokeCircle::distanceFromCenter(const PointF &point) const
{
    using namespace WUtils;
    return std::sqrt(
        Power(_data.position.x() - point.x(), 2) +
        Power(_data.position.y() - point.y(), 2)
    );
}

int StrokeCircle::isInternal(double distance, double precision) const
{
    const auto raggio = std::sqrt(_data.r);
    const auto real_dist1 = std::sqrt(distance);
    return real_dist1 <= raggio + precision;
}

bool StrokeCircle::oneSide(double inside, double outside, double prec) const
{
    return  isInternal(inside, prec) and
            not isInternal(outside, prec);
}

void StrokeCircle::draw(WPainter &painter, bool is_rubber, int page, double fakeProp, const WColor& color) const
{
    WPen pen;
    constexpr bool not_used debCircle = false;
    const auto press = _data.press;
    const double prop = fakeProp == PROP_RESOLUTION ? fakeProp : 1.;

    const auto point = Page::at_translation(_data.position, page);
    const auto tmp = point * prop;

    WDebug(debCircle, "prop: " << prop);

    pen.setColor(color);
    pen.setPressure(press * (is_rubber ? deltaColorNull : 1.0));

    painter.setPen(pen);
    painter.drawEllipse(tmp, _data.r * prop, _data.r * prop);

    WDebug(debCircle, tmp << _data.r);
}

void StrokeCircle::draw(WPainter &painter, bool is_rubber, int page, double prop_) const
{
    return draw(painter, is_rubber, page, prop_, getColor(1.0));
}

int StrokeCircle::is_inside(const WLine &line, int from, int precision, cbool needToDeletePoint) const
{
    PointF tl, br;

    constexpr bool not_used debug = true;

    W_ASSERT(_data.position.x() >= 0.);
    W_ASSERT(_data.position.y() >= 0.);
    W_ASSERT(_data.r >= 0.);
    W_ASSERT(_data.press >= 0.);

    line.get_point(tl, br);

    cdouble distance1 = distanceFromCenter(tl);
    cdouble distance2 = distanceFromCenter(br);

    WDebug(debug, distance1 << distance2 << tl << br);

    if(WUtils::is_near(distance1, _data.r, (double) precision) or
       WUtils::is_near(distance2, _data.r, (double) precision))
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
bool StrokeCircle::is_inside(const RectF &area, double precision) const
{
    int internal = 0;

    W_ASSERT(area.intersects(getBiggerPointInStroke()));
    W_ASSERT(area.topLeft().x() <= area.bottomRight().x());
    W_ASSERT(area.topLeft().y() <= area.bottomRight().y());

    if(area.contains(_data.position))
        return true;

    const auto dist1 = distanceFromCenter(area.topLeft());
    const auto dist2 = distanceFromCenter(area.bottomLeft());
    const auto dist3 = distanceFromCenter(area.topRight());
    const auto dist4 = distanceFromCenter(area.bottomRight());

    internal += isInternal(dist1, precision);
    internal += isInternal(dist2, precision);
    internal += isInternal(dist3, precision);
    internal += isInternal(dist4, precision);

    return internal and internal < 4;
}

void StrokeCircle::append(const PointF &point, pressure_t pressure)
{
    (void)(pressure);
    _data.r = WUtils::distance(_data.position, point);
}

void StrokeCircle::adjust(double zoom)
{
    _data.r /= zoom;
    _data.position /= zoom;
}

std::unique_ptr<Stroke> StrokeCircle::clone() const
{
    std::unique_ptr<StrokeCircle> res(new StrokeCircle);
    res->_data = this->_data;

    Stroke::clone(*res);

    W_ASSERT(*res == *this);
    return res;
}

int StrokeCircle::how_much_decrese() const
{
    return 0;
}

std::unique_ptr<Stroke> StrokeCircle::makeNormal() const
{
    double from, to;
    PointF tmp;
    pressure_t press;
    WVector<PointF> _pointLeft, _pointRigth;
    std::unique_ptr<StrokeNormal> _to(new StrokeNormal);

    const auto appendToStroke = [&_to](
            const WVector<PointF> &point,
            const pressure_t press) {
        for (const auto &p : std::as_const(point)) {
            _to->append(p, press);
        }
    };

    press = _data.press;
    from = _data.position.y() - WUtils::Power(_data.r, 1);
    to = _data.position.y() + WUtils::Power(_data.r, 1);

    _pointLeft.reserve(to - from);
    _pointRigth.reserve(to - from);

    // from is the top of the circle
    W_ASSERT(from <= to);

    for(; from <= to;){
        const auto _res = WUtils::Power(double(from) - _data.position.y(), 2);   // = y^2
        const double res1 = WUtils::Power(_data.r, 2) - _res;         // = x^2

        W_ASSERT(res1 >= 0.);

        const double res2 = std::sqrt(res1);                        // = mod(x)

        tmp = PointF(_data.position.x() + res2, from);                   // = x + radius
        _pointLeft.append(tmp);

        tmp.rx() = _data.position.x() - res2;                               // = x - radius
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

void StrokeCircle::scale(const PointF &offset)
{
    this->_data.position += offset;
}

bool StrokeCircle::operator==(const Stroke &other) const
{
    if (this == &other)
        return true;

    if (this->type() != other.type())
        return false;

    if (Stroke::operator!=(other))
        return false;

    const auto &tmp = dynamic_cast<const StrokeCircle &>(other);

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

RectF StrokeCircle::getBiggerPointInStroke() const
{
    const auto topLeft = _data.position - PointF { _data.r, _data.r };
    const auto bottomRight = _data.position - PointF { _data.r, _data.r };
    return RectF(topLeft, bottomRight);
}

bool StrokeCircle::isInside(const RectF &rect) const
{
    if (this->getBiggerPointInStroke().intersects(rect))
        return true;
    return this->is_inside(rect, 0);
}

int StrokeCircle::save(WritableAbstract &writer) const
{
    const auto res = Stroke::save(writer);

    if(res != OK)
        return res;

    if (auto err = writer.write(this->_data.r))
        return ERROR;
    if (auto err = PointF::write(writer, _data.position))
        return err;
    if (auto err = pressure_t::write(writer, this->_data.press))
        return ERROR;

    static_assert_type(_data.r, double);

    return OK;
}

size_t StrokeCircle::getSizeInMemory() const
{
    return 0;
}

size_t StrokeCircle::getSizeInFile() const
{
    static_assert(sizeof(StrokeComplexCommon::current_ver) == sizeof(unsigned char));

    return  sizeof(StrokeComplexCommon::current_ver) +
            sizeof(this->_data);
}

void StrokeCircle::decreasePrecision()
{
}

size_t StrokeCircle::createControl() const
{
    return 0;
}

auto StrokeCircle::loadPtr(const VersionFileController &versionController,
                           ReadableAbstract &readable) -> WPair<Error, StrokeCircle *>
{
    if (versionController.getVersionStrokeCircle() != 0)
        return {Error::makeErrVersion(), nullptr};

    auto* d = new StrokeCircle;

    if (auto err = readable.read(d->_data.r))
        return {err, nullptr};

    {
        const auto [result, data] = PointF::load(versionController, readable);
        if (result)
            return {result, {}};
        d->_data.position = std::move(data);
    }

    {
        const auto [result, data] = pressure_t::load(versionController, readable);
        if (result)
            return {result, nullptr};
        d->_data.press = std::move(data);
    }

    return {Error::makeOk(), d};
}

void StrokeCircle::append (WListFast<PointF> &&points, WListFast<pressure_t> &&pressures)
{
    W_ASSERT(points.size() == pressures.size());
    for (int i = 0; i < points.size(); i++) {
        this->append(points.at(i), pressures.at(i));
    }
}
