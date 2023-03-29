#pragma once

#include <QObject>
#include <QMediaPlayer>
#include <QBuffer>
#include "qaudiooutput.h"
#include "utils/WCommonScript.h"
#include "qiodevice.h"
#include "utils/platform.h"

constexpr bool disableAudioForDebug =
#if defined(DEBUGINFO) && defined(linux)
        false;
#else
        false;
#endif

constexpr bool debugAudioPlay = true;

class audioplay : public QObject
{
    Q_OBJECT
public:
    explicit audioplay(QObject *parent);

    bool isPlay() const;
    bool isPause() const;
    bool isStop() const;

    bool isEndMedia() const;

    void setVolume(const int val);

    void setPositionSecond(const qint64 pos);
    void setPositionMicro(const qint64 pos);

    qint64 currentDurationSecond() const;
    qint64 currentDurationMicro() const;

    qint64 getPositionSecond() const;
    qint64 getPositionMicro() const;

    void play();
    void pause();
    void stop();

    void changeIcon();

    void setMedia(QBuffer *array);
    void setMedia(const QString &path);

private slots:
    void positionHasChange(qint64 position);

    void updateStatus(QMediaPlayer::PlaybackState newState);

signals:
    void mediaIsEmpty();
    void errorPlaying();
    void endMedia();
    void stateChange();
    /**
     * @ brief now and max are express in seconds
     * @ ensures
     * @  now >= 0 && now <= max &&
    */
    void positionChange(int now, int max);

private:
    QMediaPlayer *player;
    QAudioOutput *audio_output;
};

inline bool audioplay::isPlay() const
{
    if constexpr(disableAudioForDebug)
        return false;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    return this->player->state() == QMediaPlayer::PlayingState;
#else
    return this->player->playbackState() == QMediaPlayer::PlayingState;
#endif
}

inline bool audioplay::isPause() const
{
    if constexpr(disableAudioForDebug)
            return false;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    return this->player->state() == QMediaPlayer::PausedState;
#else
    return this->player->playbackState() == QMediaPlayer::PausedState;
#endif
}

inline bool audioplay::isStop() const
{
    if constexpr(disableAudioForDebug)
            return true;

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    return this->player->state() == QMediaPlayer::StoppedState;
#else
    return this->player->playbackState() == QMediaPlayer::StoppedState;
#endif
}

inline bool audioplay::isEndMedia() const
{
    if constexpr(disableAudioForDebug)
            return false;

    return this->player->mediaStatus() == QMediaPlayer::EndOfMedia;
}

inline void audioplay::setVolume(const int val)
{
    W_ASSERT(disableAudioForDebug == false);

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    this->player->setVolume(val);
#else
    this->audio_output->setVolume(val);
#endif
}

inline void audioplay::setPositionSecond(const qint64 pos)
{
    W_ASSERT(disableAudioForDebug == false);

    this->setPositionMicro(pos*1000);
}

inline void audioplay::setPositionMicro(const qint64 pos)
{
    player->setPosition(pos);
}

inline qint64 audioplay::currentDurationMicro() const
{
    if(disableAudioForDebug)
        return 0;

    return this->player->duration();
}

inline qint64 audioplay::currentDurationSecond() const
{
    if(disableAudioForDebug)
        return 0;

    return currentDurationMicro() / 1000;
}

inline qint64 audioplay::getPositionSecond() const
{
    if(disableAudioForDebug)
        return 0;

    return this->getPositionMicro() / 1000;
}

inline qint64 audioplay::getPositionMicro() const
{
    if(disableAudioForDebug)
        return 0;

    if(isPlay())
        return this->player->position();
    return -1*1000;
}

inline void audioplay::play()
{
    W_ASSERT(disableAudioForDebug == false);
    W_ASSERT(player->isAvailable());

    this->player->play();

    if (WCommonScript::debug_enable() and !isPlay()){
        qDebug() << "Player not playing: ";
        abort();
    }
}

inline void audioplay::pause()
{
    W_ASSERT(disableAudioForDebug == false);

    this->player->pause();

    W_ASSERT(this->isStop() || this->isPause());
}

inline void audioplay::stop()
{
    W_ASSERT(disableAudioForDebug == false);

    this->player->stop();

    W_ASSERT(this->isStop());
}

inline void audioplay::setMedia(QBuffer *array)
{
    W_ASSERT(array);
    W_ASSERT(array->isReadable());
    W_ASSERT(array->size());
    W_ASSERT(disableAudioForDebug == false);

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    this->player->setMedia(QMediaContent(), array);
#else
    this->player->setSourceDevice(array, QUrl("foo.mp3"));
#endif
    qDebug() << this->player->mediaStatus();
}

inline void audioplay::setMedia(const QString &path)
{
    W_ASSERT(disableAudioForDebug == false);

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    this->player->setMedia(QUrl::fromLocalFile(path));
#else
    this->player->setSource(QUrl::fromLocalFile(path));
#endif
}

