#include "toolbar.h"
#include "mainwindow.h"
#include "ui/controlluibutton.h"
#include "ui_mainwindow.h"
#include "ui_toolbar.h"
#include "utils/common_script.h"
#include "core/core.h"

ToolBar::ToolBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ToolBar)
{
    constexpr auto colorOver = "#e0e0e0";
    W_ASSERT(parent->objectName() == "MainWindow");
    const auto color = ((MainWindow *)parent)->ui->toolBarcolore->palette().color(parent->backgroundRole());

    ui->setupUi(this);

    this->setStyleSheet(qstr("QPushButton{border:5px solid %1;} QPushButton:hover{border:10px solid %2} QPushButton:checked{border:15px solid %2}").
                        arg(color.name()).arg(colorOver));
}

ToolBar::~ToolBar()
{
    delete ui;
}

void ToolBar::Hide()
{
    this->hide();
}

void ToolBar::Show()
{
    this->show();
}

void ToolBar::setSpacer(QSizePolicy::Policy hData)
{
    auto makeSize = [](QSpacerItem * item, QSizePolicy::Policy hData){
        item->changeSize(0, 0, hData);
    };

    makeSize(ui->horizontalSpacer, hData);
    makeSize(ui->horizontalSpacer_2, hData);
    makeSize(ui->horizontalSpacer_3, hData);
    makeSize(ui->horizontalSpacer_4, hData);
}

void ToolBar::on_button_back_clicked()
{

}

void ToolBar::on_button_recent_file_clicked()
{
    core::get_main_window()->on_actionRecent_file_triggered();
}

void ToolBar::on_button_pen_clicked()
{
    core::get_main_window()->on_actionpen_triggered();
}

void ToolBar::on_button_rubber_clicked()
{
    core::get_main_window()->on_actionrubber_triggered();
}

void ToolBar::on_button_cut_clicked()
{
    core::get_main_window()->on_actionselezionetext_triggered();
}

void ToolBar::on_button_text_clicked()
{
    core::get_main_window()->on_actioninsertText_triggered();
}

void ToolBar::on_button_highlighter_clicked()
{
    core::get_main_window()->on_actionhighlighter_triggered();
}

void ToolBar::on_button_laser_clicked()
{
    core::get_main_window()->on_actionLaser_triggered();
}

void ToolBar::on_button_color_black_clicked()
{
    core::get_main_window()->on_actionblack_triggered();
}

void ToolBar::on_button_color_white_clicked()
{
    core::get_main_window()->on_actionwhite_triggered();
}

void ToolBar::on_button_color_yellow_clicked()
{
    core::get_main_window()->on_actionyellow_triggered();
}

void ToolBar::on_button_color_brown_clicked()
{
    core::get_main_window()->on_actionbrown_color_triggered();
}

void ToolBar::on_button_color_purple_clicked()
{
    core::get_main_window()->on_actionpurple_triggered();
}

void ToolBar::on_button_choose_color_clicked()
{
    core::get_main_window()->on_actionchoose_color_triggered();
}

void ToolBar::on_button_insert_image_clicked()
{
    core::get_main_window()->on_actioninsertImage_triggered();
}

void ToolBar::on_button_new_page_clicked()
{
    core::get_main_window()->on_actionnewPage_triggered();
}

void ToolBar::on_button_restore_clicked()
{
    core::get_main_window()->on_actionrestore_button_triggered();
}

void ToolBar::on_button_pen_or_mouse_clicked()
{
    core::get_main_window()->on_actionPen_or_Mouse_triggered();
}

void ToolBar::on_button_tablet_view_clicked()
{
    core::set_mobile_view(false);
    core::get_main_window()->m_controllUi->change();
}

void ToolBar::on_button_full_screen_clicked()
{
    core::get_main_window()->on_buttonFullScreen_clicked();
}

void ToolBar::on_button_hide_preview_clicked()
{
    core::get_main_window()->on_actionHide_Show_Preview_triggered();
}

void ToolBar::on_button_rec_start_clicked()
{
    core::get_main_window()->on_start_rec_triggered();
}

void ToolBar::on_button_rec_stop_clicked()
{
    core::get_main_window()->on_stop_rec_triggered();
}

void ToolBar::on_button_rec_pause_clicked()
{
    core::get_main_window()->on_pause_rec_triggered();
}

void ToolBar::on_button_replay_clicked()
{
    core::get_main_window()->on_actionListen_current_audio_triggered();
}

void ToolBar::on_button_undo_clicked()
{
    core::get_main_window()->on_actionUndu_triggered();
}

void ToolBar::on_button_redo_clicked()
{
    core::get_main_window()->on_actionRedo_triggered();
}

