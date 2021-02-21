#include "../mainwindow.h"

#include "../touch/tabletcanvas.h"

#include "../touch/datastruct/datastruct.h"
#include "image_struct.h"
#include "ui_mainwindow.h"


static struct immagine_S *insert_image(){

    QString posizionefoto;

    posizionefoto = QFileDialog::getOpenFileName(nullptr,
                                                 "Open images",
                                                 "JPEG (*.jpg, *.jpeg);;PNG (*.png);;All Files (*)");

    if(posizionefoto == "")
        return NULL;

    struct immagine_S *immagine_temp = new struct immagine_S;
    QImage immagine(posizionefoto);
    immagine_temp->immagini = immagine;

    immagine_temp->i = QPoint(0, 0);
    immagine_temp->f = QPoint(150, 150);

    return immagine_temp;
}

static void addImage(currenttitle_class *m_currenttitle){
    struct immagine_S *immagine = insert_image();
    if(!immagine)
        return;


    m_currenttitle->immagini.append(*immagine);

    delete immagine;
}

/* touch */
void MainWindow::on_insertimagebotton_triggered(QAction *){
    addImage(&m_currenttitle);
    this->update_image();
}

void MainWindow::on_actioninsertImage_triggered(){
    addImage(&m_currenttitle);

}
