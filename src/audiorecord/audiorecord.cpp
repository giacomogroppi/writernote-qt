#include "audiorecord.h"
#include "../mainwindow.h"

AudioRecord::AudioRecord(QObject *parent) : QObject(parent)
{
    this->recorder = new QAudioRecorder(this);
    this->parent = (MainWindow *)parent;
    Q_ASSERT(this->parent->objectName() == "MainWindow");
}
