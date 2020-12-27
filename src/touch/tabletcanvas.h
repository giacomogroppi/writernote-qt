#ifndef TABLETCANVAS_H
#define TABLETCANVAS_H

#include <QBrush>
#include <QColor>
#include <QPen>
#include <QPixmap>
#include <QPoint>
#include <QTabletEvent>
#include <QWidget>

#include "datastruct/datastruct.h"
#include "square/square.h"

typedef struct lastpoint_struct{
    QPointF posd = QPointF(-1, -1);
    QPointF poss = QPointF(-1, -1);
}lastpoint_t;

typedef struct{
    short int deltay;
    short int deltax;

    bool ismoving_check = false;
}ismoving_t;

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
    void gomma(QPainter &painter);
    QColor m_color = Qt::black;

    void loadfile();
    bool isloading = false;

    struct PointLoading{
        int xdrawing;
        int ydrawing;
    } pointload;
    void laod(QPainter *);

    /* la funzione è responsabile del settaggio dello spessore e del tipo per il load */
    void updateBrush_load(float pressure, int yTilt, int xTilt, QColor );
    void loadpixel();

    /* funzioni responsabili del riascolto */
    void riascolto(int);
    void loadriascolto(QPainter *);
    bool riascoltovariable = false;
    int posizione = -1;

    /* touch e zoom */
    lastpoint_t lastpointzoom;
    void zoomin(QPaintEvent *);
    void zoomon(QPaintEvent *);

    bool iszoomin = false;
    bool iszoomon = false;

    QPointF posizionezoom_puntof;
    int deltax;
    int deltay;

    /* gestisce sia la parte di spostamento con il touch che con la rotella del mouse */
    ismoving_t ismoving;
    void ismoving_f(QPaintEvent *event, QPainter *);

    void disegnafoglio();
    bool disegnofoglio_bool = false;

    /* struttura per ricordare il last point per il touch */
    struct PointLoading lastpointtouch;

    void setTabletDevice(QTabletEvent *event)
     { updateCursor(event); }

    void settingdata(datastruct *data);

    /* square per la selezione */
    square square_;

protected:
    void tabletEvent(QTabletEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

    /* rotella del mouse */
    void wheelEvent(QWheelEvent *event) override;

    /* gestisce il touch */
    bool event(QEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

    /* click rilasciato */
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
private:
    void updateCursor(const QTabletEvent *event);
    void updateBrush(const QTabletEvent *event);

    void initPixmap(int delta);
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
