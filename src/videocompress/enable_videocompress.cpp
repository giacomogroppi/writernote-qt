#include "enable_videocompress.h"

#include "videocompress_ui.h"
#include "../mainwindow.h"
#include "ui_mainwindow.h"

#include "ui_videocompress_ui.h"

/* funzione responsabile della scrittura delle preferenze */
void enable_videocompress_ui(MainWindow *parent, videocompress_ui *padre)
{
    /* se è già stato importato il video deve scrivere il path nel text edit from */

    int index = parent->self->indice.titolo.indexOf(parent->self->currentTitle);
    if(index != -1){
        QString posizione_video = parent->self->indice.video.at(index);

        if(posizione_video != "")
            padre->ui->plainTextEdit_from->setPlainText(posizione_video);
    }

    padre->ui->codec_combobox->addItem("h264", 0);
    padre->ui->codec_combobox->addItem("h265", 0);

    int i;
    for(i=1; i < 80; i++)
        padre->ui->fps_combobox->addItem(QString::number(i), 0);

    padre->ui->fps_combobox->setCurrentIndex(29);

    padre->ui->pushButton_ok->setEnabled(false);

}
