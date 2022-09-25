#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utils/get_only_name/get_only_name.h"

QString MainWindow::getNameCopybook()
{
    cbool unsave = this->m_path == "";

    if(!unsave){
        return NAME_UNSAVE;
    }

    /*
     * todo: return name without position
    */
    assert(0);
    return this->m_path;
}

void MainWindow::updateTitle()
{
    const QString name = get_only_name::exe(this->m_path);
    if(name == ""){
        setWindowTitle("Writernote");
    }
    setWindowTitle("Writernote - " + name);
}
