#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPrintDialog>
#include <QDialog>
#include <QPrinter>
#include "utils/dialog_critic/dialog_critic.h"

void MainWindow::on_actionPrint_triggered()
{    
    /* TODO -> export a pdf and after share for mobile*/
#ifdef ANDROID_WRITERNOTE
#elif IOS_WRITERNOTE
#else
    QPrinter printer;
    QPrintDialog dialogprint(&printer);

    const Document *doc = m_canvas->data;

    const double size_orizzontale = doc->datatouch->biggerx();
    const double delta = (double)printer.width() / (double)size_orizzontale;

    DataPaint dataPaint = {
        .withPdf = true,
        .IsExportingPdf = true,
        .m = delta,
        .parent = nullptr,
        .m_pixmap = nullptr,
        DATAPAINT_DEFINEREST
    };

    if( dialogprint.exec() != QDialog::Accepted )
        return;

    QPainter painter;
    if (! painter.begin(&printer)) { // failed to open file
        user_message("Writernote had an internal problem");
    }
    TabletCanvas::load(painter, doc, dataPaint);

    painter.drawText(10, 10, "Test 2");
    painter.end();
#endif
}
