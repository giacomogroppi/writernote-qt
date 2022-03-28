#include "topdf.h"
#include "mainwindow.h"
#include "datawrite/qfilechoose.h"
#include "utils/dialog_critic/dialog_critic.h"
#include <QPdfWriter>
#include <QPageSize>
#include "log/log_ui/log_ui.h"
#include "frompdf/frompdf.h"
#include "testing/memtest.h"

topdf::topdf(const QString &path, const Document &doc)
{
    uint i;
    const uint len = doc.datatouch->lengthPage();

    QList<int> page;

    this->path = &path;

    WNew(data, Document, (doc));

    this->data->datatouch->triggerNewView(-1, true);

    for(i = 0; i < len; i++){
        if(data->datatouch->at(i).getImg().isNull())
            page.append((int)i);
    }

    data->datatouch->triggerNewView(page, -1, true);

    Q_ASSERT(doc.datatouch->lengthPage() == data->datatouch->lengthPage());
}

topdf::~topdf()
{
    WDelete(data);
}

static inline void newpage(Document *data, const double tmp)
{
    data->datatouch->scala_all(QPointF(0, -tmp), INT_MAX);
}

bool topdf::createpdf(const bool withPdf){
    const uint lenpage = data->datatouch->lengthPage();
    uint i;
    const QPointF pointData = data->datatouch->getPointFirstPage();
    uchar ret = 1;
    this->translate();


    QPdfWriter pdfWriter(*this->path);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
#if !(defined(ANDROID_WRITERNOTE) || defined(IOS_WRITERNOTE)) && defined(PDFSUPPORT)
    pdfWriter.setResolution(data->m_pdf->resolution);
#else
    pdfWriter.setResolution(100);
#endif

    //const int height_pdf = pdfWriter.height();
    const int width_pdf = pdfWriter.width();

    const double size_orizzontale = data->datatouch->currentWidth();
    const double size_verticale = page::getProportion() * size_orizzontale;

    const double delta = double(width_pdf) / double(size_orizzontale);

    QPainter painter(&pdfWriter);
    
    for (i=0; i<lenpage; ++i) {
        this->draw(painter,
                   delta,
                   withPdf);

        if(i+1<lenpage){
            newpage(data, size_verticale);

            if(!pdfWriter.newPage()){
                ret = 0;
                goto release;
            }
        }
    }

    release:
    data->datatouch->scala_all(pointData, INT_MAX);
    return ret;
}

void MainWindow::on_actiontopdf_triggered()
{
    QString path_pdf;
    const Document *doc = _canvas->data;

    if(doc->datatouch->isempty())
        return user_message("There is nothing to convert to pdf");

    if(!qfilechoose::getFileForSave(path_pdf, TYPEFILEPDF))
        return;

    topdf filepdf(path_pdf, *doc);

    if(!filepdf.createpdf(true)){
        dialog_critic("We had a problem saving the file to " + path_pdf);
        return;
    }
    user_message("Pdf file save in " + path_pdf);
}
