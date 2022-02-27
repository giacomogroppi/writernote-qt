#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QListWidgetItem>
#include "datawrite/qfilechoose.h"
#include "dataread/xmlstruct.h"
#include "utils/dialog_critic/dialog_critic.h"
#include "savecopybook.h"
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
#include "touch/property/property_control.h"
#include "audioplay/audioplay.h"
#include "audiorecord/audiorecord.h"
#include "touch/multi_thread_data.h"
#include "touch/laser/laser.h"
#include <QString>

#ifdef PDFSUPPORT
#include "frompdf/frompdf.h"
#endif

MainWindow::MainWindow(TabletCanvas *canvas,
                       struct struct_user *user,
                       cloud_controll *cloud,
                       const char *path)
    : QMainWindow(NULL)
    , ui(new Ui::MainWindow)
{

    W_ASSERT(canvas);

#ifdef CLOUD
    this->m_cloud = cloud;
    this->m_user = user;
#else
    Q_UNUSED(user);
    Q_UNUSED(cloud);
#endif

    this->m_canvas = canvas;
    this->m_canvas->parent = this;
    DataPrivateInit();

    ui->setupUi(this);

    this->m_buffer = new QBuffer(this);

    //this->ui->layouteditor->insertWidget(1, this->m_canvas);
    this->_scroll = new QScrollArea(this);
    m_canvas->setGeometry(0, 0, 10000, 10000);
    _scroll->setWidget(m_canvas);
    _scroll->setAlignment(Qt::AlignHCenter);
    this->ui->layouteditor->insertWidget(1, _scroll);

    checkupdate = new class updatecheck(ui->actionUpdate_writernote);

    setting_load(this);
    this->m_rubber              = new class rubber_ui(this);
    this->m_text                = new class text_ui(this);
    this->m_pen                 = new class pen_ui(this);
    this->m_highlighter         = new class highlighter(this, &m_pen->same_data, m_pen);
    this->m_pen->m_highlighter  = m_highlighter;
    this->m_option_copybook     = new class option_copybook(this);
    this->m_text_w              = new class text_widgets(this, m_canvas);
    this->m_sheet               = new class fast_sheet_ui(this);
    this->m_setting             = new class setting_restore_ui(this, &m_canvas->data, &m_path);
    NAME_LOG_EXT                = new class log_ui(this);
    this->m_controllUi          = new class ControllUiButton(this);
    this->m_audioplayer         = new class audioplay(this);
    this->m_audio_recorder      = new class AudioRecord(this);
    this->m_laser               = new class laser(m_canvas);

#if defined(ANDROID_WRITERNOTE) || defined(IOS_WRITERNOTE)
    this->m_share_file = new ShareUtils(this);
#endif // mobile device

    this->m_sheet->setHidden(true);
    this->m_text_w->setHidden(true);
    this->m_text->setHidden(true);
    this->m_pen->setHidden(true);
    this->m_rubber->setHidden(true);
    this->m_highlighter->setHidden(true);

    m_canvas->m_rubber      = m_rubber;
    m_canvas->m_pen_ui      = m_pen;
    m_canvas->m_text        = m_text;
    m_canvas->m_sheet       = m_sheet;
    m_canvas->m_highlighter = m_highlighter;
    m_canvas->m_text_w      = m_text_w;
    m_canvas->m_laser       = m_laser;

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
    ui->actionLaser->setCheckable(true);

    ui->actionwhite->setCheckable(true);
    ui->actionblack->setCheckable(true);
    ui->actionred->setCheckable(true);
    ui->actionyellow->setCheckable(true);
    ui->actionbrown_color->setCheckable(true);
    ui->actionpurple->setCheckable(true);

    ui->sliderZoom->setValue(100);

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

    this->m_canvas->loadpixel();

    if(path)
        openFile(path);
    resFileTmpInTmpFolder();

    this->setSizeButton();
    contrUi();

    m_canvas->data->datatouch->triggerVisibility(page::getHeight() * m_canvas->data->datatouch->lengthPage());
}

MainWindow::~MainWindow()
{
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_INSERT_METHOD_PEN_MOUSE);
    setting.setValue(KEY_INSERT_METHOD_PEN_MOUSE, this->touch_or_pen);
    setting.endGroup();

    delete ui;
    ui = NULL;
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
    xmlstruct xml(&m_path, m_canvas->data);
    Document __curr;
    n_need_save __res;

    __res = needToSave(xml, __curr);

    if(__res == n_need_save::not_ || __res == n_need_save::only_writernote)
        goto __continue;

    if(__res == n_need_save::unable_load){
        if(!areyousure("Unable to load", QString("Unable to load file %1, do you want to continue?").arg(m_path))){
            return;
        }
    }
    else{
        check = checksave.check_permission();

        if(!check)
            return;
    }

__continue:

    setWindowTitle("Writernote");
    updatePageCount(-1);
    m_canvas->data->reset();

    m_canvas->clear();
    contrUi();
    m_path = "";
}

void MainWindow::togglePause()
{
    if (m_audio_recorder->isPauseRecording())
        m_audio_recorder->pauseRecord();
    else
        m_audio_recorder->startRecord();
}

/* funzionche che viene invocata quando la registrazione dell'audio viene messa in pausa */
void MainWindow::on_pause_rec_triggered()
{
    if (!m_audio_recorder->isPauseRecording())
        m_audio_recorder->pauseRecord();
    else
        m_audio_recorder->startRecord();
}

void MainWindow::on_actionRedo_triggered()
{
    this->m_canvas->m_redoundo->redo();
    emit RedoT();
}

void MainWindow::on_actionVersion_triggered()
{
    const QString bit = "\n" + ((sizeof(void *) < 8 ) ? QString("32 bit") : QString("64 bit"));
#ifdef VERSION_SNAPCRAFT
    user_message("Current version is " + (QString)VERSION_STRING + "\nBuild in: " + __DATE__ + "\nAt " + __TIME__ + bit);
#else
    user_message("Current version is " + (QString)NO_VER_DEF + "\nBuild in: " + __DATE__ + "\nAt " + __TIME__ + bit);
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
    datastruct *data = m_canvas->data->datatouch;
    data->triggerNewView(-1, true);
    data->triggerVisibility(m_canvas->height());

    m_canvas->restoreO();
}

/* new page */
void MainWindow::on_actionnewPage_triggered()
{
    this->m_canvas->data->datatouch->newPage(m_canvas->m_sheet->WhatIsSelected());
    m_canvas->call_update();
    m_canvas->updatePageCount();
}

void MainWindow::on_actionPen_or_Mouse_triggered()
{
    touch_or_pen = !touch_or_pen;
    update_touch_or_pen();
}

void MainWindow::on_actionRemove_current_PDF_triggered()
{
#ifdef PDFSUPPORT
    m_canvas->data->m_pdf->reset();
#endif // PDFSUPPORT
}

void MainWindow::on_buttonFullScreen_clicked()
{
    this->setFullScreen();
}

void MainWindow::on_actionFull_Screen_triggered()
{
    this->setFullScreen();
}

