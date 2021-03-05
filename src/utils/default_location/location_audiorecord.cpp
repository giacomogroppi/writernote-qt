#include "../../mainwindow.h"
#include "audio_default_location.h"

void MainWindow::on_actionDefault_save_location_triggered()
{
    audio_default_location m_audio;
    m_audio.exec();
}
