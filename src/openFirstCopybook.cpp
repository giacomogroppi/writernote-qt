#include "mainwindow.h"
#include "ui_mainwindow.h"

QString MainWindow::getNameCopybook()
{
    const bool unsave = this->m_path == "";

    if(!unsave){
        return NAME_UNSAVE;
    }

    /*
     * todo: return name without position
    */
    assert(0);
    return this->m_path;
}

void MainWindow::updateTitle(Document *__curr)
{
    setWindowTitle("Writernote - " + __curr->nome_copybook);
}
