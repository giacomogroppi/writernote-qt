#ifndef AUDIORECORD_H
#define AUDIORECORD_H

#include <QObject>
#include <QAudioRecorder>

class AudioRecord : public QObject
{
    Q_OBJECT
public:
    explicit AudioRecord(QObject *parent);

signals:
private slots:
private:
    QAudioRecorder *recorder;
    class MainWindow *parent;

};

#endif // AUDIORECORD_H
