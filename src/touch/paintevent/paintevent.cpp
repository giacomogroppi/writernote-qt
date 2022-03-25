#include "touch/tabletcanvas.h"
#include <QPainter>
#include "mainwindow.h"
#include <QPolygonF>
#include <QPainterPath>
#include "touch/square/square.h"

#include "frompdf/frompdf.h"
#include "images/fromimage.h"

void TabletCanvas::paintEvent(QPaintEvent *event){
    QPainter painter;
    QRect pixmapPortion;

    cbool needToCreateNewPage = data->datatouch->needToCreateNewSheet() && _sheet->auto_create;

    static DataPaint dataPaint = {
        .withPdf = true,
        .IsExportingPdf = false,
        .m = 1.,
        //.size = QSize(m_pixmap.size()),
        .parent = _parent,
        .m_pixmap = &this->_pixmap,
        DATAPAINT_DEFINEREST
    };
    WDebug(false, "TabletCanvas" << __FUNCTION__);
    dataPaint.pen       = this->m_pen;
    isWriting           = false;

    if (unlikely(_pixmap.isNull()))
        initPixmap(false);

    painter.begin(this);
    W_ASSERT(painter.isActive());

    pixmapPortion = QRect(event->rect().topLeft() * devicePixelRatio(),
                          event->rect().size() * devicePixelRatio());

    painter.drawPixmap(event->rect().topLeft(), _pixmap, pixmapPortion);

    if(unlikely( needToCreateNewPage || data->isEmpty())){
        this->data->datatouch->newPage(this->_sheet->WhatIsSelected());
        this->updatePageCount();
    }

    TabletCanvas::load(painter, this->data, dataPaint);

    if(_input == e_method::selection){
        _square->needReload(painter);
    }

    painter.end();
}

void TabletCanvas::updateBrush_load(
        const double    pressure,
        const QColor    &color,
        QPen            &m_pen)
{
    m_pen.setWidthF(pressureToWidth(pressure/2.00));

    m_pen.setColor(color);

}
