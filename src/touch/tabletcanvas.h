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
#include "rubber/rubber_ui.h"
#include "pen/pen_ui.h"
#include "../lastedit/redoundo.h"
#include "text/text_ui/text_ui.h"
#include "text/text_widgets/text_widgets.h"
#include "../sheet/fast-sheet/fast_sheet_ui.h"
#include "highlighter/highlighter.h"

class highlighter;
class MainWindow;
class square;

QT_BEGIN_NAMESPACE
class QPaintEvent;
class QString;
QT_END_NAMESPACE

struct Point {
    Point() {};
    QPointF pos;
    qreal pressure = 0;
    qreal rotation = 0;
};

/*
    DataPaint dataPaint = {
        .withPdf = false,
        .IsExportingPdf = false,
        .m = 1,
        .size = QSize(width, height),
        .parent = nullptr,
        .m_pos_ris = -1,
        .m_pixmap = nullptr,
        DATAPAINT_DEFINEREST
    };
*/

struct DataPaint{
    bool withPdf;
    bool IsExportingPdf;
    double m;
    MainWindow *parent;
    QPixmap *m_pixmap;

    QColor m_color;
    QPen pen;
    QBrush m_brush;
    struct Point lastPoint;

    void reset()
    {
        this->parent = NULL;
        this->m = 1.0;
        this->withPdf = true;
    }
#define DATAPAINT_DEFINEREST \
    .m_color = QColor(), \
    .pen = QPen(), \
    .m_brush = QBrush(), \
    .lastPoint = Point()
};

//! [0]
class TabletCanvas : public QWidget
{
    Q_OBJECT

public:
    struct Point lastPoint;

    MainWindow *parent;

    struct PointSettable pointload;
    void callResizeEvent();

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

    //void paintPixmap(QPainter &painter, QTabletEvent *event);

    /* funzione che setta il cambio dell'inserimento -> penna, gomma... */
    void sceltacolorepenna(const QColor);

    /* aggiorna la lista dei punti */
    void updatelist(QTabletEvent *event);


    Document *data = nullptr;

    enum e_method: int{
        rubber,
        pen,
        selection,
        text,
        highlighter

    };

    e_method medotodiinserimento;

    QColor m_color = Qt::black;

    static void load(QPainter &painter, const Document *data,
                     DataPaint &dataPoint);
    /*static void load(QPainter &painter, const Document *data,
                     QColor &m_color, QPen &pen,
                     QBrush &m_brush, Point &lastPoint,
                     int m_pos_ris, QPixmap *m_pixmap,
                     const bool withPdf, const double m,
                     const int size_orizzontale, const int size_verticale,
                     const MainWindow *parent, const bool IsExportingPdf);*/

    /* la funzione è responsabile del settaggio dello spessore e del tipo per il load */
    static void updateBrush_load(const double pressure, const QColor &color, QPen &m_pen, QBrush &m_brush);
    void loadpixel();

    /* touch e zoom */
    PointSettable lastpointzoom[2] = {{QPointF(0, 0), false}, {QPointF(0, 0), false}}; // [0] point left [1] point right

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
    class square *m_square = nullptr;

    bool enableredoundu = true;

    /* class for menu */
    class rubber_ui *m_rubber = NULL;
    class pen_ui *m_pen_ui = NULL;
    class text_ui *m_text = NULL;
    class highlighter *m_highlighter = NULL;
    class text_widgets *m_text_w = NULL;
    class fast_sheet_ui *m_sheet = NULL;
    class zoom_control *zoom = NULL;
    class redoundo *m_redoundo = NULL;
    class property_control *m_property = NULL;

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

    void triggerNewView(const QList<int> &Page, const bool all);
    void triggerNewView(const bool all);
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

inline void TabletCanvas::callResizeEvent()
{
    return this->resizeEvent(nullptr);
}

#endif
