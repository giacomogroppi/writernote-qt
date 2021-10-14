#include "topdf.h"
#include "../mainwindow.h"
#include "../datawrite/qfilechoose.h"
#include "../utils/dialog_critic/dialog_critic.h"
#include <QPdfWriter>
#include <QPageSize>
#include "../log/log_ui/log_ui.h"

topdf::topdf(QString *path, TabletCanvas *canvas)
{
    this->path = path;
    m_canvas = canvas;
}

static void newpage(Document *data, const double tmp){
    data->datatouch->scala_all(QPointF(0, tmp));
}

bool topdf::createpdf(const bool withPdf){
    this->translate();

    QPdfWriter pdfWriter(*this->path);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));

    const int height_pdf = pdfWriter.height();
    const int width_pdf = pdfWriter.width();

    const uint lenpage = data->datatouch->posizionefoglio.length();
    const double size_orizzontale = data->datatouch->biggerx();
    const double size_verticale = height_pdf;
    const double delta = (double)width_pdf / (double)size_orizzontale;
    QPainter painter(&pdfWriter);
    double tmp_ret;
    uint i;
    
    for (i=0; i<lenpage; ++i) {
        this->draw(painter,
                   delta,
                   size_orizzontale,
                   size_verticale/delta,
                   &tmp_ret, withPdf);

        if(i+1<lenpage){
            newpage(data, tmp_ret);

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


    if(!filepdf.createpdf(true)){
        dialog_critic("We had a problem saving the file to " + path_pdf);
        return;
    }
    user_message("Pdf file save in " + path_pdf);
}
