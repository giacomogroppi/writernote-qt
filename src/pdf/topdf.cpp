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

bool topdf::createpdf(){
    this->translate();

    int i, lenpagine;

    lenpagine = this->data->posizionefoglio.length();

    QPdfWriter pdfWriter(*this->path);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));

    QPainter painter(&pdfWriter);

    int len = data->x.length();
    for(i=0; i<len; i++)
        if(data->idtratto.at(i) == -1)
            break;


    double delta = (double)pdfWriter.width() / (double)data->x.at(i+1);

    for (i=0; i<lenpagine; ++i) {
        this->draw(&painter, delta);

        if(i+1<lenpagine){
            newpage(this->data);

            pdfWriter.newPage();
        }
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
