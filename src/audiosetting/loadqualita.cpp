#include "loadqualita.h"
#include "../mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

#include "definition.h"

/* funzione che viene chiamata tutte le volte che l'utente inizia una registrazione */
bool loadqualita(MainWindow *parent){
    oggettodascrivere_t oggettodaleggere;

    FILE *fp;
    fp = fopen(POSIZIONESETTINGS, "r");
    if(!fp)
        return false;


    fread(&oggettodaleggere, sizeof(oggettodaleggere), 1, fp);

    parent->m_audioRecorder->setAudioInput(oggettodaleggere.audiodevice);

    parent->m_audioRecorder->setEncodingSettings(oggettodaleggere.settings, QVideoEncoderSettings(), oggettodaleggere.container);

    return true;


    /*m_audioRecorder->setAudioInput(boxValue(padredialogo->ui->audioDeviceBox).toString());

    QAudioEncoderSettings settings;
    settings.setCodec(boxValue(ui->audioCodecBox).toString());
    settings.setSampleRate(boxValue(ui->sampleRateBox).toInt());
    settings.setBitRate(boxValue(ui->bitrateBox).toInt());
    settings.setChannelCount(boxValue(ui->channelsBox).toInt());
    settings.setQuality(QMultimedia::EncodingQuality(ui->qualitySlider->value()));
    settings.setEncodingMode(ui->constantQualityRadioButton->isChecked() ?
                             QMultimedia::ConstantQualityEncoding :
                             QMultimedia::ConstantBitRateEncoding);

    QString container = boxValue(ui->containerBox).toString();
    qDebug() << "\nQString container ->" << container << "FINE";
    parent->m_audioRecorder->setEncodingSettings(settings, QVideoEncoderSettings(), container);
*/

}
