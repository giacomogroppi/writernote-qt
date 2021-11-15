#include "audioplay.h"
#include "../utils/dialog_critic/dialog_critic.h"

#include "../mainwindow.h"
#include "ui_mainwindow.h"

audioplay::audioplay(QObject *parent) : QObject(parent)
{
    this->player = new QMediaPlayer(this);
    this->parent = (MainWindow *)parent;

    player = new QMediaPlayer(this);
    connect(player, &QMediaPlayer::positionChanged, this, &audioplay::positionChange);
    connect(player, &QMediaPlayer::stateChanged, this, &audioplay::updateStatus);

    connect(player, &QMediaPlayer::durationChanged, [=](qint64 duration){
        qDebug() << "Duration change " << duration;
    });

    connect(player, QOverload<QMediaPlayer::Error>::of(&QMediaPlayer::error),
        [=](QMediaPlayer::Error error){
        Q_UNUSED(error)
        user_message("We had an internal error with this error code: " + player->errorString());
    });

}

void audioplay::changeIcon()
{
    QIcon icon;
    if(isPlay() || isEndMedia())
        icon.addFile(QString::fromUtf8(":image/images/play.png"), QSize(), QIcon::Normal, QIcon::Off);
    else
        icon.addFile(QString::fromUtf8(":image/images/pause-recording.png"), QSize(), QIcon::Normal, QIcon::Off);

    parent->ui->start_play->setIcon(icon);
}

void audioplay::positionChange(qint64 position)
{
    if(!isPlay()) return;

    parent->ui->statusBar->showMessage(tr("%1 second").arg(position/1000));

    parent->ui->audioSlider->blockSignals(true);

    int duration = durationMicro();
    if(!duration)
        return user_message("The audio seems blank");

    parent->ui->audioSlider->setValue((position*100)/duration);

    parent->ui->audioSlider->blockSignals(false);
}

void audioplay::updateStatus(QMediaPlayer::State newState)
{
    if(newState == QMediaPlayer::StoppedState){

        parent->ui->start_rec->setEnabled(false);

        QIcon icon;
        icon.addFile(QString::fromUtf8(":image/images/play.png"), QSize(), QIcon::Normal, QIcon::Off);
        parent->ui->start_play->setIcon(icon);

        parent->ui->statusBar->clearMessage();
    }

    parent->ui->start_rec->setEnabled(false);
    parent->contrUi();
}
