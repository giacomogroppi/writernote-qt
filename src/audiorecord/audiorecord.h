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

    QMediaRecorder::Error errors() const;

    void setOutputLocation(const QString &path);

signals:
private slots:
    void updateProgress(qint64 duration);
    void displayErrorMessage();
private:
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

inline QMediaRecorder::Error AudioRecord::errors() const
{
    return this->recorder->error();
}

inline void AudioRecord::setOutputLocation(const QString &path)
{
    this->recorder->setOutputLocation(QUrl::fromLocalFile(path));
}

#endif // AUDIORECORD_H
