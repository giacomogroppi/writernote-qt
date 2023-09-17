#include "TabletController.h"
#include "core/WMutexLocker.h"
#include "sheet/style_struct.h"
#include "TabletUtils.h"

extern StrokePre *__tmp;
extern bool hasDraw;

TabletController::TabletController(WObject *parent,
                                   const Fn<int()>& getTimeRecording,
                                   const Fn<bool()> &isPlaying,
                                   const Fn<int()> &getTimePlaying)
    : WObject{parent}
    , _doc(new Document)
    , _needUpdate(true)
    , _isDrawing(false)
    , _isPlaying(isPlaying)
    , _getTimePlaying(getTimePlaying)
{
    // auto callUpdate = [this]() { W_EMIT_1(onNeedRefresh, UpdateEvent::makeAll()); };
    auto getDoc = [this]() -> Document & { return this->getDoc(); };

    /**
     * Dobbiamo instanzare lo strokePre in questo punto devo codice
     * perchè vogliamo assicurare a tutto il codice che accede a tale
     * oggetti che esso è valido. --> [non vogliamo controllare che sia nullo :)]
    */
    __tmp = new StrokePre([this] {
        W_EMIT_1(onNeedRefresh, UpdateEvent::makeStroke());
    });

    auto showProperty = [this] (const PointF &point, ActionProperty prop) {
        W_EMIT_2(onPropertyShow, point, prop);
    };

    auto hideProperty = [this] () {
        W_EMIT_0(onPropertyHide);
    };

    this->_tools = {
        ._highligter = new Highlighter (
                this,
                getTimeRecording,
                _color,
                _pen
            ),
        ._pen = new Pen (
                this,
                getTimeRecording,
                _color,
                _pen
            ),
        ._rubber = new Rubber (
                this
            ),
        ._laser = new Laser (
                this,
                [] (double) -> pressure_t { return 10.; },
                _color,
                _pen,
                [this] { W_EMIT_1(onNeedRefresh, UpdateEvent::makeLaser()); }
            ),
        ._square = new Square(
                this,
                hideProperty,
                showProperty,
                getDoc,
                [this] (UpdateEvent event) { W_EMIT_1(onNeedRefresh, event); }
            )
    };

    this->_toolsContainer.append(WListFast<Tools*>{
        _tools._square,
        _tools._pen,
        _tools._laser,
        _tools._rubber,
        _tools._highligter
    });

    __tmp->reset();
    _currentTool = _tools._pen;
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
