#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "controlluibutton.h"

void MainWindow::on_buttonRecentFile_clicked()
{
    this->on_actionRecent_file_triggered();
}

void MainWindow::on_actionChange_visual_triggered()
{
    this->m_controllUi->change();
}

void MainWindow::on_buttonChangeVisual_clicked()
{
    this->m_controllUi->change();
}

void MainWindow::on_buttonPen_clicked()
{
    this->on_actionpen_triggered();
}

void MainWindow::on_buttonRubber_clicked()
{
    this->on_actionrubber_triggered();
}

void MainWindow::on_buttonselezionetext_clicked()
{
    this->on_actionselezionetext_triggered();
}

void MainWindow::on_buttonInsertText_clicked()
{
    this->on_actioninsertText_triggered();
}

void MainWindow::on_buttonHighlighter_clicked()
{
    this->on_actionhighlighter_triggered();
}

void MainWindow::on_buttonLaser_clicked()
{
    this->on_actionLaser_triggered();
}

void MainWindow::on_buttonBlack_clicked()
{
    this->on_actionblack_triggered();
}

void MainWindow::on_buttonWhite_clicked()
{
    this->on_actionwhite_triggered();
}

void MainWindow::on_buttonYellow_clicked()
{
    this->on_actionyellow_triggered();
}

void MainWindow::on_buttonBrown_clicked()
{
    this->on_actionbrown_color_triggered();
}

void MainWindow::on_buttonPurple_clicked()
{
    this->on_actionpurple_triggered();
}

void MainWindow::on_buttonChooseColor_clicked()
{
    this->on_actionchoose_color_triggered();
}

void MainWindow::on_buttonInsertImage_clicked()
{
    this->on_actioninsertImage_triggered();
}

void MainWindow::on_buttonSheet_clicked()
{
    this->on_actionnew_sheet_triggered();
}

void MainWindow::on_buttonNewPage_clicked()
{
    this->on_actionnewPage_triggered();
}

void MainWindow::on_buttonRestore_clicked()
{
    this->on_actionrestore_button_triggered();
}

void MainWindow::on_buttonPenOrMouse_clicked()
{
    this->on_actionPen_or_Mouse_triggered();
}

void MainWindow::on_buttonStartRecording_clicked()
{
    this->on_start_rec_triggered();
}

void MainWindow::on_buttonStopRecording_clicked()
{
    this->on_stop_rec_triggered();
}

void MainWindow::on_buttonPauseRecording_clicked()
{
    this->on_pause_rec_triggered();
}

void MainWindow::on_buttonListen_current_audio_clicked()
{
    this->on_actionListen_current_audio_triggered();
}

void MainWindow::on_buttonUndu_clicked()
{
    this->on_actionUndu_triggered();
}

void MainWindow::on_buttonRedo_clicked()
{
    this->on_actionRedo_triggered();
}
