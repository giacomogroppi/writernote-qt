#pragma once

#include <QPainter>
#include <QSize>
#include <QPointF>

#include "touch/dataTouch/datastruct/DataStruct.h"
#include "touch/dataTouch/page/Page.h"
#include "core/WPixmap.h"

#define DRAW_SINGLE_LOAD_DEF 1.0
/*
 * the function is useful when you have an
 * image with strokes and you want to
 * draw it on the surface, without having
 * to copy the drawing rules used in
 * TabletCanvas :: load ()
*/
void singleLoad(QPainter        &painter,
        const WPixmap           &img,
        const QSize             &sizeRect,
        const QPointF           &PointFirstPage,
        cdouble                 counterPage,
        cdouble                 m);

/*
 * @countHeigth is use when we what to
 * create a target rect that to create
 * a targetRect that spans multiple
 * pages, as in Square
*/

#define DRAW_CREATE_SIZE_RECT_DEF_COUNTER_HEIGTH 1
#define DRAW_CREATE_SIZE_RECT_DEF_PRO 1.0
inline QSize createSizeRect(
        const DataStruct    &data,
        const int           countHeigth,
        const double        pro)
{
    const auto zoom = data.getZoom();
    const int height = Page::getHeight();
    return QSize(Page::getWidth(), height * countHeigth) * zoom * pro;
}

