#include "tabletcanvas.h"

#include <QCoreApplication>
#include <QPainter>
#include <QtMath>
#include <cstdlib>

#include <QDebug>

#include <QTabletEvent>
#include <QEvent>

#include "datastruct/datastruct.h"

TabletCanvas::TabletCanvas()
    : QWidget(nullptr), m_brush(m_color)
    , m_pen(m_brush, 1.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin)
{
    resize(500, 500);
    setAutoFillBackground(true);
    setAttribute(Qt::WA_TabletTracking);

    this->data = nullptr;
}


bool TabletCanvas::saveImage(const QString &file)
{
    return m_pixmap.save(file);
}


bool TabletCanvas::loadImage(const QString &file)
{
    bool success = m_pixmap.load(file);

    if (success) {
        update();
        return true;
    }
    return false;
}

void TabletCanvas::clear()
{
    m_pixmap.fill(Qt::white);
    update();
}

/* funzione che viene richiamata tutte le volte che si muove qualcosa sulla tabella */
void TabletCanvas::tabletEvent(QTabletEvent *event){
    switch (event->type()) {
        case QEvent::TabletPress:
            if (!m_deviceDown) {
                m_deviceDown = true;
                lastPoint.pos = event->pos();
                lastPoint.pressure = event->pressure();
                lastPoint.rotation = event->rotation();
            }
            break;
        case QEvent::TabletMove:
            /* richiamata quando la penna scorre toccando lo schermo */
            if (m_deviceDown) {
                QPainter painter(&m_pixmap);
                paintPixmap(painter, event);
                lastPoint.pos = event->pos();
                lastPoint.pressure = event->pressure();
                lastPoint.rotation = event->rotation();

                updatelist(event);
            }
            break;
        case QEvent::TabletRelease:
            if (m_deviceDown && event->buttons() == Qt::NoButton)
                m_deviceDown = false;
            update();
            break;
        default:
            break;
    }
    event->accept();
}


void TabletCanvas::initPixmap()
{
    qreal dpr = devicePixelRatio();
    QPixmap newPixmap = QPixmap(qRound(width() * dpr), qRound(height() * dpr));
    newPixmap.setDevicePixelRatio(dpr);
    newPixmap.fill(Qt::white);
    QPainter painter(&newPixmap);
    if (!m_pixmap.isNull())
        painter.drawPixmap(0, 0, m_pixmap);
    painter.end();
    m_pixmap = newPixmap;
}


qreal TabletCanvas::pressureToWidth(qreal pressure)
{
    return pressure * 10 + 1;
}


void TabletCanvas::resizeEvent(QResizeEvent *)
{
    initPixmap();
}
