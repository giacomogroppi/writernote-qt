#include "../../mainwindow.h"
#include "extract_audio_ui.h"

void MainWindow::on_actionExtract_audio_triggered()
{
    const void *pointer = (m_path != "") ? &m_path : nullptr;
    extract_audio_ui m_tmp(this, (QString *)pointer);
    m_tmp.exec();
}
