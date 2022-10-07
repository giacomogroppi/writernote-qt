#include "mainwindow.h"
#include "touch/tabletcanvas.h"
#include "touch/dataTouch/datastruct/datastruct.h"
#include "ui_mainwindow.h"
#include "images/fromimage.h"
#include "utils/dialog_critic/dialog_critic.h"

static inline bool Message(const QString &m_path)
{
    if(m_path == ""){
        dialog_critic("It's not possible to add image without saving this file");
        return false;
    }
    return true;
}

void MainWindow::on_actioninsertImage_triggered()
{
    QString pos;

    if(!Message(this->m_path)){
        return;
    }

    pos = "";
    const auto res = _canvas->getDoc()->addImage(pos, nullptr, this->m_path);

    if(res < 0){
        return dialog_critic(QApplication::tr("We had some problems importing the image"));
    }

    _canvas->update();
}
