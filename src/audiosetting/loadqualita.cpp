#include "loadqualita.h"
#include "../mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileInfo>
#include <QSettings>

#include "../utils/setting_define.h"

/* funzione che viene chiamata tutte le volte che l'utente inizia una registrazione */
void loadqualita(MainWindow *parent){

    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_AUDIO);

    parent->m_audioRecorder->setAudioInput(
                setting.value(KEY_AUDIO_DEVICE_BOX, "").toString());

    QAudioEncoderSettings setting_audio;
    setting_audio.setSampleRate(
                setting.value(KEY_SAMPLE_RATE_BOX, 0).toInt());

    //setting_audio.set



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

    /*
    parent->m_audioRecorder->setAudioInput((const QString)"");

    QAudioEncoderSettings settings;

    settings.setCodec((const QString) "");
    settings.setSampleRate(0);
    settings.setBitRate(0);
    settings.setChannelCount(-1);
    settings.setQuality(QMultimedia::EncodingQuality(2));
    settings.setEncodingMode(QMultimedia::ConstantQualityEncoding);

    QString container = "";

    this->m_audioRecorder->setEncodingSettings(settings, QVideoEncoderSettings(), container);*/

    QString stringa = setting.value(KEY_CONTAINER_BOX, "").toString();

    parent->m_audioRecorder->setEncodingSettings(setting_audio, QVideoEncoderSettings(), stringa);

}
