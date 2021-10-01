#include "../mainwindow.h"
#include "ui_mainwindow.h"
#include <QMediaPlayer>

#include "cambioiconariascolto.h"

void MainWindow::cambiostatoplayer(QMediaPlayer::State state){
    contrUi();
    if(state == QMediaPlayer::StoppedState){

        ui->start_rec->setEnabled(false);

        QIcon icon;
        icon.addFile(QString::fromUtf8(":image/images/play.png"), QSize(), QIcon::Normal, QIcon::Off);
        ui->start_play->setIcon(icon);

        ui->statusBar->clearMessage();

        m_canvas->m_pos_ris = -1;
    }

    ui->start_rec->setEnabled(false);
}
