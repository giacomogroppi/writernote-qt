#include "Stroke.h"
#include "touch/tabletcanvas.h"

Stroke* Stroke::load(WZipReaderSingle &reader, int version)
{
    W_ASSERT(0);
}

size_t Stroke::getSizeInFile() const
{
    size_t s = 0;
    s += sizeof(this->_metadata);
    s += sizeof(int); // prop

    static_assert(sizeof(type()) == sizeof(int));

    return s;
}

void Stroke::reset()
{
    _flag = UPDATE_BIGGER_DATA | UPDATE_PRESSURE;

    _metadata = {
            .posizione_audio = -1,
            .color = colore_s()
    };
}

bool Stroke::operator==(const Stroke &other)
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
    pen.setWidth(TabletCanvas::pressureToWidth(press / deltaPress) * prop);
    if (unlikely(is_rubber)) {
        const auto _press = pen.widthF() * deltaColorNull;
        pen.setWidthF(_press);
    } else {
        pen.setColor(color);
    }
}
