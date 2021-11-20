#ifndef AUDIORECORD_H
#define AUDIORECORD_H

#include <QObject>
#include <QAudioRecorder>
#include <QUrl>

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

    const QString & getPath() const;

signals:
private slots:
    void updateProgress(qint64 duration);
    void displayErrorMessage();
private:
    QString pathAudio;
    QAudioRecorder *recorder;
    class MainWindow *parent;

    friend class audioqualityoption;
};

inline bool AudioRecord::isRecording() const
{
    return this->recorder->state() == QMediaRecorder::RecordingState;
}

inline bool AudioRecord::isPauseRecording() const
{
    return this->recorder->state() == QMediaRecorder::PausedState;
}

inline bool AudioRecord::isStopped() const
{
    return recorder->state() == QMediaRecorder::StoppedState;
}

inline void AudioRecord::startRecord()
{
    this->recorder->record();
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
    pathAudio = path;
    this->recorder->setOutputLocation(QUrl::fromLocalFile(path));
}

/* return time in second */
inline qint64 AudioRecord::getCurrentTime()
{
    if(!isRecording())
        return 0;
    return this->recorder->duration() / 1000;
}

inline const QString &AudioRecord::getPath() const
{
    return pathAudio;
}

#endif // AUDIORECORD_H
