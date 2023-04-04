#include "TabletController.h"
#include "sheet/style_struct.h"
#include "TabletUtils.h"

extern StrokePre __tmp;

TabletController::TabletController(QObject *parent,
                                   const std::function<int()>& getTimeRecording,
                                   const std::function<bool()> &isPlaying,
                                   const std::function<int()> &getTimePlaying)
    : QObject{parent}
    , _doc(new Document)
    , _isPlaying(isPlaying)
    , _getTimePlaying(getTimePlaying)
    , _needUpdate(true)
    , _img(1, false)
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

    __tmp = StrokePre();
    _currentTool = _tools._pen;
    _objectFinder = new ObjectFinder(this, callUpdate);

    QObject::connect(_tools._square, &Square::needRefresh, this, &TabletController::onNeedRefresh);
}

const QImage &TabletController::getImg()
{
    if (this->_needUpdate or 1) {
        std::chrono::milliseconds old = std::chrono::duration_cast< std::chrono::milliseconds >(
            std::chrono::system_clock::now().time_since_epoch()
            );

        this->draw();

        const auto now = std::chrono::duration_cast< std::chrono::milliseconds >(
            std::chrono::system_clock::now().time_since_epoch()
            );

        const int diff = (now - old).count();

        //qDebug() <<"getImg() " << diff << "ms";
    }
    this->_needUpdate = false;

    W_ASSERT(!_img.isNull());

    return this->_img;
}

void TabletController::draw()
{
    if (this->getDoc().isEmpty()){
        _img = WImage(1, false);
        return;
    }

    this->_img = WImage(this->getDoc().lengthPage(), false);
    //_img.fill(Qt::white);
    TabletUtils::DataPaint d {
        .withPdf = true,
        .IsExportingPdf = true,
        .isPlay = this->_isPlaying,
        .positionAudio = this->_getTimePlaying,
        .m = 1.,
        .laser = *_tools._laser,

        DATAPAINT_DEFINEREST
    };
    Define_PAINTER_p(painter, _img);

    const auto old = std::chrono::duration_cast< std::chrono::milliseconds >(
        std::chrono::system_clock::now().time_since_epoch()
    );
    TabletUtils::load(painter, this->getDoc(), d);
    const auto now = std::chrono::duration_cast< std::chrono::milliseconds >(
        std::chrono::system_clock::now().time_since_epoch()
    );

    painter.end();
    WDebug(true, "Draw finish" << -(old - now).count());
    //getImg().save("/Users/giacomo/Desktop/tmp_foto/prova.png", "PNG");
}

void TabletController::objectMove(const QPointF &point)
{
    _objectFinder->move(point);
}

void TabletController::checkCreatePage()
{
    if (this->_doc->needToCreateNewSheet()) {
        this->_doc->newPage(n_style::square);
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

void TabletController::selectRubber()
{
    setAndCallTool(_tools._rubber);
}

void TabletController::selectLaser()
{
    setAndCallTool(_tools._laser);
}

void TabletController::selectColor(const QColor &color)
{
    this->_color = color;
}

void TabletController::positionDocChanged(const QPointF &newPosition)
{
    this->_doc->setPointFirstPage(newPosition);
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
