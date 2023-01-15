#include "tabletcanvas.h"
#include <QDebug>
#include "utils/dialog_critic/dialog_critic.h"
#include "square/square.h"
#include "mainwindow.h"
#include "audioplay/audioplay.h"
#include "audiorecord/audiorecord.h"
#include "touch/laser/laser.h"
#include "utils/WCommonScript.h"
#include "touch/object_finder/object_finder.h"
#include "preview_page_widget/preview_page_widget.h"
#include "touch/dataTouch/stroke/StrokePre.h"

#define MAXPOINT 20

StrokePre __tmp;
extern bool block_scrolling;

bool need_save_auto = false;
bool need_save_tmp = false;

static TabletPenMethod lastMethod;
static QEvent::Type eventType;

bool TabletCanvas::isWriting()
{
    return !__tmp.isEmpty();
}

static inline bool is_rubber(QTabletEvent *event, const TabletPenMethod &met)
{
    W_ASSERT(event);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    const auto res = (event->pointerType() == QTabletEvent::PointerType::Eraser) or met.isRubber();
#else
    const auto res = (event->pointerType() == QPointingDevice::PointerType::Eraser) or met.isRubber();
#endif
    return res;
}

static not_used QString convert(const QEvent::Type e)
{
    switch(e){
        case QEvent::TabletPress:
            return "QEvent::TabletPress";
        case QEvent::TabletMove:
            return "QEvent::TabletMove";
        case QEvent::TabletRelease:
            return "QEvent::TabletRelease";
        default:
            std::abort();
    }
}

static force_inline bool is_out(const datastruct *data, const QPointF &point)
{
    const auto biggerX = data->biggerx();
    const auto biggerY = data->biggery();
    return  point.x() > biggerX or
            point.y() > biggerY or
            point.x() < 0.      or
            point.y() < 0.;
}

void TabletCanvas::tabletEvent(QTabletEvent *event)
{
    const QPointF& pointTouch = event->position();
    constexpr bool tabletDebug = false;

    _isWriting = true;
    need_save_auto = true;
    need_save_tmp = true;

    eventType = event->type();

    cbool isOut = is_out(getDoc(), pointTouch);

    WDebug(tabletDebug, event->type() << convert(event->type()));

    if(un(isOut)){
        /*
         * the user is writing in a part where
         *  the sheet is not present.
         * You don't have to save the point. And
         * save the end of the current treatment
         */
        WDebug(tabletDebug, QString("Height: %1 Width: %2").arg(QString::number(height()), QString::number(width())) << pointTouch);
        ManageFinish(event, true);
        goto end;
    }

    if(un(eventType == QEvent::TabletPress)){ /* first point */
        block_scrolling = true;
        ManageStart(event);
#if defined(WIN32) || defined(WIN64)
        this->isdrawing = true;
#endif
    }
    else if(likely(eventType == QEvent::TabletMove)){ /* user move the pen */
        ManageMove(event);
    }

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    else if(eventType == QEvent::TabletRelease){ /* pen leaves the tablet */
#else
    else if(eventType == QEvent::TabletRelease || eventType == QEvent::MouseButtonRelease){ /* pen leaves the tablet */
#endif
        WDebug(tabletDebug, "Tablet release");
        this->ManageFinish(event, false);
    }

end:

    if(un(!_method.isSelection() && lastMethod.isSelection())){
        WDebug(tabletDebug, "Square reset");
        _square->reset();
    }

    update();

    event->accept();
    lastMethod = _method;
}

force_inline void TabletCanvas::ManageStart(QTabletEvent *event)
{
    constexpr const auto _debug = false;
    
    if (un(m_deviceDown))
        return;

    W_ASSERT(event->type() == QEvent::TabletPress);

    _method.method()->touchBegin(event->position(),
                                 event->pressure(),
                                 *this->getDoc());

    m_deviceDown = true;
}

force_inline void TabletCanvas::ManageMove(QTabletEvent *event)
{
    //QPainter painter;

    if (event->deviceType() == QInputDevice::DeviceType::Stylus) {
        updateCursor(event);
    }

    if (un(!m_deviceDown))
        return;

    //painter.begin(&_pixmap);
    //W_ASSERT(painter.isActive());

    if (likely(_method.isInsert())) {
        updateBrush(event);
    }
    
    this->_method.method()->touchUpdate(event->position(),
                                    event->pressure(),
                                    *this->getDoc());

    //W_ASSERT(painter.isActive());
    //painter.end();
}

force_inline void TabletCanvas::ManageFinish(QTabletEvent *event, cbool isForce)
{
    int index_mod;
    block_scrolling = false;

#if defined(WIN32) || defined(WIN64)
    this->isdrawing = false;
#endif

    //W_ASSERT(0);
    index_mod = this->_method.method()->touchEnd(event->position(), *this->getDoc());

    if (index_mod >= 0) {
        core::get_main_window()->_preview_widget->mod(index_mod);
    } else if(index_mod != -1) {
        core::get_main_window()->_preview_widget->mod(-1);
    }

    if(likely(_redoundo and index_mod != -1)){
        _redoundo->copy();
    }
}

