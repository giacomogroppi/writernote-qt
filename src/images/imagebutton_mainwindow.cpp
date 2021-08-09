#include "../mainwindow.h"

#include "../touch/tabletcanvas.h"

#include "../touch/datastruct/datastruct.h"
#include "ui_mainwindow.h"

/* touch */
void MainWindow::on_insertimagebotton_triggered(QAction *){
    addImage(m_currenttitle, nullptr, nullptr);
    this->update_image();
}

void MainWindow::on_actioninsertImage_triggered(){
    addImage(m_currenttitle, nullptr, nullptr);

}
