#include "abilitazioneinput.h"
#include "ui_mainwindow.h"

void abilitazioneinput(MainWindow *parent)
{
    bool check = (parent->self->currenttitle.posizione_binario == "");

    parent->m_canvas->setHidden(check);
    parent->ui->toolbarmatita->setHidden(check);

    /* pulsanti matita */
    parent->ui->toolBarcolore->setHidden(check);


    parent->ui->spinBox_fontsize->setHidden(!check);
    parent->ui->fontComboBox_fonttipo->setHidden(!check);
    parent->ui->boldbotton->setHidden(!check);
    parent->ui->inserttablebotton->setHidden(!check);
    parent->ui->insertimagebotton->setHidden(!check);
    parent->ui->textEdit->setHidden(!check);
    parent->ui->listbotton->setHidden(!check);

}
