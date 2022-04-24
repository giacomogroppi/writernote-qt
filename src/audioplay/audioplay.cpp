#include "audioplay.h"
#include "utils/dialog_critic/dialog_critic.h"
#include "testing/memtest.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utils/platform.h"

audioplay::audioplay(QObject *parent) : QObject(parent)
{
    if(disableAudioForDebug){
        qDebug() << "Audio disable for debug";
        return;
    }

    this->player = new QMediaPlayer(this);
    this->parent = (MainWindow *)parent;
    Q_ASSERT(this->parent->objectName() == "MainWindow");

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
    const QString path = (isPlay() || isEndMedia()) ? ":image/images/play.png" : ":image/images/pause-recording.png";

    icon.addFile(path, QSize(), QIcon::Normal, QIcon::Off);

    parent->ui->start_play->setIcon(icon);
}

void audioplay::positionChange(qint64 position)
{
    int duration;

    WDebug(debugAudioPlay, "audioplay::positionChange" << "call");
    if(!isPlay())
        return;

    parent->ui->statusBar->showMessage(tr("%1 second").arg(position/1000));

    parent->ui->audioSlider->blockSignals(true);

    duration = currentDurationMicro();
    if(unlikely(!duration))
        return user_message("The audio seems blank");

    parent->ui->audioSlider->setValue((position*100)/duration);

    parent->ui->audioSlider->blockSignals(false);
    WDebug(debugAudioPlay, "audioplay::positionChange" << this->getPositionSecond() << position);

    parent->_canvas->call_update();
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
    this->parent->_canvas->data->datatouch->triggerNewView(-1, true);
}
