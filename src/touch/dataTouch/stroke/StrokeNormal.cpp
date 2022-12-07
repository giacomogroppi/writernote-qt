#include "StrokeNormal.h"
#include "StrokeNormalFile.h"
#include "utils/WCommonScript.h"
#include "utils/common_error_definition.h"

StrokeNormal::StrokeNormal()
    : Stroke()
{
    StrokeNormal::reset_flag();
}

StrokeNormal::~StrokeNormal() = default;

int StrokeNormal::save(WZipWriterSingle &file) const
{
    const int len_point     = this->_point.length();
    const int len_pressure  = this->_pressure.length();
    file.write_object(len_point);
    file.write_object(len_pressure);

    for (const auto p : qAsConst(this->_pressure)) {
        static_assert(sizeof(p) == sizeof(pressure_t));
        file.write_object(p);
    }

    for (const auto p : qAsConst(this->_point )){
        static_assert(sizeof(p) == sizeof(point_s));
        file.write_object(p);
    }

    return OK;
}

Stroke *StrokeNormal::makeNormal() const
{
    return nullptr;
}

int StrokeNormal::load_ver_1(WZipReaderSingle &reader, int len_point)
{
    int i;

    struct point_s_curr {
        double _x, _y;
        float press;
    };

    point_s_curr point;
    point_s tmp;

    for(i = 0; i < len_point; i++){
        if(reader.read_object(point) < 0)
            return ERROR;

        tmp = point_s(point._x, point._y);

        _point.append(tmp);
        _pressure.append(point.press);
    }

    modify();
    return OK;
}

int StrokeNormal::load_ver_2(WZipReaderSingle &reader)
{
    int i;
    int len_press, len_point;
    pressure_t tmp;
    point_s point_append;

    if(reader.read_object(len_point) < 0)
        return ERROR;
    if(reader.read_object(len_press) < 0)
        return ERROR;

    W_ASSERT(len_press <= len_point);

    for(i = 0; i < len_press; i++){
        if(reader.read_object(tmp) < 0)
            return ERROR;
        _pressure.append(tmp);
    }

    for(i = 0; i < len_point; i++){
        if(reader.read_object(point_append) < 0)
            return ERROR;
        _point.append(point_append);
    }

    modify();
    return OK;
}

void StrokeNormal::modify()
{
    this->_flag = flag_state::UPDATE_PRESSURE;

    W_ASSERT(this->needToUpdatePressure());
}

int StrokeNormal::load(WZipReaderSingle &reader, int version, int len_point)
{
    switch (version) {
        case 0:
            return ERROR_VERSION;
        case 1:
            return load_ver_1(reader, len_point);
        case 2:
            return load_ver_2(reader);
        default:
            return ERROR_VERSION_NEW;
    }
}

void StrokeNormal::append(const point_s &point, pressure_t pressure)
{
    _point.append(point);
    _pressure.append(pressure);
    this->modify();
}

void StrokeNormal::draw(QPainter &painter, cbool is_rubber, cint page, QPen &pen, cdouble prop) const
{
    StrokeNormal::drawData
            <   QList<point_s>::ConstIterator,
                QList<pressure_t>::ConstIterator> data = {
        .begin_point = this->_point.constBegin(),
        .end_point   = this->_point.constEnd(),
        .begin_press = this->_pressure.constBegin(),
        .end_press   = this->_pressure.constEnd(),
        .press_null  = this->constantPressure()
    };

    StrokeNormal::draw(painter, is_rubber, page, pen, prop, this->getColor(1.), data);
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

bool StrokeNormal::is_inside(const QRectF &rect, double precision) const
{
    {
        const auto &area = this->getBiggerPointInStroke();
        if(!area.intersects(rect.toRect())){
            return false;
        }
    }

    for (const auto &p : qAsConst(this->_point)) {
        if(datastruct_isinside(rect, p)) {
            return true;
        }
    }

    return false;
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
    return COMPLEX_NORMAL;
}

#ifdef ALL_VERSION

#endif // ALL_VERSION

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

int StrokeNormal::how_much_decrese() const
{
    return this->length();
}

void StrokeNormal::removeAt(int indexFrom, int indexTo)
{
    if(indexTo == -1) {
        _point.remove(indexFrom, length() - indexFrom);
    } else {
        _point.remove(indexFrom, indexTo - indexFrom + 1);
    }
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
    if (stroke1 != stroke2)
        return false;

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
    auto *ret = new StrokeNormal();

    const auto original = index;

    for (auto from = index; from < this->_point.length(); from ++) {
        ret->append(
                    _point.takeAt(original),
                    _pressure.takeAt(original)
                    );
    }

    ret->setMetadata(this->getMetadata());

    return ret;
}

void StrokeNormal::force_pressure(pressure_t press)
{
    if(_pressure.length())
        _pressure.operator[](0) = press;
    else
        _pressure.append(press);
}

