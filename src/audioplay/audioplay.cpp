#include "audioplay.h"
#include "testing/memtest.h"
#include "utils/platform.h"
#include "core/core.h"

#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
#include <QAudioOutput>
#include <QMediaFormat>
#endif

audioplay::audioplay(QObject *parent)
    : QObject(parent)
{
    if (disableAudioForDebug) {
        qDebug() << "Audio disable for debug";
        return;
    }

    this->player = new QMediaPlayer(this);
    this->audio_output = new QAudioOutput(this);
    this->player->setAudioOutput(audio_output);

    QObject::connect(player, &QMediaPlayer::positionChanged, this, &audioplay::positionHasChange);
    QObject::connect(player, &QMediaPlayer::playbackStateChanged, this, &audioplay::updateStatus);
}

void audioplay::positionHasChange(qint64 position)
{
    int duration;

    WDebug(debugAudioPlay, "call");
    if (!isPlay())
        return;

    duration = currentDurationMicro();
    if (un(!duration)) {
        emit mediaIsEmpty();
        return;
    }

    WDebug(debugAudioPlay, this->getPositionSecond() << position);
    emit positionChange(this->getPositionSecond(), this->currentDurationSecond());
}

void audioplay::updateStatus(QMediaPlayer::PlaybackState newState)
{
    emit stateChange();
}
