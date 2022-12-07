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

void StrokeForPage::draw(QPainter &painter, double zoom, double delta,
                         QPen &pen, const QPointF &pointFirstPage,
                         const Page &page) const
{
    int counterPoint, lenPoint;
    pressure_t pressure;

redo:

    lenPoint = this->_data->_point.length();

    if(!lenPoint)
        return;

    pressure = _data->getPressure();

    /**
     * It is due to a bug introduced a long time ago.
    */
    if(unlikely(pressure <= 0.0)){
        if (_data->_point.length()) {
            (pressure_t &)_data->_point[0] = .1f;
        }
        goto redo;
    }

    pressure = TabletCanvas::pressureToWidth(pressure * zoom / 2.0) * delta;

    pen.setWidthF(pressure);
    pen.setColor(_data->getColor());

    painter.setPen(pen);

    for(counterPoint = 0; counterPoint < lenPoint; counterPoint += 2){
        const auto ref1 = datastruct::at_draw_page(counterPoint + 0, page, pointFirstPage, zoom * delta);
        const auto ref2 = datastruct::at_draw_page(counterPoint + 1, page, pointFirstPage, zoom * delta);

        painter.drawLine(ref1, ref2);
        //painter.drawLine(ref1._x, ref1._y, ref2._x, ref2._y);
    }
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
