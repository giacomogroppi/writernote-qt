#include "../tabletcanvas.h"
#include <QPainter>
#include "../../mainwindow.h"
#include "../../utils/color/setcolor.h"
#include <QPolygonF>
#include <QPainterPath>
#include "../square/square.h"

#ifdef PDFSUPPORT
#include "../../frompdf/frompdf.h"
#endif
#include "../../images/fromimage.h"

void TabletCanvas::paintEvent(QPaintEvent *event){
    QPainter painter;
    QRect pixmapPortion;
    static bool needDrawSheet;

    if(!this->parent) return;

    static DataPaint dataPaint = {
        .withPdf = true,
        .IsExportingPdf = false,
        .m = 1,
        //.size = QSize(m_pixmap.size()),
        .parent = parent,
        .m_pixmap = &this->m_pixmap,
        DATAPAINT_DEFINEREST
    };

    dataPaint.pen       = this->m_pen;

    needDrawSheet = false;

    isWriting = false;

    if (m_pixmap.isNull())
        initPixmap(false);

    painter.begin(this);

    pixmapPortion = QRect(event->rect().topLeft() * devicePixelRatio(),
                                    event->rect().size() * devicePixelRatio());
    painter.drawPixmap(event->rect().topLeft(), m_pixmap, pixmapPortion);

    needDrawSheet = data->datatouch->needToCreateNewSheet();
    if(needDrawSheet && (m_sheet->auto_create || data->datatouch->isempty()))
        this->data->datatouch->newPage(this->m_sheet->WhatIsSelected());

    TabletCanvas::load(painter, this->data, dataPaint);

    /*load(painter, this->data, m_color, m_pen, m_brush, lastPoint,
         m_pos_ris, &m_pixmap, true, 1, this->m_pixmap.width(), this->m_pixmap.width(), this->parent, false);*/

    m_square->needReload(painter, this);

    painter.end();
}

void TabletCanvas::updateBrush_load(const double pressure, const QColor &color, QPen &m_pen){
    m_pen.setWidthF(pressureToWidth(pressure/2.00));

    m_pen.setColor(color);

}
