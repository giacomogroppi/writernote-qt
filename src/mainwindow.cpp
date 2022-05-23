#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "datawrite/qfilechoose.h"
#include "dataread/xmlstruct.h"
#include "utils/dialog_critic/dialog_critic.h"
#include "savecopybook.h"
#include "datawrite/savefile.h"
#include "currenttitle/checksimilecopybook.h"
#include "windows/updatecheck.h"
#include "audioplay/aggiornotastiriascolto.h"
#include "utils/setting_define.h"
#include "utils/settings/setting_load.h"
#include "windows/mostra_finestra_i.h"
#include "utils/areyousure/areyousure.h"
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
#include "utils/choosepage.h"
#include "ui/manageMobile.h"
#include "core/core.h"
#include "utils/utils.h"
#include "preview_page_widget/preview_page_widget.h"
#include "ui/toolbar.h"
#include <QMessageBox>
#include <QListWidgetItem>
#include <QSettings>
#include <QTimer>
#include <QString>
#include <QRect>

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
    
    core::set_main_window(this);

    this->setAttribute(Qt::WA_AcceptTouchEvents);

#ifdef CLOUD
    this->m_cloud = cloud;
    this->m_user = user;
#else
    Q_UNUSED(user);
    Q_UNUSED(cloud);
#endif

    _canvas = canvas;
    DataPrivateInit();

    ui->setupUi(this);

    this->m_buffer = new QBuffer(this);

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    this->ui->layouteditor->insertWidget(1, _canvas);
#else
    this->ui->layouteditor->insertWidget(0, _canvas);
#endif

    checkupdate = new class updatecheck(ui->actionUpdate_writernote);
    
    setting_load(this);
    this->m_rubber              = new class rubber_ui(this);
    this->m_text                = new class text_ui(this);
    this->m_pen                 = new class pen_ui(this);
    this->m_highlighter         = new class highlighter(this, &m_pen->same_data, m_pen);
    this->m_pen->m_highlighter  = m_highlighter;
    this->m_option_copybook     = new class option_copybook(this);
    this->m_text_w              = new class text_widgets(this, _canvas);
    this->m_sheet               = new class fast_sheet_ui(this);
    this->m_setting             = new class setting_restore_ui(this, _canvas, &m_path);
    NAME_LOG_EXT                = new class log_ui(this);
    this->m_audioplayer         = new class audioplay(this);
    this->m_audio_recorder      = new class AudioRecord(this);
    this->m_laser               = new class laser(_canvas);
    this->_choose_page          = new class ChoosePage(this);

    this->_tool_bar             = new class ToolBar(this);
    this->ui->layout_tool_bar->addWidget(_tool_bar);

    this->m_controllUi          = new class ControllUiButton(this);

    this->_preview_widget       = new class preview_page_widget(this, this);
    this->ui->layouteditor->insertWidget(1, _preview_widget);

#if defined(ANDROID_WRITERNOTE) || defined(IOS_WRITERNOTE)
    this->m_share_file = new ShareUtils(this);
#endif // mobile device

    this->m_sheet->setHidden(true);
    this->m_text_w->setHidden(true);
    this->m_text->setHidden(true);
    this->m_pen->setHidden(true);
    this->m_rubber->setHidden(true);
    this->m_highlighter->setHidden(true);
    
    _canvas->_rubber      = m_rubber;
    _canvas->_pen_ui      = m_pen;
    _canvas->_text        = m_text;
    _canvas->_sheet       = m_sheet;
    _canvas->_highlighter = m_highlighter;
    _canvas->_text_w      = m_text_w;
    _canvas->_laser       = m_laser;
    
    /* redo and undo */
    connect(this, &MainWindow::RedoT, _canvas, &TabletCanvas::RedoM);
    connect(this, &MainWindow::UndoT, _canvas, &TabletCanvas::Undo);

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

    this->_canvas->loadpixel();

    if(path)
        openFile(path);
    resFileTmpInTmpFolder();

    this->setSizeButton();
    contrUi();

    mobile::make_ui(this);

    _canvas->data->datatouch->triggerVisibility(page::getHeight() * _canvas->data->datatouch->lengthPage());
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

bool MainWindow::event(QEvent *event)
{
    const auto not_used debugEvent = false;
    const auto name = "MainWindow::event";
    const auto type = event->type();

    WDebug(debugEvent, name << event->type());

    if(type == QEvent::MouseButtonPress){
        const auto *widget = ui->page;
        const auto sott_event = static_cast<QMouseEvent *>(event);
        const auto pos = sott_event->pos();

        const QPoint pos_object = ui->page->pos();
        cint w = widget->width();
        cint h = widget->height();

        const auto rect = QRect(
                    QPoint(pos_object.x(),
                           pos_object.y() + 90),
                    QPoint(
                        pos_object.x() + w,
                        pos_object.y() + h + 90
                        )
                    );

        W_ASSERT(rect.topRight().x() - rect.topLeft().x() == w);
        W_ASSERT(rect.bottomLeft().y() - rect.topLeft().y() == h);

        //qDebug() << __func__ << rect << pos;

        if(rect.contains(pos)){
            const QPointF pos_cursor = cursor().pos();
            const QSize size_widget = _choose_page->get_size();

            QPointF new_pos_widget = pos_cursor - QPointF(size_widget.width(), size_widget.height());
            this->_choose_page->Show(*_canvas->data);
            this->_choose_page->move(new_pos_widget.toPoint());
        }
    }

    return QWidget::event(event);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    constexpr not_used bool debugResizeMainWindow = true;
    constexpr auto minSizeX = 200;
    QSize sizeScreen;

    if(!core::is_mobile_view())
        return;

    if(is_mobile_static()){
        sizeScreen = utils::get_size_screen();
    }else{
        sizeScreen = event->size();
    }

    if(sizeScreen.width() < minSizeX){

    }

    WDebug(debugResizeMainWindow, "MainWindow::resizeEvent size:" << sizeScreen);
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
    xmlstruct xml(&m_path, _canvas->data);
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
    _canvas->data->reset();

    _canvas->clear();
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
    if(_canvas->_redoundo){
        this->_canvas->_redoundo->redo();
        emit RedoT();
    }
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
    if(_canvas->_redoundo){
        this->_canvas->_redoundo->undo();
        emit UndoT();
    }
}

/* restore file to the original position (0, 0) */
void MainWindow::on_actionrestore_button_triggered()
{
    datastruct *data = _canvas->data->datatouch;
    data->triggerNewView(-1, true);
    data->triggerVisibility(_canvas->height());

    _canvas->restoreO();
    this->updatePageCount(1);
    this->zoomChange();
}

/* new page */
void MainWindow::on_actionnewPage_triggered()
{
    this->_canvas->data->datatouch->newPage(_canvas->_sheet->WhatIsSelected());
    _canvas->call_update();
    _preview_widget->newPage();
    _canvas->updatePageCount();
}

void MainWindow::on_actionPen_or_Mouse_triggered()
{
    touch_or_pen = !touch_or_pen;
    update_touch_or_pen();
}

void MainWindow::on_actionRemove_current_PDF_triggered()
{
#ifdef PDFSUPPORT
    _canvas->data->m_pdf->reset();
#endif // PDFSUPPORT
}

void MainWindow::on_actionFull_Screen_triggered()
{
    this->setFullScreen();
}

void MainWindow::removePage(int index)
{
    auto *data = this->getCurrentDoc()->datatouch;
    data->removePage(index);
    this->_preview_widget->changeDocument();
    this->_canvas->updatePageCount();
}

void MainWindow::grabGestures(const QList<Qt::GestureType> &gestures)
{
    _canvas->grabGestures(gestures);
}

void MainWindow::on_actionHide_Show_Preview_triggered()
{
    if(_preview_widget->isVisible())
        this->_preview_widget->Hide();
    else
        this->_preview_widget->Show();
}

void MainWindow::on_actionChange_visual_triggered()
{
    core::set_mobile_view(true);
    this->m_controllUi->change();
}
