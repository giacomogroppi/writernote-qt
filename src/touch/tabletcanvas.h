#ifndef TABLETCANVAS_H
#define TABLETCANVAS_H

#include <QBrush>
#include <QColor>
#include <QPen>
#include <QPixmap>
#include <QPoint>
#include <QTabletEvent>
#include <QWidget>

#include "../currenttitle/currenttitle_class.h"
#include "zoom/zoom_control.h"
#include "square/square.h"

#include "rubber/rubber_ui.h"
#include "pen/pen_ui.h"

#define IDTRATTOZERO -5

#include "../lastedit/redoundo.h"
#include "../autosave/autosave_.h"

#include "text/text_ui/text_ui.h"
#include "text/text_widgets/text_widgets.h"
#include "../sheet/fast-sheet/fast_sheet_ui.h"

typedef struct lastpoint_struct{
    QPointF posd = QPointF(-1, -1);
    QPointF poss = QPointF(-1, -1);
}lastpoint_t;

typedef struct{
    double deltay;
    double deltax;
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
    ~TabletCanvas(){
        delete zoom;
    };

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


    currenttitle_class *data;

    /* al passaggio a mainwindow il puntatore verrà cambiato con il valore del tempo della registrazione se e solo se sta registrando
    in caso contrario rimarrà sempre zero*/
    int time = 0;

    /* 0 penna
       1 gomma*/
    int medotodiinserimento;

    QColor m_color = Qt::black;

    void loadfile();
    bool isloading = false;

    struct PointLoading{
        int xdrawing;
        int ydrawing;
    } pointload;

#define DEFAULT_PASS_ARGUMENT_LOAD -1
    void load(QPainter *,
              double m = (double)1,
              int size_orizzontale = DEFAULT_PASS_ARGUMENT_LOAD,
              int size_verticale = DEFAULT_PASS_ARGUMENT_LOAD,
              double *y_last = NULL);

    /* la funzione è responsabile del settaggio dello spessore e del tipo per il load */
    void updateBrush_load(float , QColor );
    void loadpixel();

    /* funzioni responsabili del riascolto */
    void riascolto(int);
    void loadriascolto(QPainter *);
    bool riascoltovariable = false;
    int posizione = -1;

    /* touch e zoom */
    lastpoint_t lastpointzoom;

    QPointF posizionezoom_puntof;
    int deltax;
    int deltay;
#if defined (WIN32) || defined(WIN64)
    bool isdrawing = false;
#endif

    /* gestisce sia la parte di spostamento con il touch che con la rotella del mouse */
    ismoving_t ismoving;
    void ismoving_f();

    void disegnafoglio();
    bool disegnofoglio_bool = false;

    /* struttura per ricordare il last point per il touch */
    struct PointLoading lastpointtouch;

    void setTabletDevice(QTabletEvent *event)
     { updateCursor(event); }

    void settingdata(currenttitle_class *data, QString &path);

    /* square per la selezione */
    square square_;

    bool enableredoundu = true;

    /* class for menu */
    rubber_ui *m_rubber;
    pen_ui *m_pen_ui;
    text_ui *m_text;

    text_widgets *m_text_w;

    fast_sheet_ui *m_sheet;

    zoom_control *zoom = NULL;

    /* autosave */
    autosave_ *m_autosave = NULL;
    void setAutoSave(bool v, QString &path);

    /* redoundo */
    redoundo *m_redoundo = NULL;

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
    bool isWriting = false;

    void updateCursor(const QTabletEvent *event);
    void updateBrush(const QTabletEvent *event);

    /* if paint == true launch update() */
    void initPixmap(bool paint);
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

    void updatePageCount();

public slots:

#define PRIVATE_UPDATE this->isloading = true; this->update()

    void RedoM(){
        PRIVATE_UPDATE;
    }
    void Undo(){
        PRIVATE_UPDATE;
    }

signals:
    void updatePageCount_s(int);

};
//! [0]

#endif
