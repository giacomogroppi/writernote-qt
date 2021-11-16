#include "audiorecord.h"
#include "../mainwindow.h"
#include "ui_mainwindow.h"

AudioRecord::AudioRecord(QObject *parent) : QObject(parent)
{
    this->recorder = new QAudioRecorder(this);
    this->parent = (MainWindow *)parent;
    Q_ASSERT(this->parent->objectName() == "MainWindow");

    connect(recorder, &QAudioRecorder::durationChanged, this, &AudioRecord::updateProgress);
    connect(recorder, QOverload<QMediaRecorder::Error>::of(&QAudioRecorder::error), this,
            &AudioRecord::displayErrorMessage);

}

void AudioRecord::updateProgress(qint64 duration)
{

}

void AudioRecord::displayErrorMessage()
{
    parent->ui->statusBar->showMessage(recorder->errorString());
}
