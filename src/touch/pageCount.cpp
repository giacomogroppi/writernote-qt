#include "tabletcanvas.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "log/log_ui/log_ui.h"
#include "utils/WCommonScript.h"
#include "preview_page_widget/preview_page_widget.h"

static int __page = -2;
static int __max = 0;
void MainWindow::updatePageCount(int pageCount)
{
    QString text;
    cint lenPage = _canvas->getDoc()->lengthPage();

    ui->page->setHidden(pageCount == -1);

    text = qstr("Page: %1 of %2").arg(pageCount).arg(lenPage);

    this->_preview_widget->pageMove();

    if(__page == pageCount and lenPage == __max)
        return;

    __max = lenPage;
    __page = pageCount;

    this->ui->page->setText(text);
}

void TabletCanvas::updatePageCount()
{
    auto *main = core::get_main_window();
    const int index = getDoc()->getFirstPageVisible() + 1;
    main->_preview_widget->pageMove();
    main->updatePageCount(index);
}
