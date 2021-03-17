#include "../mainwindow.h"


void MainWindow::on_actionlastStyle_triggered()
{
    this->m_sheet->show();

    QPoint hostRect = this->cursor().pos();
    m_sheet->move(hostRect);

}
