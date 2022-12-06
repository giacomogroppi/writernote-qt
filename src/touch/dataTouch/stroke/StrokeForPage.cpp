#include "StrokeForPage.h"
#include "touch/tabletcanvas.h"
#include "Stroke.h"
#include "StrokeNormal.h"

StrokeForPage::StrokeForPage()
{
    this->_data = new StrokeNormal;
}

void StrokeForPage::setPressure(pressure_t press)
{
    this->_data->force_pressure(press);
}

#ifdef ALL_VERSION
void StrokeForPage::append(const StrokeNormal &stroke)
{
    using namespace WCommonScript;
    auto &l = this->_data->_point;

    for_each(stroke._point, [&l](const point_s &point) {
        l.append(point);
    });
}

#endif // ALL_VERSION

int StrokeForPage::load(WZipReaderSingle &reader, int ver_stroke)
{
    int ok;
    auto *tmp =
            dynamic_cast<StrokeNormal*>(
                    Stroke::load(reader, ver_stroke, &ok)
            );
    if(ok != OK){

            delete tmp;
        return ok;
    }

    delete this->_data;
    this->_data = tmp;

    if(_data->_pressure[0] > 10)
        _data->_pressure[0] = 1.5;

    return OK;
}

size_t StrokeForPage::getSizeInFile() const
{
    return this->_data->getSizeInFile();
}

void StrokeForPage::scale(const QPointF &delta)
{
    this->_data->scale(delta);
}

void StrokeForPage::append(const point_s &point, pressure_t pressure)
{
    _data->append(point, pressure);
}

void StrokeForPage::reset()
{
    delete this->_data;
    this->_data = new StrokeNormal;
}

void StrokeForPage::setMetadata(const colore_s &colore)
{
    _data->setMetadata(-1, colore);
}
