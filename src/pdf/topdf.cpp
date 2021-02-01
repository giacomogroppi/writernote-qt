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

    lenpagine = data->datatouch->posizionefoglio.length();

    QPdfWriter pdfWriter(*this->path);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));

    QPainter painter(&pdfWriter);

    int len = data->datatouch->x.length();
    for(i=0; i<len; i++)
        if(data->datatouch->idtratto.at(i) == -1)
            break;

    int size_orizzontale = data->datatouch->x.at(i+1);

    double size_verticale = pdfWriter.height();

    double delta = (double)pdfWriter.width() / (double)size_orizzontale;

    for (i=0; i<lenpagine; ++i) {
        this->draw(&painter, delta, size_orizzontale, size_verticale);

        if(i+1<lenpagine){
            newpage(data->datatouch);

            pdfWriter.newPage();
        }
    }
    return true;
}


void MainWindow::on_actiontoppf_triggered()
{
    if(!self->currenttitle.datatouch->x.length())
        return dialog_critic("There is nothing to convert to pdf");

    qfilechoose *temp = new qfilechoose(this);
    QString path_pdf;

    if(!temp->filechoose(&path_pdf, TYPEFILEPDF)){
        delete temp;
        return;
    }

    delete temp;

    topdf *filepdf = new topdf(&path_pdf);

    filepdf->copy(&self->currenttitle);


    if(!filepdf->createpdf()){
        dialog_critic("We had a problem saving the file to " + path_pdf);
    }

    delete filepdf;
}
