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

    if(reader.readObject(len_point) < 0)
        manage_err(s);
    if(reader.readObject(s->_metadata) < 0)
        manage_err(s);

    const auto res = s->load_ver_1(reader, len_point);

    if(res != OK and ok)
        *ok = res;

    return std::shared_ptr<Stroke>(s);
}

auto Stroke::loadPtr(const VersionFileController &versionController,
                     ReadableAbstract &readable) -> std::pair<int, Stroke *>
{
    std::pair<int, Stroke*> r {-1, nullptr};
    int type;

    if (readable.read (&type, sizeof (type))) {
        return {-1, nullptr};
    }

    switch (versionController.getVersionStroke()) {
        case 0: {
            switch (type) {
                case COMPLEX_NORMAL:
                    r = StrokeNormal::loadPtr (versionController, readable);
                    break;
                case COMPLEX_CIRCLE:
                    r = StrokeCircle::loadPtr (versionController, readable);
                    break;
                case COMPLEX_LINE:
                    r = StrokeLine::loadPtr   (versionController, readable);
                    break;
                case COMPLEX_RECT:
                    r = StrokeRect::loadPtr   (versionController, readable);
                    break;
                default: return {-1, nullptr};
            }
        }
    }

    if (r.first < 0)
        return {-1, nullptr};

    {
        auto [res, meta] = metadata_stroke::load (versionController, readable);
        if (res < 0) {
            delete r.second;
            return {-1, nullptr};
        }
        r.second->_metadata = std::move (meta);
    }

    if (readable.read(&r.second->_flag, sizeof (r.second->_flag)) < 0) {
        delete r.second;
        return {-1, nullptr};
    }

    {
        auto [res, rect] = RectF::load(versionController, readable);
        if (res < 0) {
            delete r.second;
            return {-1, nullptr};
        }
        r.second->_biggerData = std::move (rect);
    }

    return {0, r.second};
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

    if(reader.readBySize(&meta, sizeof(meta)) < 0)
        ver_2_manage_error(ok);
    if(reader.readObject(type) < 0)
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
                            const WColor &color)
{
    pen.setWidthF(TabletUtils::pressureToWidth(press / deltaPress) * prop);
    if (un(is_rubber)) {
        const auto _press = pen.widthF() * deltaColorNull;
        pen.setWidthF(_press);
    } else {
        pen.setColor(color);
    }
}

int Stroke::save(WritableAbstract &file) const
{
    const int t = type();

    if (metadata_stroke::write(file, this->_metadata) < 0)
        return ERROR;

    if (file.write(&t, sizeof(t)))
        return -1;

    static_assert(sizeof(t) == sizeof(int));
    return OK;
}

void Stroke::setAlfaColor(unsigned char alfa)
{
    _metadata.color.set_alfa(alfa);
}

void Stroke::setColor(const WColor &color)
{
    this->_metadata.color = color;
}

WColor Stroke::getColor(double division) const
{
    auto c = WColor {
        this->_metadata.color
    };

    c.setAlfa(c.getAlfa() / division);

    return c;
}

void Stroke::setMetadata(int posizione_audio, const WColor &color)
{
    this->_metadata = metadata_stroke(posizione_audio, color);
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
