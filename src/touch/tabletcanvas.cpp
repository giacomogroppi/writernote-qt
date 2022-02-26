#include "tabletcanvas.h"
#include "mainwindow.h"
#include <QCoreApplication>
#include <QPainter>
#include <QtMath>
#include <cstdlib>
#include <QApplication>
#include <QTabletEvent>
#include <QEvent>
#include "datastruct/datastruct.h"
#include "utils/setting_define.h"
#include "utils/dialog_critic/dialog_critic.h"
#include <QSettings>
#include "square/square.h"
#include "property/property_control.h"
#include "audioplay/audioplay.h"
#include "testing/memtest.h"
#include "touch/laser/laser.h"
#include "touch/object_finder/object_finder.h"

static void saveLastMethod(TabletCanvas::e_method);
static void loadLastMethod(TabletCanvas *);

TabletCanvas::TabletCanvas()
    : QWidget(nullptr)
    , m_pen(QBrush(), 1.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin)
{
    this->setObjectName("TabletCanvas");
    this->resize(500, 500);
    setAutoFillBackground(true);
    setAttribute(Qt::WA_TabletTracking);

    WNew(data, Document, ());

    WNew(zoom, zoom_control, ());
    WNew(m_redoundo, redoundo, (this));
    m_property = new class property_control(this);
    m_square = new class square(this, m_property);
    m_finder = new class object_finder(this);

    m_property->installEventFilter(this);

    loadScrollinSetting();

    loadLastMethod(this);
}

#define DELETE_IF_EXIST(pointer) if(pointer) WDelete(pointer);
TabletCanvas::~TabletCanvas()
{
    DELETE_IF_EXIST(zoom);
    DELETE_IF_EXIST(m_redoundo);

    saveLastMethod(this->medotodiinserimento);
    saveScrollingSetting();
}

void TabletCanvas::restoreO()
{
    if(data->datatouch->isempty()){
        this->data->datatouch->newPage(this->m_sheet->WhatIsSelected());
        return;
    }

    this->data->datatouch->repositioning();
    update();

    lastpointtouch.set = false;

    this->resizeEvent(nullptr);
}

void TabletCanvas::clear()
{
    m_pixmap.fill(Qt::white);
    update();
}

void TabletCanvas::updateBrush(const QTabletEvent *event){
    int hue, saturation, value, alpha;
    this->m_color.getHsv(&hue, &saturation, &value, &alpha);

    int vValue = int(((event->yTilt() + 60.0) / 120.0) * 255);
    int hValue = int(((event->xTilt() + 60.0) / 120.0) * 255);

    switch (m_alphaChannelValuator) {
            case PressureValuator:
                m_color.setAlphaF(event->pressure()/2);
                break;
            case TangentialPressureValuator:
                if (event->deviceType() == QTabletEvent::Airbrush)
                    m_color.setAlphaF(qMax(0.01, (event->tangentialPressure() + 1.0) / 2.0));
                else
                    m_color.setAlpha(255);
                break;
            case TiltValuator:
                m_color.setAlpha(std::max(std::abs(vValue - 127),
                                          std::abs(hValue - 127)));
                break;
            default:
                m_color.setAlpha(255);
        }
    switch (m_colorSaturationValuator) {
            case VTiltValuator:
                m_color.setHsv(hue, vValue, value, alpha);
                break;
            case HTiltValuator:
                m_color.setHsv(hue, hValue, value, alpha);
                break;
            case PressureValuator:
                m_color.setHsv(hue, int(event->pressure()/2 * 255.0), value, alpha);
                break;
            default:
                ;
        }

    switch (m_lineWidthValuator) {
            case PressureValuator:
                m_pen.setWidthF(pressureToWidth(event->pressure()/2));
                break;
            case TiltValuator:
                m_pen.setWidthF(std::max(std::abs(vValue - 127),
                                         std::abs(hValue - 127)) / 12);
                break;
            default:
                m_pen.setWidthF(1);
        }
    if (event->pointerType() == QTabletEvent::Eraser) {
            m_pen.setWidthF(event->pressure()/2 * 10 + 1);
        } else {
            m_pen.setColor(m_color);
        }
}

void TabletCanvas::updateCursor(const QTabletEvent *event)
{
    QCursor cursor;
    if (event->type() != QEvent::TabletLeaveProximity) {
        if (event->pointerType() == QTabletEvent::Eraser || this->medotodiinserimento == e_method::rubber) {
            cursor = QCursor(QPixmap(":image/images/cursor-eraser.png"), 3, 28);
        }
        else {
            /* deviceType we can't change event->device to deviceType because in qt 5.12.* it's not support [and it't the version for pacakge qt support] */
            switch (event->deviceType()) {
            case QTabletEvent::Stylus:
                cursor = QCursor(QPixmap(":image/images/cursor-pencil.png"), 0, 0);
                break;
            case QTabletEvent::Airbrush:
                cursor = QCursor(QPixmap(":image/images/cursor-airbrush.png"), 3, 4);
                break;
            case QTabletEvent::RotationStylus: {
                QImage origImg(QLatin1String(":image/images/cursor-felt-marker.png"));
                QImage img(32, 32, QImage::Format_ARGB32);
                QColor solid = m_color;
                solid.setAlpha(255);
                img.fill(solid);
                QPainter painter(&img);
                QTransform transform = painter.transform();
                transform.translate(16, 16);
                transform.rotate(event->rotation());
                painter.setTransform(transform);
                painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
                painter.drawImage(-24, -24, origImg);
                painter.setCompositionMode(QPainter::CompositionMode_HardLight);
                painter.drawImage(-24, -24, origImg);
                painter.end();
                cursor = QCursor(QPixmap::fromImage(img), 16, 16);
            } break;
            default:
                break;
            }
        }
    }
    setCursor(cursor);
}


qreal TabletCanvas::pressureToWidth(qreal pressure)
{
    return pressure * 10 + 1;
}

static void saveLastMethod(TabletCanvas::e_method val){
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_METHOD_TOUCH);

    setting.setValue(KEY_METHOD_TOUCH, val);
    setting.endGroup();

}

static void loadLastMethod(TabletCanvas *p){
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_METHOD_TOUCH);

    p->medotodiinserimento = static_cast<TabletCanvas::e_method>(
                setting.value(KEY_METHOD_TOUCH, TabletCanvas::pen)
                .toInt());

    setting.endGroup();

}

void TabletCanvas::triggerNewView(const bool all){
    data->datatouch->triggerNewView(parent->m_audioplayer->getPositionSecond(), all);
}

void TabletCanvas::triggerNewView(const QList<int> &Page, const bool all){
    this->data->datatouch->triggerNewView(Page, parent->m_audioplayer->getPositionSecond(), all);
}

void canvas_send_touch_event(QObject *_canvas, const QPointF &pos,
                             QEvent::Type event_type, QTabletEvent::PointerType deviceType,
                             cbool now)
{
    QTabletEvent *e;
    TabletCanvas *c;

    W_ASSERT(_canvas->objectName() == "TabletCanvas");

    c = static_cast<TabletCanvas *>(_canvas);

    e = new QTabletEvent(event_type, pos, QPointF(), 0, deviceType, 2, 3, 3, 1, 1, 1, Qt::KeyboardModifier::NoModifier, 432243);
    if(now){
        c->send_touch_event(e);
        delete e;
    }else{
        QApplication::postEvent(_canvas, e);
    }
}

bool TabletCanvas::eventFilter(QObject *ref, QEvent *e)
{
    QPointF point_touch;
    QTabletEvent *touch;
    QEvent::Type type = e->type();
    constexpr not_used bool eventFilterCanvasDebug = false;

    if(ref == m_property){
        cbool isVisible = m_property->isVisible();

        if(type != QEvent::TabletPress && type != QEvent::TabletRelease && type != QEvent::TabletMove){
            goto out;
        }

        touch = static_cast<QTabletEvent *>(e);
        const QPointF &PT = touch->posF();

        if(unlikely(isVisible)){
            WDebug(eventFilterCanvasDebug, name << __FUNCTION__ << "Visible" << m_property->rect() << PT);
            if(m_property->rect().contains(PT.toPoint())){
                WDebug(eventFilterCanvasDebug, name << __FUNCTION__ << "Inside" << e->type());
                goto out;
            }
        }else{
            WDebug(eventFilterCanvasDebug, name << __FUNCTION__ << "Not visible");
        }

        point_touch = touch->globalPosF() - this->mapToGlobal(this->pos());

        WDebug(eventFilterCanvasDebug, name << __FUNCTION__ << "Point" << point_touch << touch->pos() << m_square->get_first_point().point << m_square->get_last_point().point);

        canvas_send_touch_event(this, point_touch, type, touch->pointerType(), true);

        // the point is out but it's visible
        if(isVisible)
            goto out;

        return true;
    }

out:
    return ref->event(e);
}
