#ifndef TABLETCANVAS_H
#define TABLETCANVAS_H

#include <QBrush>
#include <QColor>
#include <QPen>
#include <QPixmap>
#include <QPoint>
#include <QTabletEvent>
#include <QWidget>

/* struttura della classe per memorizzare i dati */
#include "datastruct/datastruct.h"

QT_BEGIN_NAMESPACE
class QPaintEvent;
class QString;
QT_END_NAMESPACE

//! [0]
class TabletCanvas : public QWidget
{
    Q_OBJECT

public:
    enum Valuator { PressureValuator, TangentialPressureValuator,
                    TiltValuator, VTiltValuator, HTiltValuator, NoValuator };
    Q_ENUM(Valuator)

    TabletCanvas();

    bool saveImage(const QString &file);
    bool loadImage(const QString &file);
    void clear();
    void setAlphaChannelValuator(Valuator type)
        { m_alphaChannelValuator = type; }
    void setColorSaturationValuator(Valuator type)
        { m_colorSaturationValuator = type; }
    void setLineWidthType(Valuator type)
        { m_lineWidthValuator = type; }
    void setColor(const QColor &c)
        { if (c.isValid()) m_color = c; }
    QColor color() const
        { return m_color; }

    void paintPixmap(QPainter &painter, QTabletEvent *event);

    /* funzione che setta il cambio dell'inserimento -> penna, gomma... */
    void sceltacolorepenna(QColor);

    /* aggiorna la lista dei punti */
    void updatelist(QTabletEvent *event);

    /* la funzione gestisce la gomma */
    void gomma_f(QTabletEvent *event);

    datastruct *data;

    /* al passaggio a mainwindow il puntatore verrà cambiato con il valore del tempo della registrazione se e solo se sta registrando
    in caso contrario rimarrà sempre zero*/
    int time = 0;

    /* 0 penna
       1 gomma*/
    int medotodiinserimento;
    QColor m_color = Qt::red;

    void loadfile();
    bool isloading = false;

    struct PointLoading{
        int xdrawing;
        int ydrawing;
    } pointload;
    void laod(QPaintEvent*, QPainter *);
    void loadpixel();

protected:
    void tabletEvent(QTabletEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void initPixmap();
    Qt::BrushStyle brushPattern(qreal value);
    static qreal pressureToWidth(qreal pressure);

    Valuator m_alphaChannelValuator = TangentialPressureValuator;
    Valuator m_colorSaturationValuator = NoValuator;
    Valuator m_lineWidthValuator = PressureValuator;

    QPixmap m_pixmap;
    QBrush m_brush;
    QPen m_pen;
    bool m_deviceDown = false;

    struct Point {
        QPointF pos;
        qreal pressure = 0;
        qreal rotation = 0;
    } lastPoint;
};
//! [0]

#endif
