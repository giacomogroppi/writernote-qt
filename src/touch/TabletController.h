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
#include "file/FileManager.h"
#include "core/WOptionSetting/WOptionSettings.h"
#include "audiorecord/audiorecord.h"
#include "audioplay/audioplay.h"

class TabletController : public WObject
{
private:
    Fn<void(const WString&)> _showGenericError;

    struct {
        Highlighter *_highlighter;
        Pen *_pen;
        Rubber *_rubber;
        Laser *_laser;
        Square *_square;
    } _tools;

    WListFast<Tools*> _toolsContainer;

    WOptionSettings _settings;
    SharedPtr<FileManager> _fileManager;

    Tools *_currentTool;
    WColor _color;
    WPen _pen;
    Document *_doc;
    WString _nameDoc;

    UniquePtr<AudioRecord> _audioRecorder;
    UniquePtr<AudioPlayer> _audioPlayer;

    mutable bool _needUpdate;

    mutable std::atomic<bool> _isDrawing;

    void setAndCallTool(Tools *tool);

    auto getDoc() -> Document &;
    const Document &getDoc() const;

    void checkCreatePage();
    void draw(WPainter &painter, double width, WFlags<UpdateEvent::UpdateEventType> flags) const;
public:
    explicit TabletController(
            WObject *parent,
            const WPath&        defaultPathSaving,
            Fn<void(const WString&)> showGenericError
    );

    ~TabletController() override;

    void getImg(WPainter &painter, double width, WFlags<UpdateEvent::UpdateEventType> flags) const;
    void getImagePage(WPainter &painter, double width) const;
    void getImageStroke(WPainter &painter, double width) const;
    
    auto getCurrentTool() const -> Tools*;
    auto getCurrentPathSaving() const -> WPath;

    auto getFileManager() -> const SharedPtr<FileManager>&;

    /**
     * \return -1 in caso di errore in scrittura
     * */
    auto closeCurrentFile() -> int;

    /**
     * In caso ci sia già un file aperto verrà automaticamente chiuso e
     * salvato.
     * Il medoto lancia automaticamente i segnali per segnalare che è necessario ridisegnare.
     * \return &lt 0 in caso di errore nell'apertura del file
     * */
     // TODO: make this method return WPair<int, WString> with a proper error message returned
    auto openFile(const WString &name) -> int;

    /**
     * \param path A valid path
     * */
    void setCurrentPathSaving(WString path);

    [[nodiscard]]
    auto getAudioPlayer() const -> AudioPlayer&;

    [[nodiscard]]
    auto getAudioRecorder() const -> AudioRecord&;

    DEFINE_LISTENER_1(selectType, int, type);
    DEFINE_LISTENER_1(selectColor, const WColor&, color);

    DEFINE_LISTENER_1(positionDocChanged, const PointF &, newPosition);

    DEFINE_LISTENER_2(touchBegin, const PointF &, point, double, pressure);
    DEFINE_LISTENER_2(touchUpdate, const PointF &, point, double, pressure);
    DEFINE_LISTENER_2(touchEnd, const PointF &, point, double, pressure);

    W_EMITTABLE_1(onNeedRefresh, const UpdateEvent&, event);

    W_EMITTABLE_1(onPlayAudioStateChanged, bool, isPlaying);

    W_EMITTABLE_0(onToolChanged);
    W_EMITTABLE_0(onPropertyHide);
    W_EMITTABLE_1(onNumberOfPageChanged, int, numerberOfPage);
    W_EMITTABLE_2(onPropertyShow, const PointF&, point, ActionProperty, actionProperty);
    W_EMITTABLE_0(onPathChanged);
};

inline const Document &TabletController::getDoc() const
{
    return *this->_doc;
}

inline Tools *TabletController::getCurrentTool() const
{
    return this->_currentTool;
}

