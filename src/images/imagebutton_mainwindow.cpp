#include "../mainwindow.h"

#include "../touch/tabletcanvas.h"

#include "../touch/datastruct/datastruct.h"
#include "image_struct.h"
#include "ui_mainwindow.h"

/*
 * todo -> give the user the possibility to decide how to upload the image
*/
#define delta_point 200

static struct immagine_S *insert_image(const char *__pos,
                                       struct PointSettable * point){
    QString posizionefoto;

    if(!__pos){
        posizionefoto = QFileDialog::getOpenFileName(nullptr,
                                                     "Open images",
                                                     "JPEG (*.jpg, *.jpeg);;PNG (*.png);;All Files (*)");

        if(posizionefoto == "")
            return NULL;
    }

    struct immagine_S *immagine_temp = new struct immagine_S;
    QImage immagine(posizionefoto);
    immagine_temp->immagini = immagine;

    if(point){
        immagine_temp->i = point->point.toPoint();
        immagine_temp->f = point->point.toPoint() + QPoint(delta_point, delta_point);
    }
    else{
        immagine_temp->i = QPoint(0, 0);
        immagine_temp->f = QPoint(delta_point, delta_point);
    }
    return immagine_temp;
}

void addImage(currenttitle_class *m_currenttitle,
                     const char *__pos,
                     struct PointSettable * point){

    struct immagine_S *immagine = insert_image(__pos, point);
    if(!immagine)
        return;


    m_currenttitle->immagini.append(*immagine);

    delete immagine;
}

/* touch */
void MainWindow::on_insertimagebotton_triggered(QAction *){
    addImage(m_currenttitle, nullptr, nullptr);
    this->update_image();
}

void MainWindow::on_actioninsertImage_triggered(){
    addImage(m_currenttitle, nullptr, nullptr);

}
