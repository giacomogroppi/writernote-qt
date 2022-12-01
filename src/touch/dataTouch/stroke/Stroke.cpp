#include "Stroke.h"
#include "touch/tabletcanvas.h"
#include "StrokeNormal.h"
#include "StrokeLine.h"

Stroke *Stroke::load_ver_1(WZipReaderSingle &reader, int *ok)
{
    auto *s = new StrokeNormal();
    int len_point;

    if(ok) *ok = OK;

    if(reader.read_object(len_point) < 0)
        return ERROR;
    if(reader.read_object(s->_metadata) < 0)
        return ERROR;

    const auto res = s->load_ver_1(reader, len_point);

    if(res != OK and ok)
        *ok = res;

    return s;
}

Stroke* Stroke::load_ver_2(WZipReaderSingle &reader, int *ok) {
#define ver_2_manage_error(contr)   \
    do {                            \
        if(contr)                   \
            *contr = ERROR;         \
        return nullptr;             \
    } while(0);



    int i;
    Stroke *res;
    int len_press, len_point;
    pressure_t tmp;
    point_s point_append;

    int type;
    metadata_stroke meta;

    static_assert(sizeof(len_press) == sizeof(len_point));
    static_assert(sizeof(len_point) == stroke_file_size_len);
    static_assert(sizeof(_stroke._metadata) == 8);

    if(reader.read_by_size(&meta, sizeof(meta)) < 0)
        ver_2_manage_error(ok);
    if(reader.read_object(type) < 0)
        ver_2_manage_error(ok);

    switch (type) {
        case COMPLEX_NORMAL:
            res = new StrokeNormal();
            res->load_ver_2(reader, ok);
            return res;
        case COMPLEX_LINE:
            res = new StrokeLine();
            to_continue
    }

    if(unlikely(_stroke.is_complex())){
        return stroke_complex_load(&_stroke, dynamic_cast<StrokeProp &>(_stroke), reader);
    }
}

Stroke* Stroke::load(WZipReaderSingle &reader, int version_stroke, int *ok)
{
    switch (version_stroke) {
        case 0:
            if(ok)
                *ok = ERROR_VERSION;
            return nullptr;
        case 1:
            return Stroke::load_ver_1(reader, ok);
        case 2:
            return Stroke::load_ver_2(reader, ok);
    default:
        break;
    }

    if(ok)
        *ok = ERROR_VERSION_NEW;
    return nullptr;
}

void Stroke::reset_flag()
{
    _flag = UPDATE_BIGGER_DATA | UPDATE_PRESSURE;
    W_ASSERT(this->needToUpdateBiggerData() == true);
    W_ASSERT(this->needToUpdatePressure() == true);
}



size_t Stroke::getSizeInFile() const
{
    size_t s = 0;
    s += sizeof(this->_metadata);
    s += sizeof(int); // prop

    static_assert(sizeof(type()) == sizeof(int));

    return s;
}

bool Stroke::operator==(const Stroke &other) const
{
    if(this->_metadata != other._metadata)
        return false;
    return true;
}

void set_press(
                            QPen &pen,
                            const pressure_t press,
                            const double prop,
                            cbool is_rubber,
                            const QColor &color)
{
    constexpr double deltaPress = 2.;
    constexpr double deltaColorNull = 1.3;

    pen.setWidth(TabletCanvas::pressureToWidth(press / deltaPress) * prop);
    if (unlikely(is_rubber)) {
        const auto _press = pen.widthF() * deltaColorNull;
        pen.setWidthF(_press);
    } else {
        pen.setColor(color);
    }
}

int Stroke::save(WZipWriterSingle &file) const
{
    const auto t = type();
    file.write_object(_metadata);
    file.write_object(t);

    static_assert(sizeof(t) == sizeof(int));
    return OK;
}

void Stroke::setAlfaColor(uchar alfa)
{
    _metadata.color.set_alfa(alfa);
}

QColor Stroke::getColor(double division) const
{
    return this->_metadata.color.toQColor(division);
}

Stroke::Stroke()
{
    this->reset_flag();
}

Stroke::Stroke(const metadata_stroke &met)
{
    this->reset_flag();
    this->_metadata = met;
}
