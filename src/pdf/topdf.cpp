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
    len = datastruct->m_point.length();

    for(i=0; i<len; i++){
        datastruct->m_point.operator[](i).m_y += temp;
    }

    /*
     * last data struct
    */
    /*
    len = datastruct->x.length();

    for(i=0; i<len; i++){
        datastruct->x[i] += NUMEROPIXELPAGINA;
    }*/
}

bool topdf::createpdf(){
    this->translate();

    int i, lenpagine;

    lenpagine = data->datatouch->posizionefoglio.length();

    QPdfWriter pdfWriter(*this->path);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));

    QPainter painter(&pdfWriter);

    int len = data->datatouch->m_point.length();
    for(i=0; i<len; i++)
        if(data->datatouch->m_point.at(i).idtratto == IDORIZZONALE)
            break;

    int size_orizzontale = data->datatouch->m_point.at(i).m_x;

    /*
     * last data struct
    */
    /*
    int len = data->datatouch->x.length();
    for(i=0; i<len; i++)
        if(data->datatouch->idtratto.at(i) == -1)
            break;

    int size_orizzontale = data->datatouch->x.at(i+1);
    */

    double size_verticale = pdfWriter.height();

    double delta = (double)pdfWriter.width() / (double)size_orizzontale;
    double temp_return;

    for (i=0; i<lenpagine; ++i) {
        this->draw(&painter, delta, size_orizzontale, size_verticale, &temp_return);

        if(i+1<lenpagine){
            newpage(data->datatouch, temp_return);

            pdfWriter.newPage();
        }
    }
    return true;
}

void MainWindow::on_actiontopdf_triggered()
{
    if(m_currenttitle.datatouch->m_point.isEmpty())
        return dialog_critic("There is nothing to convert to pdf");

    qfilechoose temp(this);
    QString path_pdf;

    if(!temp.filechoose(&path_pdf, TYPEFILEPDF))
        return;


    topdf filepdf(&path_pdf, m_canvas);

    filepdf.copy(&m_currenttitle);


    if(!filepdf.createpdf()){
        dialog_critic("We had a problem saving the file to " + path_pdf);
    }
}
