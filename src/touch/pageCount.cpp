#include "tabletcanvas.h"
#include "../mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::updatePageCount(int pageCount)
{
    QString text = "Page: " + QString::number(pageCount) + " of " + QString::number(this->m_currenttitle->datatouch->posizionefoglio.length());

    this->ui->page->setText(text);

}

static uint lastPage = 0;

void TabletCanvas::updatePageCount(){
    uint len = data->datatouch->posizionefoglio.length();

    if(!len)
        return;

    if(len == 1 && len != lastPage){
        lastPage = 1;

        parent->updatePageCount(len);
        return;
    }

    unsigned int i;
    bool find = false;

    len = data->datatouch->posizionefoglio.length();

    for(i=0; i<len-1; i++){
        if(data->datatouch->posizionefoglio.at(i+1) > 0){
            find = true;
            break;
        }
    }

    if(!find){
        lastPage = len;
        parent->updatePageCount((int)lastPage);
        return;
    }

    if(lastPage != i){
        lastPage = i;
        parent->updatePageCount(i+1);
    }
}
