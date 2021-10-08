#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QListWidgetItem>
#include "datawrite/qfilechoose.h"
#include "dataread/xmlstruct.h"
#include "utils/dialog_critic/dialog_critic.h"
#include "savecopybook.h"
#include "setting_ui.h"
#include "datawrite/savefile.h"
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
#include "touch/scrollKinetic/ui_scroll/scroll.h"
#include "utils/common_def.h"
#include "currenttitle/option/option_copybook.h"
#include "log/log_ui/log_ui.h"
#include "ui/controlluibutton.h"
#include "../android/cpp/android/shareutils.h"

#ifdef PDFSUPPORT
#include "frompdf/frompdf.h"
#endif

MainWindow::MainWindow(QWidget *parent,
                       TabletCanvas *canvas,
                       struct struct_user *user,
                       cloud_controll *cloud,
                       const char *path)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->m_cloud = cloud;

    this->m_canvas = canvas;
    this->m_canvas->parent = this;
    this->m_user = user;

    ui->setupUi(this);

    this->m_currenttitle = new Document;

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
        user_message("We had an internal error with this error code: " + player->errorString());
    });

    this->m_buffer = new QBuffer(this);

    setting_ui_start(this);

    this->ui->layouteditor->insertWidget(1, this->m_canvas);

    contrUi();

    checkupdate = new updatecheck(ui->actionUpdate_writernote);

    setting_load(this);
    this->m_rubber = new rubber_ui(this);
    this->m_pen = new pen_ui(this);
    this->m_text = new text_ui(this);
    this->m_highlighter = new highlighter(this, &m_pen->same_data, m_pen);
    m_pen->m_highlighter = m_highlighter;
    this->m_option_copybook = new option_copybook(this);
    this->m_text_w = new text_widgets(this);
    this->m_sheet = new fast_sheet_ui(this);
    this->m_setting = new setting_restore_ui(this, &m_currenttitle, &m_path);
    NAME_LOG_EXT = new log_ui(this);
    this->m_controllUi = new ControllUiButton(this);
    this->m_share_file = new ShareUtils(this);

    this->m_sheet->setHidden(true);
    this->m_text_w->setHidden(true);
    this->m_text->setHidden(true);
    this->m_pen->setHidden(true);
    this->m_rubber->setHidden(true);
    this->m_highlighter->setHidden(true);


    this->m_canvas->m_rubber = m_rubber;
    this->m_canvas->m_pen_ui = m_pen;
    this->m_canvas->m_text = m_text;
    this->m_canvas->m_sheet = m_sheet;
    this->m_canvas->m_highlighter = m_highlighter;

    this->m_canvas->m_text_w = m_text_w;
    this->m_canvas->data = this->m_currenttitle;

    /* redo and undo */
    connect(this, &MainWindow::RedoT, m_canvas, &TabletCanvas::RedoM);
    connect(this, &MainWindow::UndoT, m_canvas, &TabletCanvas::Undo);

#ifndef CLOUD
    ui->actioncloud->setVisible(false);
#endif

    ui->actionpen->setCheckable(true);
    ui->actionrubber->setCheckable(true);
    ui->actionselezionetext->setCheckable(true);
    ui->actioninsertText->setCheckable(true);
    ui->actionhighlighter->setCheckable(true);

    ui->actionwhite->setCheckable(true);
    ui->actionblack->setCheckable(true);
    ui->actionred->setCheckable(true);
    ui->actionyellow->setCheckable(true);
    ui->actionbrown_color->setCheckable(true);
    ui->actionpurple->setCheckable(true);

    updateTouch();

    QObject::connect(m_sheet, &fast_sheet_ui::changeButton, [=](bool res){
        ui->actionnewPage->setVisible(!res);
    });

    m_sheet->needToReload();

    setAcceptDrops(true);
    updatePageCount(-1);

    /* by default */
    ui->actionUpdate_writernote->setVisible(false);

    loadPenOrMouse();

    this->m_canvas->settingdata(m_currenttitle, m_path);
    this->m_canvas->loadpixel();
    this->m_canvas->time = 0;

    if(path)
        openFile(path);
    resFileTmpInTmpFolder();

    this->setSizeButton();
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
    savecopybook checksave(this);
    xmlstruct xml(&m_path, m_currenttitle);
    Document __curr;
    n_need_save __res;

    __res = needToSave(&xml, &__curr);

    if(__res == n_need_save::not_
            || __res == n_need_save::only_writernote)
        goto __continue;

    if(areyousure("Unable to load file", "Unable to load file located in "+m_path)){
        check = checksave.check_permission();

        if(!check)
            return;
    }

    __continue:

    setWindowTitle("Writernote");
    updatePageCount(-1);
    m_currenttitle->reset();
    m_canvas->clear();
    contrUi();
    setting_ui_start(this);
    m_path = "";
}

void MainWindow::togglePause()
{
    if (m_audioRecorder->state() != QMediaRecorder::PausedState)
        m_audioRecorder->pause();
    else
        m_audioRecorder->record();
}

void MainWindow::setInZipAudio(){
    this->m_currenttitle->se_registato = Document::record_zip;
}

void MainWindow::setExtAudio(){
    this->m_currenttitle->se_registato = Document::record_file;

}

/* funzionche che viene invocata quando la registrazione dell'audio viene messa in pausa */
void MainWindow::on_pause_rec_triggered()
{
    if (this->m_audioRecorder->state() != QMediaRecorder::PausedState)
        this->m_audioRecorder->pause();
    else
        this->m_audioRecorder->record();
}

void MainWindow::on_actionRedo_triggered()
{
    this->m_canvas->m_redoundo->redo();
    emit RedoT();
}

void MainWindow::on_actionVersion_triggered()
{
#ifdef VERSION_SNAPCRAFT
    user_message("Current version is " + (QString)VERSION_STRING + "\nBuild in: " + __DATE__ + "\nAt " + __TIME__);
#else
    user_message("Current version is " + (QString)NO_VER_DEF + "\nBuild in: " + __DATE__ + "\nAt " + __TIME__);
#endif
}

void MainWindow::on_actionUndu_triggered()
{
    this->m_canvas->m_redoundo->undo();
    emit UndoT();
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

void MainWindow::on_actionPen_or_Mouse_triggered()
{
    touch_or_pen = !touch_or_pen;
    update_touch_or_pen();
}


void MainWindow::on_actionRemove_current_PDF_triggered()
{
#ifdef PDFSUPPORT
    this->m_currenttitle->m_pdf->reset();
#endif // PDFSUPPORT
}
