#include "../mainwindow.h"


void MainWindow::on_actionlastStyle_triggered()
{
    this->m_sheet->setHidden(false);
    auto rect = this->cursor().pos();
    this->m_sheet->move(rect);
}
