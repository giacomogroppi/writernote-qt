#define STAMPA 1/* gestisce la disattivazione delle stringhe di debug */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QList>
#include <unistd.h>
#include <QCloseEvent>
#include <QMessageBox>
#include <QListWidgetItem>
#include <QDebug>

/* filesystem controll */
#include <fstream>
#include <cstdint>
//#include <filesystem>
#include <QFile>
#include <iostream>
#include <string.h>
#include <QInputDialog>
#include <QFileDialog>
#include <QString>
#include "datawrite/qfilechoose.cpp"
/*funzione che gestisce l'indice*/
#include "indice_class.h"
#include "self_class.h"

/* gestione copybook */
#include "currenttitle/main_include_currenttitle.h"

/* gestione della lista */
#include "currenttitle/redolist.h"

#include "datawrite/xmlstruct.h"
#include "dialog_critic.h"
#include "update_list_copybook.h"
#include "savecopybook.h"
#include "newcopybook_.h"
#include "setting_ui.h"

#include "datawrite/savefile.h"

/* audio record */
#include "audiorecord/getbufferlevels.h"
//#include "audiorecord/displayErrorMessage.cpp"
#include "datamanage/sistemazionedatiregistrati.cpp"
#include <QMediaPlayer>

/* audio play */
#include "audioplay/main_audioplay.cpp"

#include "style/main_style.cpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* definizione di cosa serve per la registrazione dell'audio */
    m_audioRecorder = new QAudioRecorder(this);
    m_probe = new QAudioProbe(this);
    connect(m_probe, &QAudioProbe::audioBufferProbed,
            this, &MainWindow::processBuffer);
    m_probe->setSource(m_audioRecorder);

    connect(m_audioRecorder, &QAudioRecorder::durationChanged, this, &MainWindow::updateProgress);

    connect(m_audioRecorder, QOverload<QMediaRecorder::Error>::of(&QAudioRecorder::error), this,
            &MainWindow::displayErrorMessage);

    /* riproduzione dell'audio */
    player = new QMediaPlayer;
    connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::riascoltoaudioprogressivo);

    self = new SelfClass;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent (QCloseEvent *event)
{
    if(!this->self->indice.titolo.length())
        return event->accept();

    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "writernote",
                                                                tr("Do you want to save\n"),
                                                                QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::Yes);

    if (resBtn == QMessageBox::Yes) {

        if(this->self->path == "")
        {   qfilechoose file(this);
            if(!file.filechoose())
                return;
        }

        savefile save_(this, this->ui->listWidgetSX->currentItem());

        bool check = save_.savefile_check_indice();

        if(this->self->currentTitle != "")
            check = check && save_.savefile_check_file(this->self->indice.titolo.indexOf(this->self->currentTitle.c_str()));


        if(check)
            return event->accept();

        else{
            dialog_critic((QString)"We had a problem saving the file, please retry");
            return event->ignore();
        }

    } else if (resBtn == QMessageBox::No)
        return event->accept();

    else
        return event->ignore();
}

/*creazione di un nuovo file*/
void MainWindow::on_actionNew_File_triggered()
{
    if(this->self->indice.titolo.length() != 0){
        savecopybook checksave(this, ui->listWidgetSX->currentItem());
        bool check = checksave.check_permission();

        if(!check)
            return;
    }

    this->self->currentTitle = "";
    this->ui->actionCreate_new_copybook->setEnabled(true);
    this->self->currenttitle.reset();
}

/* funzione che viene richiamata quando viene salvato */
void MainWindow::on_actionSave_File_triggered()
{
    if(this->self->path == ""){
        qfilechoose file(this);
        if(!file.filechoose())
            /* se l'utente non ha selezionato nessun file */
            return;
    }

    this->self->currenttitle.testi = this->ui->textEdit->toHtml();

    savefile savefile_i(this, ui->listWidgetSX->currentItem());

    bool check = savefile_i.savefile_check_indice();

    if(this->self->currentTitle != "")
        check = check && savefile_i.savefile_check_file(this->self->indice.titolo.indexOf(this->self->currentTitle.c_str()));

    if(!check)
        return dialog_critic((QString) "We had a problem while saving the file");;

}

/*APERTURA DI UN FILE*/
void MainWindow::on_actionOpen_triggered()
{
    /* funzione che gestisce l'apertura di un file */

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "/home/giacomo", "Writernote (*.writer);; All file (* *.*)");
    if(fileName == "") return;

    QFile file(fileName);

#ifdef STAMPA
    qDebug() << "File name: " << fileName;
#endif

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return dialog_critic("Il file non esiste");

    /*fa la conversione per tra i qstring e i std::string*/
    std::string pathtemp = fileName.toUtf8().constData();

    auto res = pathtemp.find("writer");
    if (std::string::npos == res)
      return dialog_critic("Miss the extantion of the file");

    /*inizializza la classe per caricare i file*/
    xmlstruct filefind(pathtemp, this->self);
    filefind.loadindice();

    update_list_copybook(this);
}

/* Funzione che gestisce il doppio click sull'item a sinistra della lista copybook */
void MainWindow::on_listWidgetSX_itemDoubleClicked(QListWidgetItem *item)
{
    if(this->player->state() == QMediaPlayer::PlayingState)
        return redolist(this);

    if(this->self->currentTitle != ""){
        savecopybook savevariabile(this, item);

        if (!savevariabile.check_permission())
            /* in caso l'utente abbia cancellato la richiesta o ci sia stato un problema interno */
            return redolist(this);
    }

    /* a questo punto deve aprire il nuovo copybook */
    if(this->self->indice.titolo[this->self->indice.titolo.indexOf(item->text())] != ""){
        xmlstruct file_(this->self->path, this->self);
        file_.loadfile((item->text() + ".xml").toUtf8().constData());
    }
    else
        this->self->currenttitle.reset();

    this->self->currentTitle = item->text().toUtf8().constData();

    this->ui->textEdit->setHtml(this->self->currenttitle.testi);

    aggiornotestiriascolto(this);
}

/* funzione che gestisce il controllo del riascolto dell'audio */
void MainWindow::on_textEdit_selectionChanged(){
    if(this->player->state() != QMediaPlayer::PlayingState)
            return;

    QString text = ui->textEdit->textCursor().selectedText();
    int position = ui->textEdit->textCursor().selectionStart();

    int i = 1, audio;
    while (true){
        if ((position >= this->self->currenttitle.testinohtml[i-1].length()) && (position <= this->self->currenttitle.testinohtml[i+1].length())){
            audio = this->self->currenttitle.posizione_iniz[i];

#ifdef STAMPA
            qDebug() << "audio: " << audio*1000 ;
#endif

            this->player->setPosition(audio*1000);
            break;
        }
        else if ((i + 1) == this->self->currenttitle.posizione_iniz.length()){
            audio = this->self->currenttitle.posizione_iniz.last();

#ifdef STAMPA
            qDebug() << "audio2: " << audio*1000;
#endif
            //self.player.setPosition(audio*1000)
            break;
        }
        i ++;
    }
#ifdef STAMPA
    qDebug() << "text: " << text << "\nPosition: " << position;
#endif
}

/* funzione che gestisce la creazione di un nuovo copybook */
void MainWindow::on_actionCreate_new_copybook_triggered()
{
    if(this->self->currentTitle != ""){
        savecopybook check(this, ui->listWidgetSX->currentItem());

        if(!check.check_permission())
            /* l'utente ha detto che non vuole creare un nuovo file, oppure non si è riusciti a salvarlo */
            return;
    }

    bool ok;
    if(this->self->path == "")
    {
        /* deve salvare prima di continuare */
        qfilechoose filec(this);
        ok = filec.filechoose();
        if (!ok)
            return;
    }

    /* richiede all'utente se vuole salvare il file */
    QString namecopybook = QInputDialog::getText(this, tr("Get text"),
                                                 tr("Title: "), QLineEdit::Normal,
                                                 "", &ok);

    if(!ok || namecopybook == "")
        /* l'utente ha chiuso la finestra */
        return;

    //da fare
    if(namecopybook.indexOf("<titolo>") != -1 || namecopybook.indexOf("</titolo>") != -1
            || namecopybook.indexOf("<audio>") != -1
            || namecopybook.indexOf("</audio>") != -1
            || namecopybook.indexOf("<compressione>") != -1
            || namecopybook.indexOf("</compressione>") != -1
            || namecopybook.indexOf("<video>") != -1
            || namecopybook.indexOf("</video>") != -1)
        return dialog_critic("You can't use video, compressione, audio and titolo as name of the copybook");

    if(this->self->indice.titolo.indexOf(namecopybook) != -1){
        dialog_critic("There is a copybook that already has this title");
        return;
    }

    if(!newcopybook_(this, namecopybook))    
        return dialog_critic("We had a problem saving the copybook");


    update_list_copybook(this);


    this->ui->actionDelete_copybook->setEnabled(true);
    this->ui->textEdit->setEnabled(true);

}

/* AUDIO TESTING */
void MainWindow::togglePause()
{
    if (m_audioRecorder->state() != QMediaRecorder::PausedState)
        m_audioRecorder->pause();
    else
        m_audioRecorder->record();
}

void MainWindow::setOutputLocation()
{
#ifdef Q_OS_WINRT
    // UWP does not allow to store outside the sandbox
    const QString cacheDir = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    if (!QDir().mkpath(cacheDir)) {
        qWarning() << "Failed to create cache directory";
        return;
    }
    QString fileName = cacheDir + QLatin1String("/output.wav");
#else
    QString fileName = QFileDialog::getSaveFileName();
#endif

#ifdef STAMPA
    qDebug() << "\nMainWidnwo::setOutputLocation -> Si è scelta la posizione -> " << fileName;
#endif

    this->self->currenttitle.audio_position_path = fileName.toUtf8().constData();;
    this->m_audioRecorder->setOutputLocation(QUrl::fromLocalFile(fileName));
    this->m_outputLocationSet = true;
}

/* funzione che gestisce lo start della registrazione */
void MainWindow::on_startrecording_triggered()
{
#ifdef STAMPA
    qDebug() << "\nMainWindow::on_startrecording_triggered testinohtml: -> " << this->self->currenttitle.testinohtml.length();
    qDebug() << "\nMainWindow::on_startrecording_triggered posizione_iniz: -> " << this->self->currenttitle.posizione_iniz.length();
#endif


    this->setOutputLocation();
#ifdef STAMPA
    qDebug() << "Start recording";
#endif

    if (this->m_audioRecorder->state() == QMediaRecorder::StoppedState) {
        /* funzione che gestisce l'ingresso dell'audio */
        //m_audioRecorder->setAudioInput(boxValue(ui->audioDeviceBox).toString());
        this->m_audioRecorder->setAudioInput((const QString)"");

        QAudioEncoderSettings settings;

        /* parte della funzione che gestisce tutto la definizione delle variabile per la registrazione dell'audio */
        /*settings.setCodec(boxValue(ui->audioCodecBox).toString());
        settings.setSampleRate(boxValue(ui->sampleRateBox).toInt());
        settings.setBitRate(boxValue(ui->bitrateBox).toInt());
        settings.setChannelCount(boxValue(ui->channelsBox).toInt());
        settings.setQuality(QMultimedia::EncodingQuality(ui->qualitySlider->value()));
        settings.setEncodingMode(ui->constantQualityRadioButton->isChecked() ?
                                 QMultimedia::ConstantQualityEncoding :
                                 QMultimedia::ConstantBitRateEncoding);
        */

        settings.setCodec((const QString) "");
        settings.setSampleRate(0);
        settings.setBitRate(0);
        settings.setChannelCount(-1);
        settings.setQuality(QMultimedia::EncodingQuality(2));
        settings.setEncodingMode(QMultimedia::ConstantQualityEncoding);

        QString container = "";

        this->m_audioRecorder->setEncodingSettings(settings, QVideoEncoderSettings(), container);
        this->m_audioRecorder->record();

        this->self->currenttitle.testinohtml.clear();
        this->self->currenttitle.posizione_iniz.clear();
    }
}



/* funzionche che viene invocata quando la registrazione dell'audio viene messa in pausa */
void MainWindow::on_pauserecordingbotton_triggered()
{
    if (this->m_audioRecorder->state() != QMediaRecorder::PausedState)
        this->m_audioRecorder->pause();
    else
        this->m_audioRecorder->record();
}

void MainWindow::processBuffer(const QAudioBuffer& buffer)
{
    if (this->m_audioLevels.count() != buffer.format().channelCount()) {
        qDeleteAll(this->m_audioLevels);
        this->m_audioLevels.clear();
        for (int i = 0; i < buffer.format().channelCount(); ++i) {
            AudioLevel *level = new AudioLevel(this->ui->centralwidget);
            this->m_audioLevels.append(level);
        }
    }

    QList<qreal> levels = getBufferLevels(buffer);
    for (int i = 0; i < levels.count(); ++i)
        this->m_audioLevels.at(i)->setLevel(levels.at(i));
}

/* funzione che gestisce l'aggiornamento della status bar in base al valore del tempo dell'audio */
void MainWindow::updateProgress(qint64 duration)
{
    this->self->currentTime = (int)(duration/1000);
    qDebug() << endl << "AudioRecord:: updatePrograss ";
    if (m_audioRecorder->error() != QMediaRecorder::NoError || duration < 2000)
        return;

    this->ui->statusBar->showMessage(tr("Recorded %1 sec").arg(duration / 1000));
}


/* editor di testo -> quando cambia il testo scritto */
void MainWindow::on_textEdit_textChanged()
{
    this->self->currenttitle.testi = this->ui->textEdit->toHtml();

    if(this->m_audioRecorder->status() != QMediaRecorder::RecordingStatus)
        return;

    QString text = ui->textEdit->toPlainText();

    if(this->self->currenttitle.posizione_iniz.length() == 0){
        this->self->currenttitle.testinohtml.append(text);
        this->self->currenttitle.posizione_iniz.append(this->self->currentTime);
        return;
    }

/*#ifdef STAMPA
    qDebug() << "\nMainWindow::on_textEdit_textChanged testinohtml: -> " << this->self->currenttitle.testinohtml.length();
    qDebug() << "\nMainWindow::on_textEdit_textChangedposizione_iniz: -> " << this->self->currenttitle.posizione_iniz.length();
#endif*/

    /*int i;
    for(i=0; i< this->self->currenttitle.posizione_iniz.length(); i++)
        qDebug() << this->self->currenttitle.posizione_iniz[i];*/

    this->self->currenttitle.testinohtml.append(ui->textEdit->toPlainText());
    this->self->currenttitle.posizione_iniz.append(this->self->currentTime);

}


/*
Siccome per cambiare item è necessario fare doppio click
e facendo un solo click comunque cambia l'item della lista
in caso non sia un tasto destro va resettato quello di prima
*/
void MainWindow::on_listWidgetSX_itemClicked(QListWidgetItem *item)
{
    return redolist(this);
}
