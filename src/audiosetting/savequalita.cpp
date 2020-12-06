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

    fprintf(fp, "%s\n", boxValue(padredialog->ui->audioDeviceBox).toString().toUtf8().constData());
    fprintf(fp, "%d\n", boxValue(padredialog->ui->sampleRateBox).toInt());
    fprintf(fp, "%s\n", boxValue(padredialog->ui->audioCodecBox).toString().toUtf8().constData());
    fprintf(fp, "%d\n", boxValue(padredialog->ui->sampleRateBox).toInt());
    fprintf(fp, "%d\n", boxValue(padredialog->ui->channelsBox).toInt());
    fprintf(fp, "%d\n", padredialog->ui->qualitySlider->value());

    /* container */
    fprintf(fp, "%s\n", boxValue(padredialog->ui->containerBox).toString().toUtf8().constData());


    fclose(fp);

    return true;

}
