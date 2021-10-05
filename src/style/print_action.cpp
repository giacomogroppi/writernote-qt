#include "../mainwindow.h"
#include "ui_mainwindow.h"
#include <QPrintDialog>
#include <QDialog>
#include <QPrinter>

void MainWindow::on_actionPrint_triggered()
{
#ifdef ANDROID_WRITERNOTE
#elif IOS_WRITERNOTE
#else
    QPrinter printer;
    QPrintDialog dialogprint(&printer);
    QColor color;
    QPen pen;
    QBrush m_brush;
    TabletCanvas::Point point;


    const double size_orizzontale = m_currenttitle->datatouch->biggerx();
    const double size_verticale = printer.height();
    const double delta = (double)printer.width() / (double)size_orizzontale;

    if( dialogprint.exec() != QDialog::Accepted )
        return;

    QPainter painter;
    if (! painter.begin(&printer)) { // failed to open file
        user_message("Writernote had an internal problem");
    }

    this->m_canvas->load(painter, this->m_currenttitle, color,
                         pen, m_brush, point, 0, nullptr, true, delta, size_orizzontale,
                         size_verticale/delta, nullptr, nullptr);

    painter.drawText(10, 10, "Test 2");
    painter.end();
    /* TODO -> export a pdf in  */
#endif
}
