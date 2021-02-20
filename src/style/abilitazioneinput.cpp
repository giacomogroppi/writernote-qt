#include "abilitazioneinput.h"
#include "ui_mainwindow.h"

#define CHECK !parent->m_currenttitle.m_touch
#define PARENTUI parent->ui

void abilitazioneinput(MainWindow *parent)
{
    parent->m_canvas->setHidden(CHECK);
    parent->ui->toolbarmatita->setHidden(CHECK);

    /* pulsanti matita */
    PARENTUI->toolBarcolore->setHidden(CHECK);
    PARENTUI->toolSheet->setHidden(CHECK);

    PARENTUI->spinBox_fontsize->setHidden(!CHECK);
    PARENTUI->fontComboBox_fonttipo->setHidden(!CHECK);
    PARENTUI->boldbotton->setHidden(!CHECK);
    PARENTUI->inserttablebotton->setHidden(!CHECK);
    PARENTUI->insertimagebotton->setHidden(!CHECK);
    PARENTUI->textEdit->setHidden(!CHECK);
    PARENTUI->listbotton->setHidden(!CHECK);
}
