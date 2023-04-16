#include "TabletController.h"
#include "core/WMutexLocker.h"
#include "sheet/style_struct.h"
#include "TabletUtils.h"

extern StrokePre *__tmp;
extern bool hasDraw;

TabletController::TabletController(QObject *parent,
                                   const std::function<int()>& getTimeRecording,
                                   const std::function<bool()> &isPlaying,
                                   const std::function<int()> &getTimePlaying)
    : QObject{parent}
    , _doc(new Document)
    , _isPlaying(isPlaying)
    , _getTimePlaying(getTimePlaying)
    , _needUpdate(true)
    , _isDrawing(false)
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

    this->_toolsContainer.append(QList<Tools*>{
        _tools._square,
        _tools._pen,
        _tools._laser,
        _tools._rubber,
        _tools._highligter
    });

    *__tmp = StrokePre();
    _currentTool = _tools._pen;
    _objectFinder = new ObjectFinder(this, callUpdate);

    QObject::connect(_tools._square, &Square::needRefresh, this, &TabletController::onNeedRefresh);
}

void TabletController::getImg(QPainter &painter, double width) const
{
    if (this->_needUpdate or 1) {
        this->draw(painter, width);
    }

    this->_needUpdate = false;
}

void TabletController::draw(QPainter &painter, double width) const
{
    if (this->getDoc().isEmpty()) {
        return;
    }

    TIME_START(time_load);
    TabletUtils loader(painter, this->_isPlaying, this->_getTimePlaying, width / Page::getWidth(), *_tools._laser, getDoc(), true, false,
                       QRectF{
                           getDoc().getPointFirstPage(),
                           getDoc().getPointFirstPage() + QPointF {width, width * Page::getProportion()}
                       });


    loader.load();
    /*TabletUtils::load(painter, this->getDoc(), d);*/

    //this->_img.save("/Users/giacomo/Desktop/tmp_foto/prova.png", "PNG");
    //TIME_STOP(time_load, "Load function:");
}

void TabletController::objectMove(const QPointF &point)
{
    _objectFinder->move(point);
}

void TabletController::checkCreatePage()
{
    if (this->_doc->needToCreateNewSheet()) {
        this->_doc->newPage(n_style::square);
        emit this->onNumberOfPageChanged(_doc->lengthPage());
    }
}

void TabletController::touchBegin(const QPointF &point, double pressure)
{
    checkCreatePage();
    _currentTool->touchBegin(point, pressure, *_doc);
    emit onNeedRefresh();
}

void TabletController::touchUpdate(const QPointF &point, double pressure)
{
    _currentTool->touchUpdate(point, pressure, *_doc);
    emit onNeedRefresh();
}

void TabletController::touchEnd(const QPointF &point, double pressure)
{
    _currentTool->touchEnd(point, *_doc);
    emit onNeedRefresh();
}

void TabletController::selectColor(const QColor &color)
{
    this->_color = color;
}

void TabletController::positionDocChanged(const QPointF &newPosition)
{
    this->_doc->setPointFirstPage(-newPosition);
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

void TabletController::selectType(int type)
{
    for (auto *t: qAsConst(this->_toolsContainer)) {
        if (t->getType() == type) {
            setAndCallTool(t);
            break;
        }
    }

    _tools._square->reset();
}
