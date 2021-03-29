#include "tabletcanvas.h"

#include <QCoreApplication>
#include <QPainter>
#include <QtMath>
#include <cstdlib>

#include <QTabletEvent>
#include <QEvent>

#include "datastruct/datastruct.h"
#include "../utils/setting_define.h"
#include <QSettings>

static void saveLastMethod(TabletCanvas::e_method);
static void loadLastMeghod(TabletCanvas *);

void TabletCanvas::restoreO()
{
    this->data->datatouch->repositioning();
    this->isloading = true;
    update();

    lastpointtouch.set = false;

}

TabletCanvas::TabletCanvas()
    : QWidget(nullptr), m_brush(m_color)
    , m_pen(m_brush, 1.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin)
{

    resize(500, 500);
    setAutoFillBackground(true);
    setAttribute(Qt::WA_TabletTracking);

    this->data = nullptr;
    this->medotodiinserimento = e_method::pen;

    if(!zoom){
        zoom = new zoom_control;
    }

    m_redoundo = new redoundo(data);

    loadLastMeghod(this);

}

TabletCanvas::~TabletCanvas(){
    if(zoom)
        delete zoom;

    saveLastMethod(this->medotodiinserimento);
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
                if (event->device() == QTabletEvent::Airbrush)
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
            m_brush.setColor(Qt::white);
            m_pen.setColor(Qt::white);
            m_pen.setWidthF(event->pressure()/2 * 10 + 1);
        } else {
            m_brush.setColor(m_color);
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
            switch (event->device()) {
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


void TabletCanvas::settingdata(currenttitle_class *data, QString &path){
    this->data = data;
    this->zoom->settingData(data->datatouch);

    if(this->m_autosave)
        m_autosave->setting_data(data, path);

    if(this->m_redoundo)
        m_redoundo->setting_data(data);

}

void TabletCanvas::setAutoSave(bool v, QString &path){
    /* v == true -> set autosave true */
    if(v){
        if(!m_autosave)
            m_autosave = new autosave_;
        m_autosave->setting_data(data, path);
    }
    else{
        if(m_autosave){
            delete m_autosave;
            m_autosave = NULL;
        }

    }
};


static void saveLastMethod(TabletCanvas::e_method val){
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_METHOD_TOUCH);

    setting.setValue(KEY_METHOD_TOUCH, val);
    setting.endGroup();

}

static void loadLastMeghod(TabletCanvas *p){
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);

    setting.beginGroup(GROUPNAME_METHOD_TOUCH);

    p->medotodiinserimento = static_cast<TabletCanvas::e_method>(setting.value(KEY_METHOD_TOUCH, TabletCanvas::pen).toInt());

    setting.endGroup();

}

