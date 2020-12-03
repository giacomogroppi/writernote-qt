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
    if(!fp){
#ifdef STAMPA
    qDebug() << "File non trovato, o non aperto correttamente";
        QFile file(POSIZIONESETTINGS);
        if(file.open(QIODevice::ReadOnly))
            qDebug() << "File in sola lettura";
        else{
            QFileInfo file2(POSIZIONESETTINGS);
            if(!file2.exists()){
                qDebug() << "Il file non esiste neanche";
            }
        }

#endif

                return false;
    }

    fwrite(&oggettodascrivere, sizeof(oggettodascrivere), 1, fp);
    fclose(fp);

    return true;

}
