#include "controlluibutton.h"
#include "../mainwindow.h"
#include "ui_mainwindow.h"

void ControllUiButton::update()
{
    int i, len;
    const bool tablet = this->mode == Mode::Tablet;
    //const bool tablet = true;

    parent->ui->mainbar->setHidden(tablet);
    parent->ui->simpleactionbar->setVisible(!tablet);
    parent->ui->audiobar->setVisible(!tablet);

    parent->ui->toolbarmatita->setVisible(!tablet);
    parent->ui->toolBarcolore->setVisible(!tablet);
    parent->ui->toolSheet->setVisible(!tablet);

    parent->ui->pushButton_back->setHidden(!tablet);
}
