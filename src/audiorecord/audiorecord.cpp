#include "audiorecord.h"
#include "../mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileInfo>
#include <QSettings>
#include <QAudioEncoderSettings>
#include "../utils/setting_define.h"

AudioRecord::AudioRecord(QObject *parent) : QObject(parent)
{
    this->recorder = new QAudioRecorder(this);
    this->parent = (MainWindow *)parent;
    Q_ASSERT(this->parent->objectName() == "MainWindow");

    connect(recorder, &QAudioRecorder::durationChanged, this, &AudioRecord::updateProgress);
    connect(recorder, QOverload<QMediaRecorder::Error>::of(&QAudioRecorder::error), this,
            &AudioRecord::displayErrorMessage);

}

void AudioRecord::loadSettings()
{
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    QAudioEncoderSettings setting_audio;
    QString str;
    setting.beginGroup(GROUPNAME_AUDIO);

    recorder->setAudioInput(
                setting.value(KEY_AUDIO_DEVICE_BOX, "").toString());


    setting_audio.setSampleRate(
                setting.value(KEY_SAMPLE_RATE_BOX, 0).toInt());


    setting_audio.setCodec(
                setting.value(KEY_AUDIO_CODEC_BOX, "").toString());

    setting_audio.setQuality(
                QMultimedia::EncodingQuality(
                                 setting.value(
                                    KEY_QUALITY_SLIDER,
                                    QMultimedia::EncodingQuality(2)).toInt()
                    )
                );


    setting_audio.setEncodingMode(QMultimedia::ConstantQualityEncoding);

    setting_audio.setChannelCount(setting.value(KEY_CHANNELS_BOX, -1).toInt());

    str = setting.value(KEY_CONTAINER_BOX, "").toString();

    setting.endGroup();

    recorder->setEncodingSettings(setting_audio, QVideoEncoderSettings(), str);

}

void AudioRecord::updateProgress(qint64 duration)
{
    if (this->errors() != QMediaRecorder::NoError)
        return;
    parent->ui->statusBar->showMessage(tr("Recorded %1 sec").arg(duration / 1000));
}

void AudioRecord::displayErrorMessage()
{
    parent->ui->statusBar->showMessage(recorder->errorString());
}
