#include "topdf.h"

#include "../mainwindow.h"
#include "../datawrite/qfilechoose.h"
#include "../dialog_critic.h"

#include <QPdfWriter>
#include <QPageSize>

topdf::topdf(QString *path)
{
    this->path = path;
}

static void newpage(datastruct *datastruct){
    int i, len;
    len = datastruct->x.length();

    for(i=0; i<len; i++){
        datastruct->x[i] += NUMEROPIXELPAGINA;
    }
}

/* the function draw the pixel into the painter  */
inline void topdf::draw( QPainter *painter, double m){
    int i_, len;

    len = this->data->x.length();


    for(i_ = 0; i_ < len; i_++)
    {
        if(this->data->y.at(i_) < NUMEROPIXELORIZZONALI && this->data->y.at(i_) >= 0){
            /* se cambio il tratto non disegna ma lo carica in lastpoint solamente */
            if(i_ && this->data->idtratto.at(i_) == this->data->idtratto.at(i_ - 1) && data->idtratto.at(i_) != -1
                    && data->y.at(i_) != 1.00
                    && data->y.at(i_) != (double)NUMEROPIXELORIZZONALI){
                this->updateBrush_load(data->pressure.at(i_), data->color.at(i_));

                painter->setPen(this->penna);
                painter->drawLine(this->lastPoint.pos*m,
                              QPointF(this->data->x.at(i_)*m, this->data->y.at(i_)*m));
            }
            else if(data->idtratto.at(i_) == -1){
                /* carica i punti della pagina */
                updateBrush_load(data->pressure.at(i_), data->color.at(i_));

                painter->setPen(this->penna);
                painter->drawLine(data->x.at(i_)*m, data->y.at(i_)*m
                                  , data->x.at(i_ + 1)*m, data->y.at(i_ + 1)*m);

                /* deve andare aventi di un punto in quando ogni riga è formata da due punti */
                i_++;
            }

            lastPoint.pos.setX(this->data->x.at(i_));
            lastPoint.pos.setY(this->data->y.at(i_));
        }
    }
}

bool topdf::createpdf(){
    this->translate();

    int i, lenpagine;

    lenpagine = this->data->posizionefoglio.length();

    QPdfWriter pdfWriter(*this->path);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));

    QPainter painter(&pdfWriter);

    double delta = (double)pdfWriter.height() / (double)NUMEROPIXELORIZZONALI;

    for (i=0; i<lenpagine; ++i) {
        this->draw(&painter, delta);

        newpage(this->data);

        pdfWriter.newPage();
    }
    return true;
}


void MainWindow::on_actiontoppf_triggered()
{
    qfilechoose *temp = new qfilechoose(this);
    QString path_pdf;

    if(!temp->filechoose(&path_pdf, TYPEFILEPDF)){
        delete temp;
        return;
    }

    delete temp;

    topdf *filepdf = new topdf(&path_pdf);

    filepdf->copy(this->self->currenttitle.datatouch);


    if(!filepdf->createpdf()){
        dialog_critic("We had a problem saving the file to " + path_pdf);
    }

    delete filepdf;
}
