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

void MainWindow::on_insertimagebotton_triggered(QAction *){
    struct immagine_S *immagine = insert_image();
    if(!immagine)
        return;


    self->currenttitle.immagini.append(*immagine);

    this->update_image();

    delete immagine;
}

void MainWindow::on_actioninsertImage_triggered(){
     struct immagine_S *immagine = insert_image();

    if(!immagine)
        return;

    self->currenttitle.datatouch->immagini.append(*immagine);

    this->m_canvas->isloading = true;
    this->m_canvas->update();

    delete immagine;
}
