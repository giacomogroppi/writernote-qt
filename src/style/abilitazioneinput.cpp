#include "abilitazioneinput.h"
#include "ui_mainwindow.h"

#define CHECK !parent->self->currenttitle.m_touch

void abilitazioneinput(MainWindow *parent)
{
    parent->m_canvas->setHidden(CHECK);
    parent->ui->toolbarmatita->setHidden(CHECK);

    /* pulsanti matita */
    parent->ui->toolBarcolore->setHidden(CHECK);
    parent->ui->toolSheet->setHidden(CHECK);

    parent->ui->spinBox_fontsize->setHidden(!CHECK);
    parent->ui->fontComboBox_fonttipo->setHidden(!CHECK);
    parent->ui->boldbotton->setHidden(!CHECK);
    parent->ui->inserttablebotton->setHidden(!CHECK);
    parent->ui->insertimagebotton->setHidden(!CHECK);
    parent->ui->textEdit->setHidden(!CHECK);
    parent->ui->listbotton->setHidden(!CHECK);
}
