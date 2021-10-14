#include "topdf.h"
#include "../mainwindow.h"
#include "../datawrite/qfilechoose.h"
#include "../utils/dialog_critic/dialog_critic.h"
#include <QPdfWriter>
#include <QPageSize>
#include "../log/log_ui/log_ui.h"
#include "../frompdf/frompdf.h"

topdf::topdf(const QString &path)
{
    this->path = &path;
}

static inline void newpage(Document *data, const double tmp){
    data->datatouch->scala_all(QPointF(0, -tmp));
}

bool topdf::createpdf(const bool withPdf){
    const uint lenpage = data->datatouch->posizionefoglio.length();
    uint i;

    this->translate();


    QPdfWriter pdfWriter(*this->path);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
#if !(defined(ANDROID_WRITERNOTE) || defined(IOS_WRITERNOTE))
    pdfWriter.setResolution(data->m_pdf->resolution);
#else
    pdfWriter.setResolution(100);
#endif

    const int height_pdf = pdfWriter.height();
    const int width_pdf = pdfWriter.width();

    topdf::adjastTranslation(data, width_pdf, height_pdf);

    const double size_orizzontale = data->datatouch->currentWidth();
    const double size_verticale = data->datatouch->currentHeight();

    const double delta = (double)width_pdf / (double)size_orizzontale;

    QPainter painter(&pdfWriter);
    
    for (i=0; i<lenpage; ++i) {
        this->draw(painter,
                   delta,
                   size_orizzontale,
                   size_verticale,
                   withPdf);

        if(i+1<lenpage){
            newpage(data, size_verticale);
            qDebug() << "New page " << i;
            if(!pdfWriter.newPage())
                return false;
        }
    }

    return true;
}

void MainWindow::on_actiontopdf_triggered()
{
    QString path_pdf;
    if(m_currenttitle->datatouch->isempty())
        return user_message("There is nothing to convert to pdf");

    if(!qfilechoose::filechoose(path_pdf, TYPEFILEPDF))
        return;

    topdf filepdf(path_pdf);

    filepdf.copy(m_currenttitle);

    if(!filepdf.createpdf(true)){
        dialog_critic("We had a problem saving the file to " + path_pdf);
        return;
    }
    user_message("Pdf file save in " + path_pdf);
}
