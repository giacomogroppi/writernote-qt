#ifndef AUDIOPLAY_H
#define AUDIOPLAY_H

#include <QObject>
#include <QMediaPlayer>
#include <QSlider>
#include <QBuffer>

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

    qint64 durationSecond() const;
    qint64 durationMicro() const;

    qint64 getPositionSecond() const;
    qint64 getPositionMicro() const;

    void play();
    void pause();
    void stop();

    void changeIcon();

    void setMedia(QBuffer *array);
    void setMedia(const QString &path);

private slots:
    void positionChange(qint64 position);
    void updateStatus(QMediaPlayer::State newState);
private:
    QMediaPlayer *player = nullptr;
    class MainWindow *parent;
};

inline bool audioplay::isPlay() const
{
    return this->player->state() == QMediaPlayer::PlayingState;
}

inline bool audioplay::isPause() const
{
    return this->player->state() == QMediaPlayer::PausedState;
}

inline bool audioplay::isStop() const
{
    return this->player->state() == QMediaPlayer::StoppedState;
}

inline bool audioplay::isEndMedia() const
{
    return this->player->mediaStatus() == QMediaPlayer::EndOfMedia;
}

inline void audioplay::setVolume(const int val)
{
    this->player->setVolume(val);
}

inline void audioplay::setPositionSecond(const qint64 pos)
{
    this->setPositionMicro(pos*1000);
}

inline void audioplay::setPositionMicro(const qint64 pos)
{
    player->setPosition(pos);
}

inline qint64 audioplay::durationMicro() const
{
    return this->player->duration();
}

inline qint64 audioplay::durationSecond() const
{
    return durationMicro() / 1000;
}

inline qint64 audioplay::getPositionSecond() const
{
    return this->getPositionMicro() / 1000;
}

inline qint64 audioplay::getPositionMicro() const
{
    if(isPlay())
        return this->player->position();
    return -1;
}

inline void audioplay::play()
{
    this->player->play();
}

inline void audioplay::pause()
{
    this->player->pause();
}

inline void audioplay::stop()
{
    this->player->stop();
}

inline void audioplay::setMedia(QBuffer *array)
{
    this->player->setMedia(QMediaContent(), array);
}

inline void audioplay::setMedia(const QString &path)
{
    this->player->setMedia(QUrl::fromLocalFile(path));
}

#endif // AUDIOPLAY_H
