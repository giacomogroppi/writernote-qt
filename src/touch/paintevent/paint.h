#ifndef PAINT_H
#define PAINT_H

#include <QPainter>
#include <QImage>
#include <QSize>
#include <QPointF>

#include "touch/datastruct/datastruct.h"
#include "touch/datastruct/page.h"

/*
 * the function is useful when you have an
 * image with strokes and you want to
 * draw it on the surface, without having
 * to copy the drawing rules used in
 * TabletCanvas :: load ()
*/
void singleLoad(
        QPainter        &painter,
        const QImage    &img,
        const QSize     &sizeRect,
        const QPointF   &PointFirstPage,
        const int       counterPage,
        const double    m = 1.0);

inline QSize createSizeRect(
        const datastruct    *data,
        const double        pro = 1.0)
{
    const auto zoom = data->getZoom();
    return QSize(page::getWidth(), page::getHeight()) * zoom * pro;
}

#endif // PAINT_H
