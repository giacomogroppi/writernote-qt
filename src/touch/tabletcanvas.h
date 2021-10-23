#ifndef TABLETCANVAS_H
#define TABLETCANVAS_H

#include <QBrush>
#include <QColor>
#include <QPen>
#include <QPixmap>
#include <QPoint>
#include <QTabletEvent>
#include <QWidget>
#include "../currenttitle/document.h"
#include "zoom/zoom_control.h"
#include "square/square.h"
#include "rubber/rubber_ui.h"
#include "pen/pen_ui.h"
#include "../lastedit/redoundo.h"
#include "text/text_ui/text_ui.h"
#include "text/text_widgets/text_widgets.h"
#include "../sheet/fast-sheet/fast_sheet_ui.h"
#include "highlighter/highlighter.h"

class highlighter;
class MainWindow;

QT_BEGIN_NAMESPACE
class QPaintEvent;
class QString;
QT_END_NAMESPACE

//! [0]
class TabletCanvas : public QWidget
{
    Q_OBJECT

public:
    struct Point {
        QPointF pos;
        qreal pressure = 0;
        qreal rotation = 0;
    } lastPoint;

    MainWindow *parent;

    struct PointSettable pointload;


#define default_speed_value 5
    int m_scrolling_speed = default_speed_value;
    bool m_scrolling_speed_enable = true;
    void saveScrollingSetting();
    void loadScrollinSetting();

    void scrollKinetic(QPointF first, QPointF second);
    void updateTimeScroll();

    void restoreO();
    inline void needUpdate(){
        update();
    };

    enum Valuator { PressureValuator, TangentialPressureValuator,
                        TiltValuator, VTiltValuator, HTiltValuator, NoValuator };
        Q_ENUM(Valuator)

    TabletCanvas();
    ~TabletCanvas();

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
    void sceltacolorepenna(const QColor);

    /* aggiorna la lista dei punti */
    void updatelist(QTabletEvent *event);


    Document *data = nullptr;

    /* al passaggio a mainwindow il puntatore verrà cambiato con il valore del tempo della registrazione se e solo se sta registrando
    in caso contrario rimarrà sempre zero*/
    int time = 0;

    enum e_method: int{
        rubber,
        pen,
        selection,
        text,
        highlighter

    };

    e_method medotodiinserimento;

    QColor m_color = Qt::black;

    static void load(QPainter &,
                     const Document *data,
                     QColor &m_color,
                     QPen &pen,
                     QBrush &m_brush,
                     Point &lastPoint,
                     int m_pos_ris,
                     QPixmap *m_pixmap,
                     const bool withPdf,
                     const double m,
                     const int size_orizzontale,
                     const int size_verticale,
                     const MainWindow *parent,
                     const bool IsExportingPdf);

    /* la funzione è responsabile del settaggio dello spessore e del tipo per il load */
    static void updateBrush_load(const double pressure, const QColor &color,
                                 const TabletCanvas::Valuator m_lineWidthValuator, QPen &m_pen, QBrush &m_brush);
    void loadpixel();

    /* funzioni responsabili del riascolto */
    void riascolto(int);

    /*
     * indice la posizione del replay
    */
    int m_pos_ris = -1;

    /* touch e zoom */
    PointSettable lastpointzoom[2] = {{QPointF(0, 0), false}, {QPointF(0, 0), false}};

    QPointF posizionezoom_puntof;
    int deltax;
    int deltay;
#if defined (WIN32) || defined(WIN64)
    bool isdrawing = false;
#endif

    /* gestisce sia la parte di spostamento con il touch che con la rotella del mouse */
    struct PointSettable ismoving;
    void ismoving_f();

    /* struttura per ricordare il last point per il touch */
    struct PointSettable lastpointtouch;

    void setTabletDevice(QTabletEvent *event)
     { updateCursor(event); }

    void settingdata(Document *data);

    /* square per la selezione */
    class square m_square;

    bool enableredoundu = true;

    /* class for menu */
    class rubber_ui *m_rubber;
    class pen_ui *m_pen_ui;
    class text_ui *m_text;
    class highlighter *m_highlighter;
    text_widgets *m_text_w;
    fast_sheet_ui *m_sheet;
    zoom_control *zoom = NULL;
    redoundo *m_redoundo = NULL;
    QPixmap m_pixmap;

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


    QBrush m_brush;
    QPen m_pen;

    bool m_deviceDown = false;
    inline void ManageFinish(QTabletEvent *event);
    inline void ManageStart(QTabletEvent *event, const QPointF &point);

    void updatePageCount();

public slots:

    void RedoM(){
        this->update();
    }
    void Undo(){
        this->update();
    }

};
//! [0]

#endif
