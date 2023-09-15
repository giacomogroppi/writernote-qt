#include "TabletController.h"
#include "core/WMutexLocker.h"
#include "sheet/style_struct.h"
#include "TabletUtils.h"

extern StrokePre *__tmp;
extern bool hasDraw;

TabletController::TabletController(WObject *parent,
                                   const std::function<int()>& getTimeRecording,
                                   const std::function<bool()> &isPlaying,
                                   const std::function<int()> &getTimePlaying)
    : WObject{parent}
    , _doc(new Document)
    , _needUpdate(true)
    , _isDrawing(false)
    , _isPlaying(isPlaying)
    , _getTimePlaying(getTimePlaying)
{
    auto objectMove = [this](const PointF &point) { this->objectMove(point); };
    auto callUpdate = [this]() { W_EMIT_1(onNeedRefresh, UpdateEvent::makeAll()); };
    auto getDoc = [this]() -> Document & { return this->getDoc(); };

    auto showProperty = [this] (const PointF &point, ActionProperty prop) {
        W_EMIT_2(onPropertyShow, point, prop);
    };

    auto hideProperty = [this] () {
        W_EMIT_0(onPropertyHide);
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
                getDoc,
                callUpdate
            )
    };

    this->_toolsContainer.append(WListFast<Tools*>{
        _tools._square,
        _tools._pen,
        _tools._laser,
        _tools._rubber,
        _tools._highligter
    });

    *__tmp = StrokePre();
    _currentTool = _tools._pen;
    _objectFinder = new ObjectFinder(this, callUpdate);
}

// TODO: to remove
void TabletController::getImageStroke(WPainter &painter, double width) const
{
    if (this->getDoc().isEmpty()) {
        return;
    }

    TIME_START(time_load);

    TabletUtils loader(
            painter, this->_isPlaying,
            this->_getTimePlaying, width / Page::getWidth(),
            Optional(_tools._laser), getDoc(),
            true, false,
            RectF {
                // top left of the page
                getDoc().getPointFirstPage(),

                // bottom right of the page
                getDoc().getPointFirstPage() + PointF { width, width * Page::getProportion() }
            }
    );

    loader.load(WFlags{UpdateEvent::stroke});

    //this->_img.write("/Users/giacomo/Desktop/tmp_foto/prova.png", "PNG");
    
    // TODO: remove this
    TIME_STOP(time_load, "Load function:");
}

// TODO: to remove
void TabletController::getImagePage(WPainter &painter, double width) const
{
    if (this->getDoc().isEmpty()) {
        return;
    }

    TIME_START(time_load);

    TabletUtils loader(
            painter, this->_isPlaying,
            this->_getTimePlaying, width / Page::getWidth(),
            Optional(_tools._laser), getDoc(),
            true, false,
            RectF {
                // top left of the page
                getDoc().getPointFirstPage(),

                // bottom right of the page
                getDoc().getPointFirstPage() + PointF { width, width * Page::getProportion() }
            }
    );

    loader.load(UpdateEvent::page | UpdateEvent::sheet);

    //this->_img.write("/Users/giacomo/Desktop/tmp_foto/prova.png", "PNG");
    
    // TODO: remove this
    TIME_STOP(time_load, "Load function:");
}

void TabletController::getImg(WPainter &painter, double width, WFlags<UpdateEvent::UpdateEventType> flags) const
{
    this->draw(painter, width, flags);
    return;
    if (this->_needUpdate or 1) {

    }

    this->_needUpdate = false;
}

void TabletController::draw(WPainter &painter, double width, WFlags<UpdateEvent::UpdateEventType> flags) const
{
    if (this->getDoc().isEmpty()) {
        return;
    }

    TIME_START(time_load);

    TabletUtils loader(
            painter, this->_isPlaying,
            this->_getTimePlaying, width / Page::getWidth(),
            Optional(_tools._laser), getDoc(),
            true, false,
            RectF {
                // top left of the page
                getDoc().getPointFirstPage(),

                // bottom right of the page
                getDoc().getPointFirstPage() + PointF { width, width * Page::getProportion() }
            }
    );

    loader.load(flags);

    //this->_img.write("/Users/giacomo/Desktop/tmp_foto/prova.png", "PNG");
    
    // TODO: remove this
    TIME_STOP(time_load, "Load function:");
}

void TabletController::objectMove(const PointF &point)
{
    _objectFinder->move(point);
}

void TabletController::checkCreatePage()
{
    if (this->_doc->needToCreateNewSheet()) {
        this->_doc->newPage(n_style::square);
        W_EMIT_1(onNumberOfPageChanged, _doc->lengthPage());
    }
}

void TabletController::touchBegin(const PointF &point, double pressure)
{
    checkCreatePage();
    const auto res = _currentTool->touchBegin(point, pressure, *_doc);
    W_EMIT_1(onNeedRefresh, res);
}

void TabletController::touchUpdate(const PointF &point, double pressure)
{
    const auto res = _currentTool->touchUpdate(point, pressure, this->getDoc());

    W_EMIT_1(onNeedRefresh, res);
}

void TabletController::touchEnd(const PointF &point, double pressure)
{
    const auto res = _currentTool->touchEnd(point, *_doc);

    W_EMIT_1(onNeedRefresh, res);
}

void TabletController::selectColor(const WColor &color)
{
    this->_color = color;
}

void TabletController::positionDocChanged(const PointF &newPosition)
{
    this->_doc->setPointFirstPage(newPosition * (-1.));
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

        W_EMIT_0(onToolChanged);
    }
}

Document &TabletController::getDoc()
{
    return *this->_doc;
}

void TabletController::selectType(int type)
{
    for (auto *t: std::as_const(this->_toolsContainer)) {
        if (t->getType() == type) {
            setAndCallTool(t);
            break;
        }
    }

    _tools._square->reset();
}
