#pragma once

#include "utils/WCommonScript.h"
#include "core/String/WString.h"
#include "core/WElement.h"
#include "core/Path/WPath.h"
#include "core/WFile.h"
#include "AudioData/AudioData.h"

#ifdef USE_QT

#include <QMediaCaptureSession>
#include <QMediaRecorder>
#include <QUrl>
#include <QMediaFormat>
#include <QStandardPaths>

class AudioRecord : public WObject
{
private:
    Fn<void(const WString &)> _error;
public:
    explicit AudioRecord(WObject *parent,
                         Fn<void(const WString &)> error);
    ~AudioRecord() override;

    [[nodiscard]]
    auto isRecording() const -> Bool;

    [[nodiscard]]
    auto isPauseRecording() const -> Bool;

    [[nodiscard]]
    auto isStopped() const -> Bool;

    void startRecord();
    void pauseRecord();
    void stopRecording();

    /**
     * \return Time in second
     * */
    auto getCurrentTime() -> UnsignedLong;

    void displayErrorMessage();

    W_EMITTABLE_1(onDurationChanged, Unsigned, currentLenght);
    W_EMITTABLE_1(onDataChanged, AudioData, audioData);

private:
    QMediaCaptureSession m_captureSession;
    QMediaRecorder *recorder;

    static constexpr auto name = "writernote_audio.mp3";
    WPath _currentPosition;
    void setOutputLocation(const WPath &path);

    friend class audioqualityoption;
};

inline auto AudioRecord::isRecording() const -> Bool
{
    return Bool(this->recorder->recorderState() == QMediaRecorder::RecordingState);
}

inline auto AudioRecord::isPauseRecording() const -> Bool
{
    return Bool(this->recorder->recorderState() == QMediaRecorder::PausedState);
}

inline auto AudioRecord::isStopped() const -> Bool
{
    return Bool(recorder->recorderState() == QMediaRecorder::StoppedState);
}

inline void AudioRecord::startRecord()
{
    const auto tmpLocation = QStandardPaths::writableLocation(QStandardPaths::StandardLocation::TempLocation);
    const auto pathSaving = WPath (tmpLocation);

    WFile::remove(pathSaving / name);

    this->_currentPosition = pathSaving / name;
    this->setOutputLocation(_currentPosition);

    W_ASSERT(WPath(this->recorder->outputLocation().toLocalFile()) == pathSaving);

    this->recorder->record();

#ifdef DEBUGINFO
    if(!isRecording()){
        qDebug() << this->recorder->errorString();
        std::abort();
    }
#endif
}

inline void AudioRecord::pauseRecord()
{
    this->recorder->pause();
}

inline void AudioRecord::stopRecording()
{
    recorder->stop();
}

inline void AudioRecord::setOutputLocation(const WPath &path)
{
    W_ASSERT(this->isStopped());

    WFile::remove(path);
    _currentPosition = path;
    this->recorder->setOutputLocation(path.toString());
    this->recorder->setMediaFormat(QMediaFormat::MP3);
}

/* return time in second */
inline auto AudioRecord::getCurrentTime() -> UnsignedLong
{
    if(!isRecording())
        return UnsignedLong (0ul);
    return UnsignedLong (static_cast<unsigned long>(this->recorder->duration()) / 1000ul);
}

#endif // USE_QT
