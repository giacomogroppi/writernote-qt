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

    fprintf(fp, "\n");
    fprintf(fp, "\n");
    fprintf(fp, "0\n");
    fprintf(fp, "0\n");
    fprintf(fp, "-1\n");
    fprintf(fp, "2\n");

    fprintf(fp, "\n");

    return true;
}

/* funzione che viene chiamata tutte le volte che l'utente inizia una registrazione */
bool loadqualita(MainWindow *parent){

    if(!checkexist())
        return false;

    FILE *fp;
    fp = fopen(POSIZIONESETTINGS, "r");
    if(!fp)
        return false;
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
    char stringa[100];
    int number, i = 0;

    i += fscanf(fp, "%[^\n]", stringa);
    parent->m_audioRecorder->setAudioInput(stringa);

    QAudioEncoderSettings settings;

    /* codec */
    i += fscanf(fp, "%[^\n]", stringa);
    settings.setCodec(stringa);


    /* rate */
    i += fscanf(fp, "%d", &number);
    settings.setSampleRate(number);

    /* bit rate */
    i += fscanf(fp, "%d", &number);
    settings.setBitRate(number);

    /* setChannelCount */
    i += fscanf(fp, "%d", &number);
    settings.setChannelCount(number);

    i += fscanf(fp, "%d", &number);
    settings.setQuality(QMultimedia::EncodingQuality(number));

    settings.setEncodingMode(QMultimedia::ConstantQualityEncoding);

    /* container */
    i += fscanf(fp, "%[^\n]", stringa);
    settings.setCodec(stringa);

    fclose(fp);

    if(i < 7)
        return false;

    parent->m_audioRecorder->setEncodingSettings(settings, QVideoEncoderSettings(), stringa);

    return true;

}
