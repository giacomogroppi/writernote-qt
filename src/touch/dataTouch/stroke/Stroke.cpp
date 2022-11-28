#include "Stroke.h"
#include "touch/tabletcanvas.h"

Stroke* Stroke::load(WZipReaderSingle &reader, int version, int *ok)
{
    W_ASSERT(0);
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
