#include "savequalita.h"

#include "audioqualityoption.h"
#include "ui_audioqualityoption.h"

#include "definition.h"

static QVariant boxValue(const QComboBox *box)
{
    int idx = box->currentIndex();
    if (idx == -1)
        return QVariant();

    return box->itemData(idx);
}

bool savequalita(audioqualityoption *padredialog){
    oggettodascrivere_t oggettodascrivere;
    oggettodascrivere.audiodevice = boxValue(padredialog->ui->audioDeviceBox).toString();

    oggettodascrivere.settings.setCodec(boxValue(padredialog->ui->audioCodecBox).toString());
    oggettodascrivere.settings.setSampleRate(boxValue(padredialog->ui->sampleRateBox).toInt());
    oggettodascrivere.settings.setChannelCount(boxValue(padredialog->ui->channelsBox).toInt());
    oggettodascrivere.settings.setQuality(QMultimedia::EncodingQuality(padredialog->ui->qualitySlider->value()));
    oggettodascrivere.settings.setEncodingMode(QMultimedia::ConstantQualityEncoding);

    oggettodascrivere.container = boxValue(padredialog->ui->containerBox).toString();


    FILE *fp;
    fp = fopen(POSIZIONESETTINGS, "w");
    if(!fp)
        return false;


    fwrite(&oggettodascrivere, sizeof(oggettodascrivere), 1, fp);

    return true;


    /*const char *audioDeviceBox = boxValue(padredialog->ui->audioDeviceBox).toString().toUtf8().constData();
    const char *audioCodecBox = boxValue(padredialog->ui->audioCodecBox).toString().toUtf8().constData();
    const int sampleRateBox = boxValue(padredialog->ui->sampleRateBox).toInt();
    const int channelsBox = boxValue(padredialog->ui->channelsBox).toInt();
    auto qualityslider = QMultimedia::EncodingQuality(padredialog->ui->qualitySlider->value())
    auto */

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
    m_audioRecorder->setEncodingSettings(settings, QVideoEncoderSettings(), container);*/

}
