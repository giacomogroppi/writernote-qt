#include "../mainwindow.h"

#include "../touch/tabletcanvas.h"

#include "../touch/datastruct/datastruct.h"

void MainWindow::on_insertimagebotton_triggered(QAction *arg1)
{
    QString posizionefoto;

    posizionefoto = QFileDialog::getOpenFileName(this,
                                                 "Open images",
                                                 "JPEG (*.jpg, *.jpeg);;PNG (*.png);;All Files (*)");
    if (posizionefoto == "")
        return;

    struct immagine_S immagine_temp;
    QImage immagine(posizionefoto);
    immagine_temp.immagini = immagine;

    immagine_temp.i = QPoint(0, 0);
    immagine_temp.f = QPoint(150, 150);

    self->currenttitle.datatouch->immagini.append(immagine_temp);

    this->m_canvas->isloading = true;
    this->m_canvas->update();
}
