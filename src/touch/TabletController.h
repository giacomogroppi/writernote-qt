#pragma once

#include "touch/highlighter/Highligter.h"
#include "touch/laser/Laser.h"
#include "touch/pen/Pen.h"
#include "touch/rubber/Rubber.h"
#include "object_finder/ObjectFinder.h"
#include "touch/square/Square.h"
#include "core/WImage.h"
#include <atomic>
#include "touch/tools/Tools.h"

class TabletController : public WObject
{
    Q_OBJECT
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
    colore_s _color;
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
    void draw(WPainter &painter, double width) const;
public:
    explicit TabletController(WObject *parent,
                              const std::function<int()>& getTimeRecording,
                              const std::function<bool()> &isPlaying,
                              const std::function<int()> &getTimePlaying);

    void getImg(WPainter &painter, double width) const;

    Tools* getCurrentTool() const;

public slots:
    void selectType(int type);
    void selectColor(const colore_s &color);

    void positionDocChanged(const PointF &newPosition);

    void touchBegin(const PointF &point, double pressure);
    void touchUpdate(const PointF &point, double pressure);
    void touchEnd(const PointF &point, double pressure);

signals:
    void onNeedRefresh(int pageMin, int pageMax);
    void onToolChanged();
    void onPropertyHide();
    void onNumberOfPageChanged(int numerberOfPage);
    void onPropertyShow(const PointF &point, ActionProperty);
};

inline const Document &TabletController::getDoc() const
{
    return *this->_doc;
}

inline Tools *TabletController::getCurrentTool() const
{
    return this->_currentTool;
}

