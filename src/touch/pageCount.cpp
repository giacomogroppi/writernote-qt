#include "tabletcanvas.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "log/log_ui/log_ui.h"
#include "utils/common_script.h"
#include "preview_page_widget/preview_page_widget.h"

static int __page = -2;
static int __max = 0;
void MainWindow::updatePageCount(int pageCount)
{
    QString text;
    cint lenPage = _canvas->data->datatouch->lengthPage();

    ui->page->setHidden(pageCount == -1);

    text = qstr("Page: %1 of %2").arg(pageCount).arg(lenPage);

    this->_preview_widget->pageMove();

    if(__page == pageCount && lenPage == __max)
        return;

    __max = lenPage;
    __page = pageCount;

    this->ui->page->setText(text);
}

void TabletCanvas::updatePageCount()
{
    const int index = data->datatouch->getFirstPageVisible() + 1;
    _parent->updatePageCount(index);
}
