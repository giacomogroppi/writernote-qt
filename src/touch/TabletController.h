#pragma once

#include "touch/highlighter/Highligter.h"
#include "touch/laser/Laser.h"
#include "touch/pen/Pen.h"
#include "touch/rubber/Rubber.h"
#include "object_finder/ObjectFinder.h"
#include "touch/square/Square.h"
#include "core/Image/WImage.h"
#include <atomic>
#include "touch/tools/Tools.h"
#include "Scheduler/WObject.h"
#include "touch/UpdateEvent.h"

class TabletController : public WObject
{
private:
    struct {
        Highligter *_highligter;
        Pen *_pen;
        Rubber *_rubber;
        Laser *_laser;
        Square *_square;
    } _tools;

    WListFast<Tools*> _toolsContainer;

    ObjectFinder *_objectFinder;
    Tools *_currentTool;
    WColor _color;
    WPen _pen;
    Document *_doc;

    mutable bool _needUpdate;

    mutable std::atomic<bool> _isDrawing;

    void objectMove(const PointF &point);
    void setAndCallTool(Tools *tool);

    Document &getDoc();
    const Document &getDoc() const;

    const std::function<bool()> _isPlaying;
    const std::function<int()> _getTimePlaying;
    void checkCreatePage();
    void draw(WPainter &painter, double width, WFlags<UpdateEvent::UpdateEventType> flags) const;
public:
    explicit TabletController(WObject *parent,
                              const std::function<int()>  &getTimeRecording,
                              const std::function<bool()> &isPlaying,
                              const std::function<int()>  &getTimePlaying);

    void getImg(WPainter &painter, double width, WFlags<UpdateEvent::UpdateEventType> flags) const;
    void getImagePage(WPainter &painter, double width) const;
    void getImageStroke(WPainter &painter, double width) const;
    
    Tools* getCurrentTool() const;

    DEFINE_LISTENER(selectType(int type));
    DEFINE_LISTENER(selectColor(const WColor &color));

    DEFINE_LISTENER(positionDocChanged(const PointF &newPosition));

    DEFINE_LISTENER(touchBegin(const PointF &point, double pressure));
    DEFINE_LISTENER(touchUpdate(const PointF &point, double pressure));
    DEFINE_LISTENER(touchEnd(const PointF &point, double pressure));

    W_EMITTABLE_1(onNeedRefresh, const UpdateEvent&, event);

    W_EMITTABLE_0(onToolChanged);
    W_EMITTABLE_0(onPropertyHide);
    W_EMITTABLE_1(onNumberOfPageChanged, int, numerberOfPage);
    W_EMITTABLE_2(onPropertyShow, const PointF&, point, ActionProperty, actionProperty);
};

inline const Document &TabletController::getDoc() const
{
    return *this->_doc;
}

inline Tools *TabletController::getCurrentTool() const
{
    return this->_currentTool;
}

