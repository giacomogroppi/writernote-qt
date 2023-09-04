#include "StrokeNormal.h"
#include "StrokeNormalFile.h"
#include "utils/WCommonScript.h"
#include "utils/common_error_definition.h"

StrokeNormal::StrokeNormal()
    : Stroke()
{
    StrokeNormal::reset_flag();
}

StrokeNormal::~StrokeNormal()
{
    this->_point.clear();
    this->_pressure.clear();
}

int StrokeNormal::save(WritableAbstract &file) const
{
    if (Stroke::save(file))
        return ERROR;

    if (WListFast<PointF>::write(file, this->_point) < 0)
        return ERROR;

    if (WListFast<pressure_t>::write(file, this->_pressure) < 0)
        return ERROR;

    return OK;
}

std::unique_ptr<Stroke> StrokeNormal::makeNormal() const
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
    PointF tmp;

    for(i = 0; i < len_point; i++){
        if(reader.readObject(point) < 0)
            return ERROR;

        tmp = PointF(point._x, point._y);

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
    PointF point_append;

    if(reader.readObject(len_point) < 0)
        return ERROR;
    if(reader.readObject(len_press) < 0)
        return ERROR;

    W_ASSERT(len_press <= len_point);

    for(i = 0; i < len_press; i++){
        if(reader.readObject(tmp) < 0)
            return ERROR;
        _pressure.append(tmp);
    }

    for(i = 0; i < len_point; i++){
        if(reader.readObject(point_append) < 0)
            return ERROR;
        _point.append(point_append);
    }

    modify();
    return OK;
}

void StrokeNormal::modify() const
{
    this->_flag = flag_state::UPDATE_PRESSURE;
    Stroke::modify();
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

void StrokeNormal::append(const PointF &point, pressure_t pressure)
{
    _point.append(point);
    _pressure.append(pressure);
    this->modify();
}

void StrokeNormal::draw(WPainter &painter, cbool is_rubber, cint page, WPen &pen, cdouble prop) const
{
    const auto constPressure = constantPressure();

    auto data = drawData<WListFast<PointF>::const_iterator, WListFast<pressure_t>::const_iterator> (
         this->_point.constBegin(), this->_point.constEnd(), this->_pressure.constBegin(), constPressure, 0
    );

    StrokeNormal::draw(painter, is_rubber, page, pen, prop, this->getColor(1.), data);
}

int StrokeNormal::is_inside(const WLine &rect, int from, int precision, cbool needToDeletePoint) const
{
    const PointF *p1, *p2;
    WLine tmp {};

    int &i = from;

    W_ASSERT(from >= 0);

    const auto len = this->length();

    if (!len)
        return -1;

    if (i >= len)
        return -1;

    if (i == 0){
        const auto &ref = this->getBiggerPointInStroke();
        if(!ref.intersects(rect.toRect()))
            return -1;
    }

    p1 = &_point.at(i);
    for (i++; i < len; i++) {
        p2 = &_point.at(i);

        tmp = WLine(*p1, *p2);

        if (WLine::intersect(tmp, rect, precision)) {
            return i;
        }

        p1 = p2;
    }

    return -1;
}

bool StrokeNormal::is_inside(const RectF &rect, double precision) const
{
    {
        const auto &area = this->getBiggerPointInStroke();
        if(!area.intersects(rect)){
            return false;
        }
    }

    const auto function = [rect](const PointF &point) {
        return datastruct_isinside(rect, point);
    };

    return _point.anyMatch([&rect](const PointF &point) {
        return datastruct_isinside(rect, point);
    });
}

size_t StrokeNormal::createControll() const
{
    size_t controll = 0;

    for(const auto &p : std::as_const(this->_point)){
        controll += WCommonScript::diff(p.x());
        controll += WCommonScript::diff(p.y());
    }

    for(const auto &p : std::as_const(this->_pressure)){
        controll += WCommonScript::diff(p);
    }

    return controll + Stroke::createControll();
}

RectF StrokeNormal::getBiggerPointInStroke() const
{
    if (!Stroke::needToUpdateBiggerData()) {
        return Stroke::getBiggerPointInStroke();
    }

    const auto b = StrokeNormal::getBiggerPointInStroke(this->_point.constBegin(),
                                                        this->_point.constEnd());

    this->setBiggerData(b);

    return b;
}

bool StrokeNormal::isInsideBiggerData(const RectF &rect) const
{
    const auto &area = this->getBiggerPointInStroke();
    return area.intersects(rect);
}

bool StrokeNormal::isInside(const RectF &rect) const
{
    if(!this->isInsideBiggerData(rect))
        return false;

    for(const auto &ref : std::as_const(_point)){
        if (datastruct_isinside(rect, ref))
            return true;
    }

    return false;
}

size_t StrokeNormal::getSizeInMemory() const
{
    return sizeof(WPoint) * this->length();
}

size_t StrokeNormal::getSizeInFile() const
{
    size_t s = 0;
    cint len_pressure = _pressure.size();
    cint len_point    = _point.size();

    static_assert(sizeof(len_pressure) == sizeof(len_point));
    static_assert(sizeof(len_pressure) == 4);

    static_assert_type(len_pressure, const int);
    static_assert_type(len_point, const int);

    s += sizeof(len_point);
    s += sizeof(len_pressure);

    s += sizeof(pressure_t)     * len_pressure;
    s += sizeof(WPoint) * len_point;

    return s + Stroke::getSizeInFile();
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

    if(this->_pressure.size() > 1){
        W_ASSERT(_pressure.size() == _point.size());
        _pressure.remove(i);
    }

    _point.remove(i);
    this->modify();

    return len < 2;
}

int StrokeNormal::type() const
{
    return COMPLEX_NORMAL;
}

StrokeNormal::StrokeNormal(const StrokeNormal &ref)
    : _flag(ref._flag)
{
    *this = ref;
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

UniquePtr<Stroke> StrokeNormal::clone() const
{
    UniquePtr<StrokeNormal> tmp(new StrokeNormal(*this));

    W_ASSERT(*this == *tmp);
    return tmp;
}

void StrokeNormal::preappend(int i)
{
    _point.reserve(i);
    _pressure.reserve(i);
}

bool StrokeNormal::isEmpty() const
{
    return this->_point.isEmpty();
}

void StrokeNormal::adjust(double zoom)
{
    for(auto &p : _point) {
        p /= zoom;
    }
}

void StrokeNormal::scale(const PointF &offset)
{
    for(auto &point : _point){
        point += offset;
    }
}

StrokeNormal &StrokeNormal::operator=(const StrokeNormal &other)
{
    if (&other == this)
        return *this;
    
    Stroke::operator=(other);
    this->_point = other._point;
    this->_pressure = other._pressure;

    W_ASSERT(this->operator==(other));

    return *this;
}

bool StrokeNormal::operator==(const Stroke &_other) const
{
    if(this->type() != _other.type())
        return false;

    const auto &other = dynamic_cast<const StrokeNormal &>(_other);

    if (!WCommonScript::cmp_list(
            this->_point,
            other._point,
            [](auto &p1, auto &p2){
                return p1 == p2;
            })) {
        return false;
    }

    if (!WCommonScript::cmp_list(
            this->_pressure,
            other._pressure,
            [](auto p1, auto p2) {
                return p1 == p2;
            }
            )){
        return false;
    }

    return true;
}

bool StrokeNormal::operator!=(const Stroke &other) const
{
    return !(*this == other);
}

std::shared_ptr<StrokeNormal> StrokeNormal::split(int index)
{
    std::shared_ptr<StrokeNormal> ret(new StrokeNormal);

    const auto original = index;

    for (auto from = index; from < this->_point.size(); from ++) {
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
    if(_pressure.size())
        _pressure.operator[](0) = press;
    else
        _pressure.append(press);
}

StrokeNormal::StrokeNormal(StrokeNormal &&other) noexcept
    : Stroke(dynamic_cast<Stroke&&> (other))
    , _point(std::move(other._point))
    , _pressure(std::move(other._pressure))
    , _flag(other._flag)
{
}

auto StrokeNormal::loadPtr(const VersionFileController &versionController,
                           ReadableAbstract &readable) -> WPair<int, StrokeNormal *>
{
    if (versionController.getVersionStrokeNormal() != 0)
        return {-1, nullptr};

    std::unique_ptr<StrokeNormal> result(new StrokeNormal());

    {
        auto [res, d] = WListFast<PointF>::load(versionController, readable);
        if (res < 0)
            return {-1, nullptr};
        result->_point = std::move(d);
    }

    {
        auto [res, d] = WListFast<pressure_t>::load(versionController, readable);
        if (res < 0)
            return {-1, nullptr};
        result->_pressure = std::move(d);
    }

    if (readable.read(&result->_flag, sizeof(result->_flag)) < 0)
        return {-1, nullptr};

    return {0, result.release()};
}

void StrokeNormal::append (WListFast<PointF> &&points, WListFast<pressure_t> &&pressures)
{
    W_ASSERT(points.size() == pressures.size());
    _point = std::move(points);
    _pressure = std::move(pressures);
    this->modify();
}
