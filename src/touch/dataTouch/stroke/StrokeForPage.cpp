#include "StrokeForPage.h"
#include "touch/tabletcanvas.h"

void StrokeForPage::draw(QPainter &painter, cbool is_rubber, cint page, QPen &pen, cdouble prop) const
{
    StrokeNormal::draw(painter, is_rubber, page, pen, prop);
    W_ASSERT(0);
    int counterPoint;
    pressure_t pressure;

    const auto lenPoint = length();

    if(!lenPoint)
        return;

    pressure = getPressure();

    //pressure = TabletCanvas::pressureToWidth(pressure * prop / 2.0) * delta;
    pressure = static_cast<pressure_t>(
            TabletCanvas::pressureToWidth(pressure * prop / 2.0)
    );

    pen.setWidthF(pressure);
    pen.setColor(getColor(1.));

    painter.setPen(pen);

    for(counterPoint = 0; counterPoint < lenPoint; counterPoint += 2){
        const auto ref1 = datastruct::at_draw_page(counterPoint + 0, page, pointFirstPage, prop);
        const auto ref2 = datastruct::at_draw_page(counterPoint + 1, page, pointFirstPage, prop);

        painter.drawLine(ref1, ref2);
        //painter.drawLine(ref1._x, ref1._y, ref2._x, ref2._y);
    }
}

StrokeForPage::StrokeForPage()
    : StrokeNormal()
{

}
