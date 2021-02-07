#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QList>
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
#include "datawrite/qfilechoose.h"
/*funzione che gestisce l'indice*/
#include "indice_class.h"
#include "self_class.h"
#include "style/abilitazioneinput.h"
/* gestione della lista */
#include "currenttitle/redolist.h"

#include "dataread/xmlstruct.h"
#include "dialog_critic.h"
#include "update_list_copybook.h"
#include "savecopybook.h"
#include "newcopybook_.h"
#include "setting_ui.h"

#include "datawrite/savefile.h"
#include "style/abilitazioneinput.h"

/* da sistemare */
#include <QMediaPlayer>

#include "audiosetting/loadqualita.h"
#include "currenttitle/checksimilecopybook.h"

#if defined(WIN32) || defined(WIN64) || defined(TESTING)
#include "windows/updatecheck.h"
#endif

#include "audioplay/aggiornotastiriascolto.h"
#include "utils/setting_define.h"

#include <QSettings>

#include "utils/settings/setting_load.h"

MainWindow::MainWindow(QWidget *parent, TabletCanvas *canvas)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->m_canvas = canvas;

    ui->setupUi(this);

    m_audioRecorder = new QAudioRecorder(this);

    connect(m_audioRecorder, &QAudioRecorder::durationChanged, this, &MainWindow::updateProgress);
    connect(m_audioRecorder, QOverload<QMediaRecorder::Error>::of(&QAudioRecorder::error), this,
            &MainWindow::displayErrorMessage);


    player = new QMediaPlayer;
    connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::riascoltoaudioprogressivo);
    connect(player, &QMediaPlayer::stateChanged, this, &MainWindow::cambiostatoplayer);

    self = new SelfClass;

    setting_ui_start(this);

    this->ui->layouteditor->insertWidget(1, this->m_canvas);
    this->m_canvas->setHidden(true);

    abilitazioneinput(this);

    checkupdate = new updatecheck();

    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_GEOMETRY);

    QRect value_rect = setting.value(KEY_GEOMETRY).toRect();
    this->setGeometry(value_rect);

    setting.endGroup();

    setting_load(this);
}


MainWindow::~MainWindow()
{
    delete ui;
}

/* create new file */
void MainWindow::on_actionNew_File_triggered()
{
    if(this->self->indice.titolo.length() != 0){
        savecopybook checksave(this, &self->currentTitle);
        bool check = checksave.check_permission();

        if(!check)
            return;
    }

    this->self->currentTitle = "";
    //this->ui->actionCreate_new_copybook->setEnabled(true);
    this->self->currenttitle.reset();
}

void MainWindow::on_actionOpen_triggered(char *nomeFile)
{
    QString fileName;
    if(!nomeFile){
        fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "/home/giacomo", "Writernote (*.writer);; All file (* *.*)");
        if(fileName == "")
            return;
    }else{
        fileName = nomeFile;
    }


    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return dialog_critic("I can't open this file because of the permission");

    std::string pathtemp = fileName.toUtf8().constData();

    auto res = pathtemp.find("writer");
    if (std::string::npos == res)
        return dialog_critic("Miss the extension of the file");

    /*inizializza la classe per caricare i file*/
    this->self->path = pathtemp.c_str();
    xmlstruct *filefind = new xmlstruct(&this->self->path, &this->self->indice, &this->self->currenttitle);
    if(!filefind->loadindice()){
        delete filefind;
        return dialog_critic("We had a problem reading the file");
    }

    delete filefind;

    if(this->self->indice.titolo.length() > 0)
        this->ui->listWidgetSX->setEnabled(true);
    update_list_copybook(this);
}

/* Funzione che gestisce il doppio click sull'item a sinistra della lista copybook */
void MainWindow::on_listWidgetSX_itemDoubleClicked(QListWidgetItem *item)
{
    if(this->player->state() == QMediaPlayer::PlayingState)
        return redolist(this);

    if(this->m_audioRecorder->state() == QAudioRecorder::RecordingState
       || this->m_audioRecorder->state() == QAudioRecorder::PausedState)
        return redolist(this);

    if(this->self->currentTitle != ""){
        /* capisce se il currenttitle è cambiato, in caso contrario non chiede se si è sicuri di volerlo cambiare */
        currenttitle_class *tempcopybook = new currenttitle_class;
        xmlstruct *fileload = new xmlstruct(&this->self->path, &this->self->indice, tempcopybook);

        if(!fileload->loadfile((this->self->currentTitle + ".xml").toUtf8().constData())){
            delete fileload;
            return dialog_critic("We had a problem opening the current copybook");
        }

        if(checksimilecopybook(&this->self->currenttitle, tempcopybook) != OK){
            auto *savevariabile = new savecopybook(this, &this->self->currentTitle);

            if (!savevariabile->check_permission()){
                /* in caso l'utente abbia cancellato la richiesta o ci sia stato un problema interno */
                delete savevariabile;
                return redolist(this);
            }
        }
    }

    /* a questo punto deve aprire il nuovo copybook */
    if(this->self->indice.titolo[this->self->indice.titolo.indexOf(item->text())] != ""){
        auto *file_ = new xmlstruct(&this->self->path, &this->self->indice, &this->self->currenttitle);
        if(!file_->loadfile((item->text() + ".xml").toUtf8().constData())){
            delete file_;
            return dialog_critic("We had a problem opening the new copybook");
        }
    }
    else{
        this->self->currenttitle.reset();
        this->self->currenttitle.datatouch->reset();
    }
    this->self->currentTitle = item->text().toUtf8().constData();
    this->setWindowTitle("Writernote - " + this->self->currentTitle);

    if(this->self->currenttitle.posizione_binario == "")
        this->ui->textEdit->setHtml(this->self->currenttitle.testi);

    settingtextedit(this, true);
    settingstyle(this, true);

    aggiornotestiriascolto(this);
    abilitazioneinput(this);
    this->ui->actionPrint->setEnabled(true);

    /* pass the pointer to the class */
    if(this->self->currenttitle.posizione_binario != ""){
        this->m_canvas->settingdata(&self->currenttitle, self->path);
        this->m_canvas->loadpixel();

        this->m_canvas->time = 0;
    }

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
            this->player->setPosition(audio);

            this->player->setPosition(audio*1000);
            break;
        }
        else if ((i + 1) == this->self->currenttitle.posizione_iniz.length()){
            audio = this->self->currenttitle.posizione_iniz.last();

            this->player->setPosition(audio);

            //self.player.setPosition(audio*1000)
            break;
        }
        i ++;
    }
}


/* AUDIO TESTING */
void MainWindow::togglePause()
{
    if (m_audioRecorder->state() != QMediaRecorder::PausedState)
        m_audioRecorder->pause();
    else
        m_audioRecorder->record();
}

bool MainWindow::setOutputLocation()
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
    //QString fileName = QFileDialog::getSaveFileName();
    auto *qfile = new qfilechoose(this);
    QString fileName;
    if(!qfile->filechoose(&fileName, TYPEAUDIO)){
        delete qfile;
        return false;
    }
    delete qfile;
#endif

    this->self->currenttitle.audio_position_path = fileName;
    this->m_audioRecorder->setOutputLocation(QUrl::fromLocalFile(fileName));
    this->m_outputLocationSet = true;
    return true;
}

/* funzione che gestisce lo start della registrazione */
void MainWindow::on_startrecording_triggered()
{
    if(this->self->currenttitle.audio_position_path != "")
        return dialog_critic("You had already record an audio");

    if(!this->setOutputLocation())
        return;

    if(this->self->currenttitle.audio_position_path == "")
        return;

    if (this->m_audioRecorder->state() == QMediaRecorder::StoppedState) {        
        /*if(!loadqualita(this)){
            this->m_audioRecorder->setAudioInput((const QString)"");

            QAudioEncoderSettings settings;

            settings.setCodec((const QString) "");
            settings.setSampleRate(0);
            settings.setBitRate(0);
            settings.setChannelCount(-1);
            settings.setQuality(QMultimedia::EncodingQuality(2));
            settings.setEncodingMode(QMultimedia::ConstantQualityEncoding);

            QString container = "";

            this->m_audioRecorder->setEncodingSettings(settings, QVideoEncoderSettings(), container);
        }*/

        loadqualita(this);

        this->m_audioRecorder->record();

        this->self->currenttitle.testinohtml.clear();
        this->self->currenttitle.posizione_iniz.clear();
    }

    this->ui->startrecording->setEnabled(false);
    this->ui->stoprecordingbotton->setEnabled(true);
    this->ui->pauserecordingbotton->setEnabled(true);

}



/* funzionche che viene invocata quando la registrazione dell'audio viene messa in pausa */
void MainWindow::on_pauserecordingbotton_triggered()
{
    if (this->m_audioRecorder->state() != QMediaRecorder::PausedState)
        this->m_audioRecorder->pause();
    else
        this->m_audioRecorder->record();
}


/* editor di testo -> quando cambia il testo scritto */
void MainWindow::on_textEdit_textChanged()
{
    /* se non sta registrando deve uscire */
    if(this->m_audioRecorder->state() != QMediaRecorder::RecordingState)
        return;
    this->self->currenttitle.testi = this->ui->textEdit->toHtml();

    if(this->m_audioRecorder->status() != QMediaRecorder::RecordingStatus)
        return;

    QString text = ui->textEdit->toPlainText();

    this->self->currenttitle.testinohtml.append(ui->textEdit->toPlainText());
    this->self->currenttitle.posizione_iniz.append(this->self->currentTime);
}


void MainWindow::on_spinBox_fontsize_valueChanged(const QString &arg1)
{
    this->ui->textEdit->setCurrentFont(arg1);
}

void MainWindow::on_fontComboBox_fonttipo_currentFontChanged(const QFont &f)
{
    this->ui->textEdit->setCurrentFont(f);
}

void MainWindow::on_actionRedo_triggered()
{
    this->ui->textEdit->redo();
}

void MainWindow::on_actionCut_triggered()
{
    this->ui->textEdit->cut();
}

void MainWindow::on_actionSelect_all_triggered()
{
    this->ui->textEdit->selectAll();
}

void MainWindow::on_actionPaste_triggered()
{
    this->ui->textEdit->paste();
}

void MainWindow::on_actionCopy_triggered()
{
    this->ui->textEdit->copy();
}
