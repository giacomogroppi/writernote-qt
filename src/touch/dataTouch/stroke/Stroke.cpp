#include "Stroke.h"
#include "StrokeNormal.h"
#include "StrokeLine.h"
#include "StrokeCircle.h"
#include "StrokeRect.h"
#include "utils/common_error_definition.h"
#include "touch/TabletUtils.h"

std::shared_ptr<Stroke> Stroke::load_ver_1(WZipReaderSingle &reader, int *ok)
{
#define manage_err(pointer) do { delete pointer; return ERROR; } while(0)
    auto *s = new StrokeNormal();
    int len_point;

    if(ok) *ok = OK;

    if(reader.read_object(len_point) < 0)
        manage_err(s);
    if(reader.read_object(s->_metadata) < 0)
        manage_err(s);

    const auto res = s->load_ver_1(reader, len_point);

    if(res != OK and ok)
        *ok = res;

    return std::shared_ptr<Stroke>(s);
}

std::shared_ptr<Stroke> Stroke::load_ver_2(WZipReaderSingle &reader, int *ok)
{
#define ver_2_manage_error(contr)   \
    do {                            \
        if(contr)                   \
            *contr = ERROR;         \
        return nullptr;             \
    } while(0);


    int type;
    metadata_stroke meta;

    static_assert(sizeof(_metadata) == 8);

    if(reader.read_by_size(&meta, sizeof(meta)) < 0)
        ver_2_manage_error(ok);
    if(reader.read_object(type) < 0)
        ver_2_manage_error(ok);

    switch (type) {
    case COMPLEX_NORMAL:
    {
        std::shared_ptr<StrokeNormal> tmp(new StrokeNormal);
        tmp->load_ver_2(reader);
        return tmp;
    }
    case COMPLEX_LINE:
    {
        std::shared_ptr<StrokeLine> tmp(new StrokeLine);
        tmp->load(reader);
        return tmp;
    }
    case COMPLEX_CIRCLE:
    {
        std::shared_ptr<StrokeCircle> tmp(new StrokeCircle);
        tmp->load(reader);
        return tmp;
    }
    case COMPLEX_RECT:
    {
        std::shared_ptr<StrokeRect> tmp(new StrokeRect);
        tmp->load(reader);
        return tmp;
    }
    default: break;
    }

    if(ok)
        *ok = ERROR;
    return nullptr;
}

std::shared_ptr<Stroke> Stroke::load(
        WZipReaderSingle &reader,
        int version_stroke,
        int *ok)
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
    _flag = UPDATE_BIGGER_DATA;
    W_ASSERT(this->needToUpdateBiggerData() == true);
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
    if (this->_metadata != other._metadata)
        return false;
    return true;
}

bool Stroke::operator!=(const Stroke &other) const
{
    return this->_metadata != other._metadata;
}

void set_press(
                            WPen &pen,
                            const pressure_t press,
                            const double prop,
                            cbool is_rubber,
                            const colore_s &color)
{
    pen.setWidthF(TabletUtils::pressureToWidth(press / deltaPress) * prop);
    if (un(is_rubber)) {
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

void Stroke::setAlfaColor(unsigned char alfa)
{
    _metadata.color.set_alfa(alfa);
}

void Stroke::setColor(const colore_s &color)
{
    this->_metadata.color = color;
}

colore_s Stroke::getColor(double division) const
{
    auto c = colore_s {
        this->_metadata.color
    };

    c.setAlfa(c.getAlfa() / division);

    return c;
}

void Stroke::setMetadata(int posizione_audio, const colore_s &color)
{
    this->_metadata = {
        .posizione_audio = posizione_audio,
        .color = color
    };
}

void Stroke::setMetadata(const metadata_stroke &metadata)
{
    this->_metadata = metadata;
}

size_t Stroke::createControll() const
{
    size_t controll = 0;
    int i;

    for (i = 0; i < NCOLOR; i++) {
        controll += _metadata.color.colore[i];
    }

    return controll;
}

RectF Stroke::getBiggerPointInStroke() const
{
    W_ASSERT(this->needToUpdateBiggerData() == false);
    return this->_biggerData;
}

void Stroke::clearAudio()
{
    this->_metadata.posizione_audio = -1;
}

Stroke::Stroke()
{
    Stroke::reset_flag();
}

Stroke::Stroke(const metadata_stroke &met)
{
    Stroke::reset_flag();
    this->_metadata = met;
}
