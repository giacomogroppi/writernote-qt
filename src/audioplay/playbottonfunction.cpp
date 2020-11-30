#ifndef AUDIOPLAY_PLAY_BOTTON_FUNCTION_CPP
#define AUDIOPLAY_PLAY_BOTTON_FUNCTION_CPP

#include "../mainwindow.h"
#include "ui_mainwindow.h"

#include <QIcon>

/* funzione play in basso a sinistra */
void MainWindow::on_startriascoltobotton_clicked()
{
    QIcon icon;
    if(this->player->state() == QMediaPlayer::PlayingState){
        icon.addFile(QString::fromUtf8("../images/play.png"), QSize(), QIcon::Normal, QIcon::Off);
    }
    else{
        icon.addFile(QString::fromUtf8("../images/pause-recording.png"), QSize(), QIcon::Normal, QIcon::Off);
    }
    this->ui->startriascoltobotton->setIcon(icon);
}


/* funzione in python
  print("playButtonFunction -> start")
        if self.play_: return

        self.pauseButton.setEnabled(True)
        self.timeSlider.setEnabled(True)
        self.volumeSlider.setEnabled(True)
        self.playButton.setEnabled(False)

        self.riascolto_Audio()
        self.player.setPosition(self.position)
        print(self.position)
        print("playButtonFunction -> stop")

 */

#endif //AUDIOPLAY_PLAY_BOTTON_FUNCTION_CPP
