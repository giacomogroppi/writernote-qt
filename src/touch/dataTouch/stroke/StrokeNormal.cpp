#include "StrokeNormal.h"
#include "StrokeNormalFile.h"
#include "utils/WCommonScript.h"

StrokeNormal::StrokeNormal()
{}

StrokeNormal::~StrokeNormal()
{}

int StrokeNormal::save(WZipWriterSingle &file) const
{
    W_ASSERT(0);
}

int StrokeNormal::load(WZipReaderSingle &reader, int version)
{
    W_ASSERT(0);
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

    const auto b = StrokeNormal::getBiggerPointInStroke(this->_point.constBegin(),
                                                        this->_point.constEnd());

    this->setBiggerData(b);

    return b;
}

QRect StrokeNormal::getBiggerPointInStroke(QList<point_s>::const_iterator begin,
                                           QList<point_s>::const_iterator end)
{
    QRect biggerData;

    if(unlikely(begin == end)){
        WWarning("Warning: Stroke empty");
        return {0, 0, 0, 0};
    }

    QPoint topLeft = begin->toQPointF(1.).toPoint();
    QPoint bottomRight = topLeft;

    for(; begin != end; begin ++){
        const point_s &point = *begin;

        if(topLeft.x() > point.x())
            topLeft.setX(static_cast<int>(
                                 point.x()
                         ));

        if(topLeft.y() > point.y())
            topLeft.setY(static_cast<int>(
                                 point.y()
                         ));

        if(bottomRight.x() < point.x())
            bottomRight.setX(static_cast<int>(
                                     point.x())
            );

        if(bottomRight.y() < point.y())
            bottomRight.setY(static_cast<int>(
                                     point.y())
            );
    }

    W_ASSERT(topLeft.x() <= bottomRight.x());
    W_ASSERT(topLeft.y() <= bottomRight.y());

    biggerData = QRect(topLeft, bottomRight);

    return biggerData;
}

bool StrokeNormal::isInsideBiggerData(const QRect &rect) const
{
    const auto &area = this->getBiggerPointInStroke();
    return area.intersects(rect);
}

bool StrokeNormal::isInside(const QRectF &rect) const
{
    if(!this->isInsideBiggerData(rect.toRect()))
        return false;

    std::any_of(_point.constBegin(), _point.constEnd(), [=](const point_s &p) {
        return datastruct_isinside(rect, p);
    });

    /*for(const auto &ref : qAsConst(_point)){
        if (datastruct_isinside(rect, ref))
            return true;
    }*/

    return false;
}

size_t StrokeNormal::getSizeInMemory() const
{
    return sizeof(point_s) * this->length();
}

size_t StrokeNormal::getSizeInFile() const
{
    StrokeNormalFileSave d(*this);
    return d.getSizeInFile() + Stroke::getSizeInFile();
}

void StrokeNormal::decreasePrecision()
{
    const auto len = length();

    for(auto i = 1; i < len - 1; i++){
        if(i % 2 == 0){
            this->removeAt(i);
        }
    }
}

int StrokeNormal::removeAt(int i)
{
    auto len = length();

    W_ASSERT(i < len);

    if(this->_pressure.length() > 1){
        W_ASSERT(_pressure.length() == _point.length());
        _pressure.removeAt(i);
    }

    _point.removeAt(i);
    this->modify();

    return len < 2;
}

int StrokeNormal::type() const
{
    W_ASSERT(0);
    return 0;
}

StrokeNormal::StrokeNormal(const StrokeNormal &ref)
{
    *this = ref;
}

void StrokeNormal::movePoint(const QPointF &translation)
{
    for(auto &p : _point) {
        p += translation;
    }
}

void StrokeNormal::reset()
{
    Stroke::reset();

    _pressure.clear();
    _point.clear();
}

Stroke *StrokeNormal::clone() const
{
    auto *tmp = new StrokeNormal(*this);
    return tmp;
}

void StrokeNormal::preappend(int i)
{
    _point.reserve(i);
    _pressure.reserve(i);
}

bool StrokeNormal::isEmpty() const
{
    const auto e = Stroke::isEmpty();
    return e and this->_point.isEmpty();
}

void StrokeNormal::adjust(double zoom)
{
    for(auto &p : _point) {
        p /= zoom;
    }
}

void StrokeNormal::scale(const QPointF &offset)
{
    for(auto &point : _point){
        point += offset;
    }
}

StrokeNormal &StrokeNormal::operator=(const StrokeNormal &other)
{
    Stroke::operator=(other);
    this->_point = other._point;
    this->_pressure = other._pressure;

    W_ASSERT(StrokeNormal::cmp(*this, other));

    return *this;
}

bool StrokeNormal::cmp(const StrokeNormal &stroke1, const StrokeNormal &stroke2)
{
    if (!Stroke::cmp(stroke1, stroke2)) {
        return false;
    }

    if (!WCommonScript::cmp_list(
            stroke1._point,
            stroke2._point,
            [](auto &p1, auto &p2){
                return p1 == p2;
        })) {
        return false;
    }

    if (!WCommonScript::cmp_list(
            stroke1._pressure,
            stroke2._pressure,
            [](auto p1, auto p2) {
                return p1 == p2;
            }
            )){
        return false;
    }

    return true;
}

StrokeNormal* StrokeNormal::split(int index)
{
    StrokeNormal *ret = new StrokeNormal();
    ret->reset();

    for (auto from = index; from < this->_point.length(); from ++) {
        ret->append(
                    _point.at(i),
                    _pressure.at(i);
                    );
    }

    ret->setMetadata(this->getMetadata());

    return ret;
}

