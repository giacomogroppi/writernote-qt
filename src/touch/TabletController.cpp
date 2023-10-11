#include "TabletController.h"

#include <memory>
#include <utility>
#include "core/WMutexLocker.h"
#include "sheet/style_struct.h"
#include "TabletUtils.h"
#include "Scheduler/Scheduler.h"

extern StrokePre *__tmp;
extern bool hasDraw;

TabletController::TabletController(WObject *parent,
                                   const WPath& defaultPathSaving,
                                   Fn<void(const WString&)> showGenericError)
    : WObject{parent}
    , _showGenericError(std::move(showGenericError))
    , _tools()
    , _settings()
    , _fileManager(nullptr)
    , _color(color_black)
    , _doc(new Document)
    , _audioRecorder(new AudioRecord(nullptr, _showGenericError))
    , _audioPlayer(new AudioPlayer(nullptr))
    , _needUpdate(true)
    , _isDrawing(false)
{
    // auto callUpdate = [this]() { W_EMIT_1(onNeedRefresh, UpdateEvent::makeAll()); };
    auto getDoc = [this] () -> Document & { return this->getDoc(); };
    auto getTimeRecording = [this] { return this->_audioRecorder->getCurrentTime(); };
    auto getTimePlayer    = [this] { return this->_audioPlayer->getCurrentTimeSecond(); };
    auto isRecording = [this] { return this->_audioRecorder->isRecording(); };
    auto isPlaying =   [this] { return this->_audioPlayer->isPlaying(); };

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
        ._highlighter = new Highlighter (
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

    this->_toolsContainer.append(WListFast<Tools*> {
        _tools._square,
        _tools._pen,
        _tools._laser,
        _tools._rubber,
        _tools._highlighter
    });

    __tmp->reset();
    _currentTool = _tools._pen;

    this->_settings.begin();

    this->_fileManager = std::make_unique<FileManager>(
            nullptr,
            WPath(
                    _settings.value(
                        WOptionSettings::namePathSaving,
                        defaultPathSaving.toString()
                    ).toString()),
            true
    );

    this->_settings.save();
}

// TODO: to remove
void TabletController::getImageStroke(WPainter &painter, double width) const
{
    if (this->getDoc().isEmpty()) {
        return;
    }

    TIME_START(time_load);

    TabletUtils loader(
            painter,
            this->getAudioPlayer().isPlaying(),
            this->getAudioPlayer().getCurrentTimeSecond(),
            Double(width / Page::getWidth()),
            Optional(_tools._laser),
            getDoc(),
            Bool(true),
            Bool(false),
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
            painter,
            getAudioPlayer().isPlaying(),
            getAudioPlayer().getCurrentTimeSecond(),
            Double(width / Page::getWidth()),
            Optional(_tools._laser), getDoc(),
            Bool(true),
            Bool(false),
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
            painter, getAudioPlayer().isPlaying(),
            getAudioPlayer().getCurrentTimeSecond(), Double(width / Page::getWidth()),
            Optional(_tools._laser), getDoc(),
            Bool(true), Bool(false),
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

auto TabletController::getDoc() -> Document&
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

auto TabletController::getCurrentPathSaving() const -> WPath
{
    return this->_fileManager->getCurrentPath();
}

TabletController::~TabletController()
{
    _settings.begin();
    _settings.setValue(WOptionSettings::namePathSaving, _fileManager->getCurrentPath().toString());
    _settings.save();
}

void TabletController::setCurrentPathSaving(WString path)
{
    this->_fileManager->moveTo(WPath(std::move(path)));
}

auto TabletController::getFileManager() -> const SharedPtr<FileManager>&
{
    return this->_fileManager;
}

auto TabletController::openFile(const WString &name) -> int
{
    auto data = _fileManager->openFile<Document>(name, Extension::makeWriternote());
    const auto methodUpdate = [this] {
        W_EMIT_1(onNeedRefresh, UpdateEvent::makeAll());
    };

    if (data.first < 0)
        return -1;

    *this->_doc = std::move (data.second);
    this->_nameDoc = name;

    // we need to give the ui the time to switch
    Scheduler::addTaskMainThread(SharedPtrThreadSafe<WTask>(new WTaskFunction(nullptr, methodUpdate, true)));

    return 0;
}

auto TabletController::getAudioPlayer() const -> AudioPlayer &
{
    return *_audioPlayer;
}

auto TabletController::getAudioRecorder() const -> AudioRecord &
{
    return *_audioRecorder;
}

auto TabletController::closeCurrentFile() -> int
{
    const auto result = this->_fileManager->updateFile(_nameDoc, *_doc, Extension::makeWriternote());

    if (result < 0)
        return result;

    _doc->reset();
    _nameDoc.clear();

    W_EMIT_1(onNeedRefresh, UpdateEvent::makeAll());
    W_EMIT_1(onNumberOfPageChanged, 0);
}
