#pragma once

#include <QPainter>
#include "core/WImage.h"
#include "touch/dataTouch/Point.h"

class WPainter: public QPainter
{
private:
    WImage *_img;
    colore_s color;
#ifdef Q_OS_IOS

#else
    QPainter _painter;
#endif
public:
    explicit WPainter(QPaintDevice *img);

    void setColor(const colore_s &color);

    void drawLine(const QPointF &p1, const QPointF &p2);

    bool end();
};

inline void WPainter::setColor(const colore_s &color)
{
    this->color = color;
}

