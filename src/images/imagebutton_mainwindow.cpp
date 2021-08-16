#include "../mainwindow.h"
#include "../touch/tabletcanvas.h"
#include "../touch/datastruct/datastruct.h"
#include "ui_mainwindow.h"
#include "../images/fromimage.h"
#include "../utils/dialog_critic/dialog_critic.h"

static bool Message(const QString &m_path){
    if(m_path == ""){
        dialog_critic("It's not possible to add image without saving this file");
        return false;
    }
    return true;
}

/* touch */
void MainWindow::on_insertimagebotton_triggered(QAction *){
    QString pos;

    if(!Message(this->m_path)){
        return;
    }

    pos = "";
    m_currenttitle->m_img->addImage(pos, nullptr, this->m_path);
    this->m_canvas->update();
}

void MainWindow::on_actioninsertImage_triggered(){
    QString pos;

    if(!Message(this->m_path)){
        return;
    }

    pos = "";
    m_currenttitle->m_img->addImage(pos, nullptr, this->m_path);

}
