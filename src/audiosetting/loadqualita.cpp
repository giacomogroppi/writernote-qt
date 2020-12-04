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

}
