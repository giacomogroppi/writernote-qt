#include "tabletcanvas.h"
#include "core/core.h"
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

TabletCanvas::TabletCanvas()
    : QWidget(nullptr)
    , m_pen(QBrush(), 1.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin)
    , _method(true)
{
    this->setObjectName("TabletCanvas");
    this->resize(500, 500);
    setAutoFillBackground(true);
    setAttribute(Qt::WA_TabletTracking);

    WNew(data, Document, ());

    WNew(_zoom, zoom_control, ());
    WNew(_redoundo, redoundo, (this));
    _property = new class property_control(this);
    _square = new class square(this, _property);
    _finder = new class object_finder(this);

    _property->installEventFilter(this);

    loadScrollinSetting();

    this->setAttribute(Qt::WA_AcceptTouchEvents);
    /*qDebug() << this->testAttribute(Qt::WA_AcceptTouchEvents);
    qDebug() << this->testAttribute(Qt::WA_WState_AcceptedTouchBeginEvent);
    qDebug() << this->testAttribute(Qt::WA_TouchPadAcceptSingleTouchEvents);
    qDebug() << this->testAttribute(Qt::WA_X11DoNotAcceptFocus);
    qDebug() << this->testAttribute(Qt::WA_TabletTracking);*/
}

TabletCanvas::~TabletCanvas()
{
    WDelete(_zoom);

    if(_redoundo)
        WDelete(_redoundo);

    WDelete(data);

    _method.save();
    saveScrollingSetting();
}

void TabletCanvas::restoreO()
{
    if(data->datatouch->isempty()){
        this->data->datatouch->newPage(this->_sheet->WhatIsSelected());
        return;
    }

    this->data->datatouch->repositioning();
    update();

    lastpointtouch = false;

    this->resizeEvent(nullptr);
}

void TabletCanvas::clear()
{
    _pixmap.fill(Qt::white);
    update();
}

void TabletCanvas::updateBrush(const QTabletEvent *event)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    int hue, saturation, value, alpha;
    this->_color.getHsv(&hue, &saturation, &value, &alpha);

    int vValue = int(((event->yTilt() + 60.0) / 120.0) * 255);
    int hValue = int(((event->xTilt() + 60.0) / 120.0) * 255);

    switch (m_alphaChannelValuator) {
            case PressureValuator:
                _color.setAlphaF(event->pressure()/2);
                break;
            case TangentialPressureValuator:
                if (event->deviceType() == QTabletEvent::Airbrush)
                    _color.setAlphaF(qMax(0.01, (event->tangentialPressure() + 1.0) / 2.0));
                else
                    _color.setAlpha(255);
                break;
            case TiltValuator:
                _color.setAlpha(std::max(std::abs(vValue - 127),
                                         std::abs(hValue - 127)));
                break;
            default:
                _color.setAlpha(255);
        }
    switch (m_colorSaturationValuator) {
            case VTiltValuator:
                _color.setHsv(hue, vValue, value, alpha);
                break;
            case HTiltValuator:
                _color.setHsv(hue, hValue, value, alpha);
                break;
            case PressureValuator:
                _color.setHsv(hue, int(event->pressure()/2 * 255.0), value, alpha);
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
            m_pen.setColor(_color);
        }
#endif
}

void TabletCanvas::updateCursor(const QTabletEvent *event)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCursor cursor;
    if (event->type() != QEvent::TabletLeaveProximity) {
        if (event->pointerType() == QTabletEvent::Eraser || this->_input == e_method::rubber) {
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
                QColor solid = _color;
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
#else
    return;
    QCursor cursor;
    if (event->type() != QEvent::TabletLeaveProximity) {
        if (event->pointerType() == QPointingDevice::PointerType::Eraser) {
            cursor = QCursor(QPixmap(":/images/cursor-eraser.png"), 3, 28);
        } else {
            switch (event->deviceType()) {
            case QInputDevice::DeviceType::Stylus:
                if (event->pointingDevice()->capabilities().testFlag(QPointingDevice::Capability::Rotation)) {
                    QImage origImg(QLatin1String(":/images/cursor-felt-marker.png"));
                    QImage img(32, 32, QImage::Format_ARGB32);
                    QColor solid = _color;
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
                } else {
                    cursor = QCursor(QPixmap(":/images/cursor-pencil.png"), 0, 0);
                }
                break;
            case QInputDevice::DeviceType::Airbrush:
                cursor = QCursor(QPixmap(":/images/cursor-airbrush.png"), 3, 4);
                break;
            default:
                break;
            }
        }
    }
    setCursor(cursor);
#endif
}


qreal TabletCanvas::pressureToWidth(qreal pressure)
{
    return pressure * 10 + 1;
}

static void loadLastMethod(TabletCanvas *p)
{

}

void TabletCanvas::triggerNewView(cbool all)
{
    data->datatouch->triggerNewView(core::get_main_window()->m_audioplayer->getPositionSecond(), all);
}

void TabletCanvas::triggerNewView(const QList<int> &Page, cbool all)
{
    this->data->datatouch->triggerNewView(Page, core::get_main_window()->m_audioplayer->getPositionSecond(), all);
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
void canvas_send_touch_event(const QPointF &pos,
                             QEvent::Type event_type, QTabletEvent::PointerType deviceType,
                             cbool now)
#else
void canvas_send_touch_event(const QPointF &pos,
                             QEvent::Type event_type, QPointingDevice::PointerType deviceType,
                             cbool now)
#endif
{
    QTabletEvent *e;
    constexpr auto not_used debugSendEvent = false;
    constexpr auto not_used name = "canvas_send_touch_event";
    auto *canvas = core::get_canvas();

    WDebug(debugSendEvent, name << event_type);

#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
    auto *pointing = new QPointingDevice("", 0, QInputDevice::DeviceType::Stylus, deviceType, QInputDevice::Capability::All, 1, 1);

    e = new QTabletEvent(event_type, pointing, pos, QPointF(), 0, 0, 0, 0, 0, 0, Qt::KeyboardModifier::NoModifier, Qt::MouseButton::NoButton, Qt::MouseButton::NoButton);

    W_ASSERT(e->type() == event_type);
    W_ASSERT(e->position() == pos);
    W_ASSERT(e->pointerType() == deviceType);

#else
    e = new QTabletEvent(event_type, pos, QPointF(), 0, deviceType, 2, 3, 3, 1, 1, 1, Qt::KeyboardModifier::NoModifier, 432243);
#endif

    if(now){
        canvas->send_touch_event(e);
        delete e;
#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
        delete pointing;
#endif
    }else{
        QApplication::postEvent(canvas, e);
    }
}

static inline bool isTabletEvent(const QEvent *event)
{
    const auto type = event->type();
    return  type != QEvent::TabletPress &&
            type != QEvent::TabletRelease &&
            type != QEvent::TabletMove;
}

bool TabletCanvas::eventFilter(QObject *ref, QEvent *e)
{
    QPointF point_touch;
    QTabletEvent *touch;
    QEvent::Type type = e->type();
    constexpr not_used bool eventFilterCanvasDebug = false;

    if(ref == _property){
        cbool isVisible = _property->isVisible();

        if(isTabletEvent(e)){
            WDebug(eventFilterCanvasDebug, "Not Touch Event" << type);
            goto out;
        }

        touch = dynamic_cast<QTabletEvent *>(e);
        const QPointF &PT = touch->position();

        if(unlikely(isVisible)){
            WDebug(eventFilterCanvasDebug, "Visible" << _property->rect() << PT);
            if(_property->rect().contains(PT.toPoint())){
                WDebug(eventFilterCanvasDebug, "Inside" << e->type());
                goto out;
            }
        }else{
            WDebug(eventFilterCanvasDebug, "Not visible");
        }

        point_touch = touch->globalPosition() - this->mapToGlobal(this->pos());

        WDebug(eventFilterCanvasDebug, "Point" << point_touch << touch->position() << _square->get_first_point() << _square->get_last_point());

        canvas_send_touch_event(point_touch, type, touch->pointerType(), true);

        // the point is out but it's visible
        if(isVisible)
            goto out;

        return true;
    }

out:
    return ref->event(e);
}
