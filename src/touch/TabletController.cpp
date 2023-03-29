#include "TabletController.h"

TabletController::TabletController(QObject *parent,
                                   const std::function<int()>& getTimeRecording)
    : QObject{parent}
    , _doc(new Document)
{
    auto objectMove = [this](const QPointF &point) { this->objectMove(point); };
    auto callUpdate = [this]() { this->callUpdate(); };
    auto getDoc = [this]() -> Document & { return this->getDoc(); };

    auto showProperty = [this] (const QPointF &point, ActionProperty prop) {
        emit this->onPropertyShow(point, prop);
    };

    auto hideProperty = [this] () {
        emit this->onPropertyHide();
    };

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
            ),
        ._square = new Square(
                this,
                hideProperty,
                showProperty,
                getDoc
                )
    };

    _objectFinder = new ObjectFinder(this, callUpdate);

    QObject::connect(_tools._square, &Square::needRefresh, this, &TabletController::onNeedRefresh);
}

void TabletController::objectMove(const QPointF &point)
{
    _objectFinder->move(point);
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
    setAndCallTool(_tools._rubber);
}

void TabletController::selectLaser()
{
    setAndCallTool(_tools._laser);
}

void TabletController::selectPen()
{
    setAndCallTool(_tools._pen);
}

void TabletController::selectSquare()
{
    setAndCallTool(_tools._square);
}

void TabletController::selectHighligter()
{
    setAndCallTool(_tools._highligter);
}

void TabletController::setAndCallTool(Tools *tool)
{
    W_ASSERT(tool != nullptr);
    auto *old = this->_currentTool;

    this->_currentTool = tool;

    if (old != tool) {
        if (old == this->_tools._square) {
            this->_tools._square->reset();
        }

        emit onToolChanged();
    }
}

void TabletController::callUpdate()
{
    emit onNeedRefresh();
}

Document &TabletController::getDoc()
{
    return *this->_doc;
}
