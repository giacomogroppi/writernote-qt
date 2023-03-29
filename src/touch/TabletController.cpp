#include "TabletController.h"

TabletController::TabletController(QObject *parent,
                                   const std::function<int()>& getTimeRecording)
    : QObject{parent}
    , _doc(new Document)
{
    auto objectMove = [this]() { this->objectMove(); };
    auto callUpdate = [this]() { this->callUpdate(); };

    this->_tools = {
        ._highligter = new Highligter (
                this,
                getTimeRecording,
                objectMove,
                _color,
                _pen
            ),
        ._pen = new Pen (
                this,
                getTimeRecording,
                objectMove,
                _color,
                _pen
            ),
        ._rubber = new Rubber (
                this
            ),
        ._laser = new Laser (
                this,
                [] (double) -> pressure_t { return 10.; },
                objectMove,
                _color,
                _pen,
                callUpdate
            )
    };
}

void TabletController::objectMove()
{
}

void TabletController::touchBegin(const QPointF &point, double pressure)
{
    _currentTool->touchBegin(point, pressure, *_doc);
}

void TabletController::touchUpdate(const QPointF &point, double pressure)
{
    _currentTool->touchBegin(point, pressure, *_doc);
}

void TabletController::touchEnd(const QPointF &point, double pressure)
{
    _currentTool->touchBegin(point, pressure, *_doc);
}

void TabletController::selectRubber()
{

}

void TabletController::setAndCallTool(Tools *tool)
{
    W_ASSERT(tool != nullptr);
    auto *now = this->_currentTool;

    this->_currentTool = tool;

    if (now != tool)
        emit onToolChanged();
}
