#ifndef RIASCOLTO_AUDIO_PROGRESSIVO_CPP
#define RIASCOLTO_AUDIO_PROGRESSIVO_CPP

#include "../mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
#include "../self_class.h"

/* funzione che gestiste il riascolto dell'audio */
void MainWindow::riascoltoaudioprogressivo(qint64 position){
    if(this->player->state() != QMediaPlayer::PlayingState)
        return;

/*#ifdef STAMPA
    qDebug() << "position" << position;
#endif*/

    int position_inlist = this->self->currenttitle.posizione_iniz.indexOf(position/1000);

    if(position_inlist == -1) return;



    QString testoGrassetto = "<!DOCTYPE html><html><body><b>" + this->self->currenttitle.testinohtml[position_inlist] + "</b>";
    testoGrassetto += self->currenttitle.testinohtml.last().mid(position_inlist, -1) + "</body></html>";

    this->ui->textEdit->setHtml(testoGrassetto);

    this->ui->audioSlider->blockSignals(true);

    qDebug() << "tempo -> " << this->player->duration();

    this->ui->audioSlider->setValue((position*100)/this->player->duration());

    this->ui->audioSlider->blockSignals(false);


    this->ui->statusBar->showMessage(tr("%1 second").arg(position / 1000));
}

#endif

/*

self.currentTime = int(position/1000)
        print("Audio time: {}".format(self.currentTime))

        if self.play_:
            try:
                position = self.currentTitleJSON['posizione_iniz'].index(str(self.currentTime))
                print(self.currentTitleJSON['posizione_iniz'][position])
            except:
                ''' in caso la riga sia stata eliminata da data.py '''
                return False

            ''' next data structure '''
            try:
                lung = len(self.currentTitleJSON['testinohtml'][position])
                testoGrassetto = '<!DOCTYPE html><html><body><b>' + self.currentTitleJSON['testinohtml'][position] + '</b>'
                testoGrassetto += self.currentTitleJSON['testinohtml'][-1][lung:] + '</body></html>'
                print(position)
            except IndexError:
                print("\nindexerror")
                pass

            self.editor.setHtml(testoGrassetto)


*/
