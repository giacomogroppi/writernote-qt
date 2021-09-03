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

static void newpage(datastruct *datastruct, const double tmp){
    int i, len;
    len = datastruct->length();

    for(i=0; i<len; i++){
        //datastruct->at_mod(i)->m_y += tmp;
        datastruct->scala_all(QPointF(0, tmp));
    }

}

bool topdf::createpdf(){
    this->translate();

    QPdfWriter pdfWriter(*this->path);
    const int height_pdf = pdfWriter.height();
    const int width_pdf = pdfWriter.width();
    uint i;
    const uint lenpage = data->datatouch->posizionefoglio.length();
    double tmp_ret;
    const double size_orizzontale = data->datatouch->biggerx();
    const double size_verticale = height_pdf;
    const double delta = (double)width_pdf / (double)size_orizzontale;
    QPainter painter(&pdfWriter);

    pdfWriter.setPageSize(QPageSize(QPageSize::A4));    

    for (i=0; i<lenpage; ++i) {
        this->draw(painter,
                   delta,
                   size_orizzontale,
                   size_verticale,
                   &tmp_ret);

        if(i+1<lenpage){
            newpage(data->datatouch, tmp_ret);

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
