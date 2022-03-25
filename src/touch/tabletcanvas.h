#ifndef TABLETCANVAS_H
#define TABLETCANVAS_H

#include <QBrush>
#include <QColor>
#include <QPen>
#include <QPixmap>
#include <QPoint>
#include <QTabletEvent>
#include <QWidget>
#include "currenttitle/document.h"
#include "zoom/zoom_control.h"
#include "rubber/rubber_ui.h"
#include "pen/pen_ui.h"
#include "lastedit/redoundo.h"
#include "text/text_ui/text_ui.h"
#include "text/text_widgets/text_widgets.h"
#include "sheet/fast-sheet/fast_sheet_ui.h"
#include "highlighter/highlighter.h"

class highlighter;
class MainWindow;
class square;

QT_BEGIN_NAMESPACE
class QPaintEvent;
class QString;
QT_END_NAMESPACE

void canvas_send_touch_event(QObject *_canvas, const QPointF &pos,
                             QEvent::Type event_type, QTabletEvent::PointerType deviceType, cbool now);

struct Point {
    Point() {};
    QPointF pos;
    qreal pressure = 0;
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

    QPen pen;
    struct Point lastPoint;

    void reset()
    {
        this->parent = NULL;
        this->m = 1.0;
        this->withPdf = true;
    }
#define DATAPAINT_DEFINEREST \
    .pen = QPen(), \
    .lastPoint = Point()
};

//! [0]
class TabletCanvas : public QWidget
{
    Q_OBJECT

private:
    static constexpr const char *name = "TabletCanvas";
public:
    static qreal pressureToWidth(qreal pressure);
    struct Point _lastPoint;

    MainWindow *_parent;

    struct PointSettable _pointload;
    void callResizeEvent();

#define default_speed_value 5
    int m_scrolling_speed = default_speed_value;
    bool m_scrolling_speed_enable = true;
    void saveScrollingSetting();
    void loadScrollinSetting();

    void scrollKinetic(QPointF first, QPointF second);
    void updateTimeScroll();

    void restoreO();

    enum Valuator { PressureValuator, TangentialPressureValuator,
                        TiltValuator, VTiltValuator, HTiltValuator, NoValuator };
        Q_ENUM(Valuator)

    TabletCanvas();
    ~TabletCanvas();

    void clear();
    force_inline void setAlphaChannelValuator(Valuator type) { m_alphaChannelValuator = type; }
    force_inline void setColorSaturationValuator(Valuator type) { m_colorSaturationValuator = type; }
    force_inline void setLineWidthType(Valuator type) { m_lineWidthValuator = type; }
    force_inline void setColor(const QColor &c) { if (c.isValid()) _color = c; }
    force_inline QColor color() const { return _color; }

    //void paintPixmap(QPainter &painter, QTabletEvent *event);

    /* funzione che setta il cambio dell'inserimento -> penna, gomma... */
    void sceltacolorepenna(const QColor);

    /* aggiorna la lista dei punti */
    void updatelist(QTabletEvent *event);


    Document *data;

    enum e_method: int{
        rubber,
        pen,
        selection,
        text,
        highlighter,
        laser
    };

    e_method _input;
    QColor _color = Qt::black;

    static void load(QPainter &painter, const Document *data,
                     DataPaint &dataPoint);

    /* la funzione è responsabile del settaggio dello spessore e del tipo per il load */
    static void updateBrush_load(const double pressure, const QColor &color, QPen &m_pen);
    void loadpixel();
    void call_update();

    /* touch e zoom */
    // [0] point left [1] point right
    PointSettable lastpointzoom[2];

    QPointF posizionezoom_puntof;
    int deltax;
    int deltay;
#if defined (WIN32) || defined(WIN64)
    bool isdrawing = false;
#endif

    /* gestisce sia la parte di spostamento con il touch che con la rotella del mouse */
    struct PointSettable _ismoving;
    void ismoving_f();
    void updatePageCount();

    /* struttura per ricordare il last point per il touch */
    struct PointSettable lastpointtouch;

    void setTabletDevice(QTabletEvent *event) { updateCursor(event); }

    /* class for menu */
    class square            *_square        = NULL;
    class rubber_ui         *_rubber        = NULL;
    class pen_ui            *_pen_ui        = NULL;
    class text_ui           *_text          = NULL;
    class highlighter       *_highlighter   = NULL;
    class text_widgets      *_text_w        = NULL;
    class fast_sheet_ui     *_sheet         = NULL;
    class zoom_control      *_zoom          = NULL;
    class redoundo          *_redoundo      = NULL;
    class property_control  *_property      = NULL;
    class laser             *_laser         = NULL;
    class object_finder     *_finder        = NULL;

    QPixmap _pixmap;

    void send_touch_event(QTabletEvent *event);
    void send_mouse_event(QMouseEvent *event);

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
    bool eventFilter(QObject *ref, QEvent *e) override;
private:

    void triggerNewView(const QList<int> &Page, const bool all);
    void triggerNewView(const bool all);
    bool isWriting = false;

    void updateCursor(const QTabletEvent *event);
    void updateBrush(const QTabletEvent *event);

    /* if paint == true launch update() */
    void initPixmap(bool paint);
    Qt::BrushStyle brushPattern(qreal value);

    Valuator m_alphaChannelValuator = TangentialPressureValuator;
    Valuator m_colorSaturationValuator = NoValuator;
    Valuator m_lineWidthValuator = PressureValuator;

    QPen m_pen;

    bool m_deviceDown = false;
    void ManageFinish(QTabletEvent *event, cbool isForce);
    void ManageStart(QTabletEvent *event, const QPointF &point);
    void ManageMove(QTabletEvent *event, const QPointF &point);

public slots:

    void RedoM(){
        this->update();
    }
    void Undo(){
        this->update();
    }

};

force_inline void TabletCanvas::callResizeEvent()
{
    return this->resizeEvent(nullptr);
}

force_inline void TabletCanvas::call_update()
{
    update();
}

force_inline void TabletCanvas::send_touch_event(QTabletEvent *event)
{
    return this->tabletEvent(event);
}

force_inline void TabletCanvas::send_mouse_event(QMouseEvent *event)
{
    return this->mouseMoveEvent(event);
}

#endif
