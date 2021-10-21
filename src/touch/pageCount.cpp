#include "tabletcanvas.h"
#include "../mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::updatePageCount(int pageCount)
{
    QString text;

    ui->page->setHidden(pageCount == -1);

    text = "Page: " + QString::number(pageCount) + " of " + QString::number(m_currenttitle->datatouch->lengthPage());

    this->ui->page->setText(text);

}

static uint lastPage = 0;

void TabletCanvas::updatePageCount(){
    uint i;

    const uint lenPage = data->datatouch->lengthPage();

    if(!lenPage){
        parent->updatePageCount(-1);
        return;
    }

    if(lenPage == 1 && lastPage == 0){
        lastPage = 1;
        parent->updatePageCount(lastPage);

        return;
    }

    for(i=0; i<lenPage; ++i){
        if(data->datatouch->at(i)->isVisible()){
            break;
        }
    }

    if(i < lenPage)
        ++i;

    if(lastPage != i){
        lastPage = i;

        parent->updatePageCount(lastPage);
    }

}
