#include "tabletcanvas.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "log/log_ui/log_ui.h"

static int __page = -2;
static int __max = 0;
void MainWindow::updatePageCount(int pageCount)
{
    QString text;
    cint lenPage = this->m_canvas->data->datatouch->lengthPage();

    ui->page->setHidden(pageCount == -1);

    text = "Page: " + QString::number(pageCount) + " of " + QString::number(lenPage);

    if(__page == pageCount && lenPage == __max) return;

    __max = lenPage;
    __page = pageCount;

    this->ui->page->setText(text);
}

void TabletCanvas::updatePageCount(){
    datastruct *__data = data->datatouch;
    const int lenPage = __data->lengthPage();
    const double deltay = __data->getPointFirstPage().y();

    for(int i = 0; i < lenPage; i++){
        const double currentHeight = __data->at(i).currentHeight();
        if(currentHeight + deltay > 0.0)
            return parent->updatePageCount(i + 1);
    }

    parent->updatePageCount(-1);

    NAME_LOG_EXT->write("Missing page", log_ui::possible_bug);
}
