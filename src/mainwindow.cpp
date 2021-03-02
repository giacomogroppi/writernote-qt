#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QList>
#include <QCloseEvent>
#include <QMessageBox>
#include <QListWidgetItem>

#include <fstream>
#include <cstdint>
#include <QFile>
#include <iostream>
#include <string.h>
#include <QInputDialog>
#include <QFileDialog>
#include "datawrite/qfilechoose.h"

#include "style/abilitazioneinput.h"

#include "currenttitle/redolist.h"

#include "dataread/xmlstruct.h"
#include "utils/dialog_critic/dialog_critic.h"
#include "update_list_copybook.h"
#include "savecopybook.h"
#include "newcopybook_.h"
#include "setting_ui.h"

#include "datawrite/savefile.h"
#include "style/abilitazioneinput.h"

#include "currenttitle/checksimilecopybook.h"

#include "windows/updatecheck.h"

#include "audioplay/aggiornotastiriascolto.h"
#include "utils/setting_define.h"

#include <QSettings>

#include "utils/settings/setting_load.h"
#include "windows/mostra_finestra_i.h"

MainWindow::MainWindow(QWidget *parent, TabletCanvas *canvas, struct struct_user *user, cloud_controll *cloud)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->m_cloud = cloud;
    this->m_canvas = canvas;
    this->m_user = user;

    ui->setupUi(this);

    m_audioRecorder = new QAudioRecorder(this);

    connect(m_audioRecorder, &QAudioRecorder::durationChanged, this, &MainWindow::updateProgress);
    connect(m_audioRecorder, QOverload<QMediaRecorder::Error>::of(&QAudioRecorder::error), this,
            &MainWindow::displayErrorMessage);

    player = new QMediaPlayer;
    connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::riascoltoaudioprogressivo);
    connect(player, &QMediaPlayer::stateChanged, this, &MainWindow::cambiostatoplayer);

    setting_ui_start(this);

    this->ui->layouteditor->insertWidget(1, this->m_canvas);
    this->m_canvas->setHidden(true);

    abilitazioneinput(this);

    checkupdate = new updatecheck();

    setting_load(this);

    this->m_rubber = new rubber_ui(this);
    this->m_pen = new pen_ui(this);
    this->m_text = new text_ui(this);

    this->m_text_w = new text_widgets(this);

    this->m_text_w->setHidden(true);
    this->m_text->setHidden(true);
    this->m_pen->setHidden(true);
    this->m_rubber->setHidden(true);

    this->m_canvas->m_rubber = m_rubber;
    this->m_canvas->m_pen_ui = m_pen;
    this->m_canvas->m_text = m_text;

    this->m_canvas->m_text_w = m_text_w;

    ui->button_left_hide->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    ui->button_right_hide->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

#ifndef CLOUD
    ui->actioncloud->setVisible(false);
#endif
}


MainWindow::~MainWindow()
{
    delete ui;
    delete m_rubber;
    delete m_pen;
}

/* create new file */
void MainWindow::on_actionNew_File_triggered()
{
    if(this->m_indice.titolo.length() != 0){
        savecopybook checksave(this, &m_currentTitle);
        bool check = checksave.check_permission();

        if(!check)
            return;
    }

    this->m_currentTitle = "";
    //this->ui->actionCreate_new_copybook->setEnabled(true);
    this->m_currenttitle.reset();
}

void MainWindow::on_actionOpen_triggered(char *nomeFile)
{
    QString fileName;
    if(!nomeFile){
        fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "/home/giacomo", "Writernote (*.writer);; All file (* *.*)");
    }else{
        fileName = nomeFile;
    }

    if(fileName == "")
        return;

    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return dialog_critic("I can't open this file because of the permission");

    //std::string pathtemp = fileName.toUtf8().constData();

    if(fileName.indexOf(".writer") == -1)
        return dialog_critic("Are you sure it's a writernote file?");

    m_path = fileName;

    xmlstruct filefind(&m_path, &m_indice, &m_currenttitle);
    if(!filefind.loadindice())
        return dialog_critic("We had a problem reading the file");


    if(this->m_indice.titolo.length() > 0)
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

    if(m_currentTitle != ""){
        /* capisce se il currenttitle è cambiato, in caso contrario non chiede se si è sicuri di volerlo cambiare */
        currenttitle_class tempcopybook;
        xmlstruct fileload(&m_path, &m_indice, &tempcopybook);

        int temp = fileload.loadfile((m_currentTitle + ".xml").toUtf8().constData());
        if(temp == ERROR_VERSION){
            return dialog_critic("the version you created this file with is too old to read");
        }
        if(temp == ERROR){
            return dialog_critic("We had a problem opening the current copybook");
        }

        if(checksimilecopybook(&m_currenttitle, &tempcopybook) != OK){
            savecopybook savevariabile(this, &m_currentTitle);

            if (!savevariabile.check_permission())
                /* in caso l'utente abbia cancellato la richiesta o ci sia stato un problema interno */
                return redolist(this);

        }
    }

    /* a questo punto deve aprire il nuovo copybook */
    if(m_indice.titolo[m_indice.titolo.indexOf(item->text())] != ""){
        auto *file_ = new xmlstruct(&m_path, &m_indice, &m_currenttitle);
        int res = file_->loadfile((item->text() + ".xml").toUtf8().constData());

        if(res == ERROR){
            delete file_;
            return dialog_critic("We had a problem opening the new copybook");
        }else if(res == ERROR_VERSION){
            delete file_;
            return dialog_critic("The version you created this file with is too old");
        }
    }
    else{
        m_currenttitle.reset();
        m_currenttitle.datatouch->reset();
    }
    m_currentTitle = item->text().toUtf8().constData();
    this->setWindowTitle("Writernote - " + m_currentTitle);

    if(!m_currenttitle.m_touch)
        this->ui->textEdit->setHtml(m_currenttitle.testi);

    settingtextedit(this, true);
    settingstyle(this, true);

    aggiornotestiriascolto(this);
    abilitazioneinput(this);
    this->ui->actionPrint->setEnabled(true);

    /* pass the pointer to the class */
    if(m_currenttitle.m_touch){
        this->m_canvas->settingdata(&m_currenttitle, m_path);
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
        if ((position >= m_currenttitle.testinohtml[i-1].length()) && (position <= m_currenttitle.testinohtml[i+1].length())){
            audio = m_currenttitle.posizione_iniz[i];
            this->player->setPosition(audio);

            this->player->setPosition(audio*1000);
            break;
        }
        else if ((i + 1) == m_currenttitle.posizione_iniz.length()){
            audio = m_currenttitle.posizione_iniz.last();

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

    m_currenttitle.audio_position_path = fileName;
    this->m_audioRecorder->setOutputLocation(QUrl::fromLocalFile(fileName));
    this->m_outputLocationSet = true;
    return true;
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
    this->m_currenttitle.testi = this->ui->textEdit->toHtml();

    if(this->m_audioRecorder->status() != QMediaRecorder::RecordingStatus)
        return;

    QString text = ui->textEdit->toPlainText();

    m_currenttitle.testinohtml.append(ui->textEdit->toPlainText());
    m_currenttitle.posizione_iniz.append(m_currentTime);
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

#define VERSION_MAJOR 4.432.234

#define STRINGIZE2(s) #s
#define STRINGIZE(s) STRINGIZE2(s)
#define VERSION_STRING STRINGIZE(VERSION_SNAPCRAFT)

void MainWindow::on_actionVersion_triggered()
{
#ifdef VERSION_SNAPCRAFT
    messaggio_utente("Current version is " + (QString)VERSION_STRING);
#endif // version
}

