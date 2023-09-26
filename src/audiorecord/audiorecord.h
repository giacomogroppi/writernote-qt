#pragma once

#include "Scheduler/WObject.h"
#ifdef USE_QT
#include <QMediaCaptureSession>
#include <QMediaFormat>
#include <QMediaRecorder>
#include <QAudioInput>
#include <QAudioDevice>
#include <QUrl>
#include <QFile>
#endif // USE_QT
#include "utils/WCommonScript.h"
#include "core/String/WString.h"
#include "core/Path/WPath.h"
#include "core/WElement.h"

class AudioRecord : public WObject
{
private:
    Fn<void(const WString &)> _error;
    Fn<void(int)> _durationChange;
public:
    /**
     * duration is express in seconds
    */
    explicit AudioRecord(WObject *parent,
                         Fn<void(const WString &)> error,
                         Fn<void(int)> durationChange);
    ~AudioRecord() override;

    [[nodiscard]]
    auto isRecording() const -> bool;

    [[nodiscard]]
    auto isPauseRecording() const -> bool;

    [[nodiscard]]
    auto isStopped() const -> bool;

    void startRecord();
    void pauseRecord();
    void stopRecording();

    void setOutputLocation(const WPath &path);

    auto getCurrentTime() -> UnsignedLong;

    [[nodiscard]] auto getPath() const -> WPath;


    DEFINE_LISTENER(displayErrorMessage());
    DEFINE_LISTENER(updateProgress(unsigned long duration));
private:

#ifdef USE_QT
    QMediaCaptureSession m_captureSession;
    QMediaRecorder *recorder;
#endif

    friend class audioqualityoption;
};

#if defined(USE_QT)
inline bool AudioRecord::isRecording() const
{
    return this->recorder->recorderState() == QMediaRecorder::RecordingState;
}

inline bool AudioRecord::isPauseRecording() const
{
    return this->recorder->recorderState() == QMediaRecorder::PausedState;
}

inline bool AudioRecord::isStopped() const
{
    return recorder->recorderState() == QMediaRecorder::StoppedState;
}

inline void AudioRecord::startRecord()
{
    const auto path = this->getPath();

    QFile::remove(path);
    W_ASSERT(this->recorder->outputLocation().toLocalFile() == path);
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

inline void AudioRecord::setOutputLocation(const WString &path)
{
    QFile::remove(path);
    this->recorder->setOutputLocation(QUrl::fromLocalFile(path));
}

/* return time in second */
inline unsigned long AudioRecord::getCurrentTime()
{
    if(!isRecording())
        return 0;
    return this->recorder->duration() / 1000;
}

inline const WString AudioRecord::getPath() const
{
    return this->recorder->outputLocation().toLocalFile();
}
#endif // USE_QT
