#include "topdf.h"

#include "../mainwindow.h"
#include "../datawrite/qfilechoose.h"
#include "../utils/dialog_critic/dialog_critic.h"

#include <QPdfWriter>
#include <QPageSize>

topdf::topdf(QString *path, TabletCanvas *canvas)
{
    this->path = path;
    m_canvas = canvas;
}

static void newpage(datastruct *datastruct, double temp){
    int i, len;
    len = datastruct->length();

    for(i=0; i<len; i++){
        datastruct->at_mod(i)->m_y += temp;
    }

}

bool topdf::createpdf(){
    this->translate();

    uint i, lenpagine;
    double size_orizzontale, size_verticale, delta, temp_ret;

    lenpagine = data->datatouch->posizionefoglio.length();

    QPdfWriter pdfWriter(*this->path);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));

    QPainter painter(&pdfWriter);

    size_orizzontale = data->datatouch->biggerx();

    size_verticale = pdfWriter.height();

    delta = (double)pdfWriter.width() / (double)size_orizzontale;
    for (i=0; i<lenpagine; ++i) {
        this->draw(painter,
                   delta,
                   size_orizzontale,
                   size_verticale,
                   &temp_ret);

        if(i+1<lenpagine){
            newpage(data->datatouch, temp_ret);

            pdfWriter.newPage();
        }
    }
    return true;
}

void MainWindow::on_actiontopdf_triggered()
{
    if(m_currenttitle->datatouch->isempty())
        return user_message("There is nothing to convert to pdf");

    QString path_pdf;

    if(!qfilechoose::filechoose(path_pdf, TYPEFILEPDF))
        return;

    topdf filepdf(&path_pdf, m_canvas);

    filepdf.copy(m_currenttitle);


    if(!filepdf.createpdf()){
        dialog_critic("We had a problem saving the file to " + path_pdf);
        return;
    }
    user_message("Pdf file save in " + path_pdf);
}
