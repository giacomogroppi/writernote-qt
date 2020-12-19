#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCloseEvent>
#include <QMessageBox>

#include "datawrite/qfilechoose.h"
#include "datawrite/savefile.h"
#include "dialog_critic.h"

#include "currenttitle/currenttitle_class.h"

#include "datawrite/xmlstruct.h"
#include "currenttitle/checksimilecopybook.h"

void MainWindow::closeEvent (QCloseEvent *event)
{
    if(!this->self->indice.titolo.length())
        return event->accept();

    /* apre il file in file e lo carica nell'oggetto, e lo confronta */
    currenttitle_class *tempcopybook = new currenttitle_class;
    indice_class *tempindice = new indice_class;

    xmlstruct *temp_lettura = new xmlstruct(&this->self->path, tempindice, tempcopybook);
    temp_lettura->loadindice();
    temp_lettura->loadfile((this->self->currentTitle + ".xml").toUtf8().constData());

    QString filep = this->self->currenttitle.testi;
    this->self->currenttitle.testi = this->ui->textEdit->toHtml();

    bool check1 = checksimilecopybook(tempcopybook, &this->self->currenttitle);
    if(check1)
        qDebug() << "Il file va bene";

    check1 = check1 && checksimileindice(&this->self->indice, tempindice);

    /* se è uguale sia il copybook che l'indice accetta */
    if(check1)
        return event->accept();

    this->self->currenttitle.testi = filep;

    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "writernote",
                                                                tr("Do you want to save\n"),
                                                                QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::Yes);

    if (resBtn == QMessageBox::Yes) {

        if(this->self->path == "")
        {   qfilechoose file(this);
            if(!file.filechoose())
                return;
        }

        savefile save_(this, &this->self->currenttitle, &this->self->currentTitle);

        bool check = save_.savefile_check_indice();

        if(this->self->currentTitle != "")
            check = check && save_.savefile_check_file(this->self->indice.titolo.indexOf(this->self->currentTitle));


        if(check)
            return event->accept();

        else{
            dialog_critic((QString)"We had a problem saving the file, please retry");
            return event->ignore();
        }

    } else if (resBtn == QMessageBox::No)
        return event->accept();

    else
        return event->ignore();

    delete tempcopybook;
    delete tempindice;
    delete temp_lettura;
}
