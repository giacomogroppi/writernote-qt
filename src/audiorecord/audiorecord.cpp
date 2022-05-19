#include "audiorecord.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileInfo>
#include <QSettings>
#include "core/core.h"

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QAudioEncoderSettings>
#else
#include <QMediaRecorder>
#endif

#include "utils/setting_define.h"
#include "testing/memtest.h"

AudioRecord::AudioRecord(QObject *parent) : QObject(parent)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    this->recorder = new QAudioRecorder(this);
#else
    this->recorder = new QMediaRecorder(this);
    m_captureSession.setRecorder(recorder);
    m_captureSession.setAudioInput(new QAudioInput(this));
#endif

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    connect(recorder, &QAudioRecorder::durationChanged, this, &AudioRecord::updateProgress);
    connect(recorder, QOverload<QMediaRecorder::Error>::of(&QAudioRecorder::error), this,
            &AudioRecord::displayErrorMessage);
#else
    QObject::connect(recorder, &QMediaRecorder::durationChanged, this, &AudioRecord::updateProgress);
#endif
}

void AudioRecord::loadSettings()
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
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
#endif
}

void AudioRecord::updateProgress(qint64 duration)
{
    if (this->errors() != QMediaRecorder::NoError)
        return;

    core::get_main_window()->ui->statusBar->showMessage(tr("Recorded %1 sec").arg(duration / 1000));
}

void AudioRecord::displayErrorMessage()
{
    core::get_main_window()->ui->statusBar->showMessage(recorder->errorString());
}
