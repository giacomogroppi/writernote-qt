#include "tabletcanvas.h"
#include "../mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::updatePageCount(int pageCount)
{
    QString text;

    ui->page->setHidden(pageCount == -1);

    text = "Page: " + QString::number(pageCount) + " of " + QString::number(this->m_currenttitle->datatouch->posizionefoglio.length());

    this->ui->page->setText(text);

}

static uint lastPage = 0;

void TabletCanvas::updatePageCount(){
    uint len, i;

    len = data->datatouch->posizionefoglio.length();

    if(!len){
        parent->updatePageCount(-1);
        return;
    }

    if(len == 1 && lastPage == 0){
        lastPage = 1;
        parent->updatePageCount(lastPage);

        return;
    }

    for(i=0; i<len; ++i){
        if(data->datatouch->posizionefoglio.at(i) >= 0.0){
            break;
        }
    }

    if(i < (uint)data->datatouch->posizionefoglio.length())
        ++i;

    if(lastPage != i){
        lastPage = i;

        parent->updatePageCount(lastPage);
    }

}
