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
#include "utils/areyousure/areyousure.h"
#include <QTimer>
#include "restore_file/ui/setting_restore_ui.h"
#include "touch/scrollKinetic/ui_scroll/ui_scroll.h"
#include "utils/common_def.h"

MainWindow::MainWindow(QWidget *parent,
                       TabletCanvas *canvas,
                       struct struct_user *user,
                       cloud_controll *cloud)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->m_cloud = cloud;

    this->m_canvas = canvas;
    this->m_canvas->parent = this;

    this->m_user = user;

    ui->setupUi(this);

    this->m_currenttitle = new currenttitle_class;

    m_audioRecorder = new QAudioRecorder(this);

    connect(m_audioRecorder, &QAudioRecorder::durationChanged, this, &MainWindow::updateProgress);
    connect(m_audioRecorder, QOverload<QMediaRecorder::Error>::of(&QAudioRecorder::error), this,
            &MainWindow::displayErrorMessage);

    player = new QMediaPlayer(this);
    connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::riascoltoaudioprogressivo);
    connect(player, &QMediaPlayer::stateChanged, this, &MainWindow::cambiostatoplayer);
    connect(player, &QMediaPlayer::durationChanged, [=](qint64 duration){
        qDebug() << "Duration change " << duration;
    });

    connect(player, QOverload<QMediaPlayer::Error>::of(&QMediaPlayer::error),
        [=](QMediaPlayer::Error error){
        Q_UNUSED(error)
        messaggio_utente("We had an internal error with this error code: " + player->errorString());
    });

    this->m_buffer = new QBuffer(this);

    setting_ui_start(this);

    this->ui->layouteditor->insertWidget(1, this->m_canvas);
    this->m_canvas->setHidden(true);

    abilitazioneinput(this);

    checkupdate = new updatecheck(ui->actionUpdate_writernote);

    setting_load(this);

    this->m_rubber = new rubber_ui(this);
    this->m_pen = new pen_ui(this);
    this->m_text = new text_ui(this);

    this->m_text_w = new text_widgets(this);

    this->m_sheet = new fast_sheet_ui(this);

    this->m_setting = new setting_restore_ui(this, &m_currenttitle, &m_indice, &m_path);

    this->m_sheet->setHidden(true);
    this->m_text_w->setHidden(true);
    this->m_text->setHidden(true);
    this->m_pen->setHidden(true);
    this->m_rubber->setHidden(true);

    this->m_canvas->m_rubber = m_rubber;
    this->m_canvas->m_pen_ui = m_pen;
    this->m_canvas->m_text = m_text;
    this->m_canvas->m_sheet = m_sheet;

    this->m_canvas->m_text_w = m_text_w;

    /* redo and undo */
    connect(this, &MainWindow::RedoT, m_canvas, &TabletCanvas::RedoM);
    connect(this, &MainWindow::UndoT, m_canvas, &TabletCanvas::Undo);

    ui->button_left_hide->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    ui->button_right_hide->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

#ifndef CLOUD
    ui->actioncloud->setVisible(false);
#endif

#define CHECKABLE(x) x->setCheckable(true)

    CHECKABLE(ui->actionpen);
    CHECKABLE(ui->actionrubber);
    CHECKABLE(ui->actionselezionetext);
    CHECKABLE(ui->actioninsertText);

    CHECKABLE(ui->actionwhite);
    CHECKABLE(ui->actionblack);
    CHECKABLE(ui->actionred);
    CHECKABLE(ui->actionyellow);

    updateTouch();

    ui->actionpen->setChecked(true);

    m_timer = new QTimer(this);
    QObject::connect(m_timer, &QTimer::timeout, this, &MainWindow::showRiascolto);

    QObject::connect(m_sheet, &fast_sheet_ui::changeButton, [=](bool res){
        ui->actionnewPage->setVisible(!res);
    });

    m_sheet->needToReload();

    setAcceptDrops(true);
    updatePageCount(-1);


    /* by default */
    ui->actionUpdate_writernote->setVisible(false);

    loadPenOrMouse();
}

MainWindow::~MainWindow()
{
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_INSERT_METHOD_PEN_MOUSE);
    setting.setValue(KEY_INSERT_METHOD_PEN_MOUSE, this->touch_or_pen);
    setting.endGroup();

    delete ui;
}

void MainWindow::update_touch_or_pen()
{
    ui->actionPen_or_Mouse->setChecked(!this->touch_or_pen);
}

/* create new file */
void MainWindow::on_actionNew_File_triggered()
{
    bool check;
    savecopybook checksave(this, &m_currentTitle);
    xmlstruct xml(&m_path, &m_indice, m_currenttitle);
    currenttitle_class __curr;
    indice_class __ind;
    n_need_save __res;

    if(this->m_indice.titolo.length() == 0)
        goto __continue;

    __res = needToSave(&xml, &__curr, &__ind);

    if(__res == n_need_save::not_
            || __res == n_need_save::only_writernote)
        goto __continue;

    if(areyousure(nullptr, "Unable to load file", "Unable to load file located in "+m_path)){
        check = checksave.check_permission();

        if(!check)
            return;
    }

    __continue:

    setWindowTitle("Writernote");
    updatePageCount(-1);
    m_currentTitle = "";
    m_currenttitle->reset();
    m_indice.reset();
    ui->listWidgetSX->clear();
    m_canvas->clear();
    setting_load(this);
    abilitazioneinput(this);
    setting_ui_start(this);
    m_path = "";
}

/* funzione che gestisce il controllo del riascolto dell'audio */
void MainWindow::on_textEdit_selectionChanged(){
    if(this->player->state() != QMediaPlayer::PlayingState)
            return;

    QString text = ui->textEdit->textCursor().selectedText();
    int position = ui->textEdit->textCursor().selectionStart();

    int i = 1, audio;
    while (true){
        if ((position >= m_currenttitle->testinohtml[i-1].length()) && (position <= m_currenttitle->testinohtml[i+1].length())){
            audio = m_currenttitle->posizione_iniz[i];
            this->player->setPosition(audio);

            this->player->setPosition(audio*1000);
            break;
        }
        else if ((i + 1) == m_currenttitle->posizione_iniz.length()){
            audio = m_currenttitle->posizione_iniz.last();

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

void MainWindow::setInZipAudio(){
    this->m_currenttitle->se_registato = currenttitle_class::record_zip;
}

void MainWindow::setExtAudio(){
    this->m_currenttitle->se_registato = currenttitle_class::record_file;

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
    if(this->m_audioRecorder->state() != QMediaRecorder::RecordingState)
        return;

    this->m_currenttitle->testi = this->ui->textEdit->toHtml();

    m_currenttitle->testinohtml.append(ui->textEdit->toPlainText());

    m_currenttitle->posizione_iniz.append(this->m_audioRecorder->duration()/1000);
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
    if(this->m_currenttitle->m_touch){
        this->m_canvas->m_redoundo->redo(&this->m_currenttitle);
        emit RedoT();
        return;
    }
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


void MainWindow::on_actionVersion_triggered()
{
#ifdef VERSION_SNAPCRAFT
    messaggio_utente("Current version is " + (QString)VERSION_STRING);
#else
    messaggio_utente("Current version is " + (QString)NO_VER_DEF);
#endif
}


void MainWindow::on_actionUndu_triggered()
{
    if(m_currenttitle->m_touch){
        this->m_canvas->m_redoundo->undo(&this->m_currenttitle);
        emit UndoT();
        return;
    }

    ui->textEdit->undo();
}

/* restore file to the original position (0, 0) */
void MainWindow::on_actionrestore_button_triggered()
{
    this->m_canvas->restoreO();
}

/* new page */
void MainWindow::on_actionnewPage_triggered()
{
    m_canvas->disegnofoglio_bool = true;
    m_canvas->needUpdate();
}

void MainWindow::loadPenOrMouse(){
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_INSERT_METHOD_PEN_MOUSE);
    touch_or_pen = setting.value(KEY_INSERT_METHOD_PEN_MOUSE, false).toBool();
    setting.endGroup();
    update_touch_or_pen();
}

void MainWindow::on_actionPen_or_Mouse_triggered()
{
    touch_or_pen = !touch_or_pen;
    update_touch_or_pen();
}
