#pragma once

#include <QObject>
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
# include <QAudioRecorder>
#else
# include <QMediaCaptureSession>
# include <QMediaFormat>
# include <QMediaRecorder>
# include <QAudioInput>
# include <QAudioDevice>
#endif
#include <QUrl>
#include <QFile>
#include "utils/WCommonScript.h"

class AudioRecord : public QObject
{
    Q_OBJECT
public:
    explicit AudioRecord(QObject *parent);

    bool isRecording() const;
    bool isPauseRecording() const;
    bool isStopped() const;

    void startRecord();
    void pauseRecord();
    void stopRecording();

    QMediaRecorder::Error errors() const;

    void setOutputLocation(const QString &path);

    void loadSettings();
    qint64 getCurrentTime();

    const QString getPath() const;

signals:
private slots:
    void updateProgress(qint64 duration);
    void displayErrorMessage();
private:

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QAudioRecorder *recorder;
#else
    QMediaCaptureSession m_captureSession;
    QMediaRecorder *recorder;
#endif

    friend class audioqualityoption;
};

inline bool AudioRecord::isRecording() const
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    return this->recorder->state() == QMediaRecorder::RecordingState;
#else
    return this->recorder->recorderState() == QMediaRecorder::RecordingState;
#endif
}

inline bool AudioRecord::isPauseRecording() const
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    return this->recorder->state() == QMediaRecorder::PausedState;
#else
    return this->recorder->recorderState() == QMediaRecorder::PausedState;
#endif
}

inline bool AudioRecord::isStopped() const
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    return recorder->state() == QMediaRecorder::StoppedState;
#else
    return recorder->recorderState() == QMediaRecorder::StoppedState;
#endif
}

inline void AudioRecord::startRecord()
{
    const auto path = this->getPath();

#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)

    this->m_captureSession.audioInput()->setDevice(QVariant("").value<QAudioDevice>());

    QMediaFormat format;
    {
        format.setFileFormat(QMediaFormat::FileFormat::MPEG4);
        format.setAudioCodec(QMediaFormat::AudioCodec::MP3);
    }
    recorder->setMediaFormat(format);
    recorder->setAudioSampleRate(44100);
    recorder->setAudioBitRate(64000);
    recorder->setAudioChannelCount(1);

    recorder->setEncodingMode(QMediaRecorder::ConstantBitRateEncoding);
#endif

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

inline QMediaRecorder::Error AudioRecord::errors() const
{
    return this->recorder->error();
}

inline void AudioRecord::setOutputLocation(const QString &path)
{
    QFile::remove(path);
    this->recorder->setOutputLocation(QUrl::fromLocalFile(path));
}

/* return time in second */
inline qint64 AudioRecord::getCurrentTime()
{
    if(!isRecording())
        return 0;
    return this->recorder->duration() / 1000;
}

inline const QString AudioRecord::getPath() const
{
    return this->recorder->outputLocation().toLocalFile();
}
