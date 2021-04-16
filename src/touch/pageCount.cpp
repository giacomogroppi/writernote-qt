#include "tabletcanvas.h"
#include "../mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::updatePageCount(int pageCount)
{
    QString text = "Page: " + QString::number(pageCount) + " of " + QString::number(this->m_currenttitle->datatouch->posizionefoglio.length());

    this->ui->page->setText(text);

}

static uint lastPage = -1;

void TabletCanvas::updatePageCount(){
    uint len = data->datatouch->posizionefoglio.length();

    if(!len)
        return;

    if(len == 1 && lastPage == 0){
        lastPage = 1;
        parent->updatePageCount(lastPage);

        return;
    }

    uint i;
    for(i=0; i<len; ++i){
        if(data->datatouch->posizionefoglio.at(i) < 0)
            break;
    }

    ++i;

    if(lastPage != i){
        lastPage = i;

        parent->updatePageCount(lastPage);
    }

}
