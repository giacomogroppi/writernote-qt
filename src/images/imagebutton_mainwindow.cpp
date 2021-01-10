#include "../mainwindow.h"

#include "../touch/tabletcanvas.h"

void MainWindow::on_insertimagebotton_triggered(QAction *arg1)
{
    QString posizionefoto;

    posizionefoto = QFileDialog::getOpenFileName(this,
                                                 "Open images",
                                                 "JPEG (*.jpg, *.jpeg);;PNG (*.png);;All Files (*)");
    if (posizionefoto == "")
        return;

    QImage immagine(posizionefoto);
    self->currenttitle.datatouch->immagini.append(immagine);

    this->m_canvas->isloading = true;
    this->m_canvas->update();
}
