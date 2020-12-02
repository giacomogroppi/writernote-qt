#include "loadqualita.h"
#include "../mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

#include <QFileInfo>

#include "definition.h"

static bool checkexist(){
    QFileInfo check_file(POSIZIONESETTINGS);

    if(check_file.exists()){
        return true;
    }


    /* scrive le preferenze base */
    FILE *fp;
    fp = fopen(POSIZIONESETTINGS, "w");

    oggettodascrivere_t oggettodascrivere;

    oggettodascrivere.audiodevice = "";

    oggettodascrivere.settings.setCodec((const QString) "");
    oggettodascrivere.settings.setSampleRate(0);
    oggettodascrivere.settings.setBitRate(0);
    oggettodascrivere.settings.setChannelCount(-1);
    oggettodascrivere.settings.setQuality(QMultimedia::EncodingQuality(2));
    oggettodascrivere.settings.setEncodingMode(QMultimedia::ConstantQualityEncoding);

    oggettodascrivere.container = "";

    fwrite(&oggettodascrivere, sizeof(oggettodascrivere), 1, fp);
    return true;
}

/* funzione che viene chiamata tutte le volte che l'utente inizia una registrazione */
bool loadqualita(MainWindow *parent){
    if(!checkexist())
        return false;

    oggettodascrivere_t oggettodaleggere;

    FILE *fp;
    fp = fopen(POSIZIONESETTINGS, "r");
    if(!fp)
        return false;


    int quanti = fread(&oggettodaleggere, sizeof(oggettodaleggere), 1, fp);
    fclose(fp);
    if(quanti < 1)
        return false;


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
