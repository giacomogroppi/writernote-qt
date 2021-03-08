#include "../../mainwindow.h"
#include "extract_audio_ui.h"

#define EXE(x) x.exec()

void MainWindow::on_actionExtract_audio_triggered()
{
    if(m_indice.titolo.length() != 0){
        extract_audio_ui m_temp(this, &m_indice, &m_path);
        EXE(m_temp);
    }
    else{
        extract_audio_ui m_temp(this, nullptr, nullptr);
        EXE(m_temp);
    }

}
