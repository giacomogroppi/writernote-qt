#include "audioplay.h"
#include "utils/dialog_critic/dialog_critic.h"
#include "testing/memtest.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utils/platform.h"
#include "core/core.h"

#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
#include <QAudioOutput>
#include <QMediaFormat>
#endif

audioplay::audioplay(QObject *parent) : QObject(parent)
{
    if(disableAudioForDebug){
        qDebug() << "Audio disable for debug";
        return;
    }

    this->player = new QMediaPlayer(this);
#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
    this->audio_output = new QAudioOutput(this);
    this->player->setAudioOutput(audio_output);
#endif

    QObject::connect(player, &QMediaPlayer::positionChanged, this, &audioplay::positionChange);

#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
    QObject::connect(player, &QMediaPlayer::playbackStateChanged, this, &audioplay::updateStatus);
    QObject::connect(player, &QMediaPlayer::sourceChanged, this, &audioplay::mediachange);
#else
    QObject::connect(player, &QMediaPlayer::stateChanged, this, &audioplay::updateStatus);
#endif

    QObject::connect(player, &QMediaPlayer::durationChanged, [=](qint64 duration){
        qDebug() << "Duration change " << duration;
    });
}

void audioplay::changeIcon()
{
    QIcon icon;
    const QString path = (isPlay() || isEndMedia()) ? ":image/images/play.png" : ":image/images/pause-recording.png";

    icon.addFile(path, QSize(), QIcon::Normal, QIcon::Off);

    core::get_main_window()->ui->start_play->setIcon(icon);
}

void audioplay::positionChange(qint64 position)
{
    int duration;
    MainWindow *parent = core::get_main_window();

    WDebug(debugAudioPlay, "call");
    if(!isPlay())
        return;

    parent->ui->statusBar->showMessage(tr("%1 second").arg(position/1000));

    parent->ui->audioSlider->blockSignals(true);

    duration = currentDurationMicro();
    if(unlikely(!duration))
        return user_message(QApplication::tr("The audio seems blank"));

    parent->ui->audioSlider->setValue((position*100)/duration);

    parent->ui->audioSlider->blockSignals(false);
    WDebug(debugAudioPlay, this->getPositionSecond() << position);

    parent->_canvas->call_update();
}

void audioplay::mediachange(const QUrl &media)
{
    qDebug() << __func__ << "source change" << media << this->player->mediaStatus();
}

void audioplay::updateStatus(
# if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QMediaPlayer::State newState
# else
    QMediaPlayer::PlaybackState newState
# endif
)
{
    MainWindow *parent = core::get_main_window();
    if(newState == QMediaPlayer::StoppedState){

        parent->ui->start_rec->setEnabled(false);

        QIcon icon;
        icon.addFile(QString::fromUtf8(":image/images/play.png"), QSize(), QIcon::Normal, QIcon::Off);
        parent->ui->start_play->setIcon(icon);

        parent->ui->statusBar->clearMessage();
    }

    parent->ui->start_rec->setEnabled(false);
    parent->contrUi();
    parent->_canvas->getDoc()->datatouch->triggerNewView(-1, true);
}
