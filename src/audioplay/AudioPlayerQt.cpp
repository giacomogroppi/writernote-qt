#include "AudioPlayerQt.h"

#include <memory>

#ifdef USE_QT

AudioPlayer::AudioPlayer(WObject *parent)
    : WObject(parent)
    , _mediaPlayer(nullptr)
    , _buffer(nullptr)
    , _dataArray()
{
    QObject::connect(&_mediaPlayer, &QMediaPlayer::mediaStatusChanged, [this] (QMediaPlayer::MediaStatus status) {
         if (status == QMediaPlayer::MediaStatus::EndOfMedia)
             W_EMIT_0(onAudioEnd);
    });

    QObject::connect(&_mediaPlayer, &QMediaPlayer::positionChanged, [this] (qint64 position) {
        W_ASSERT(position >= 0 and position < UINT_MAX);

        const auto castedPosition = static_cast<unsigned> (position);
        W_EMIT_1(onTimeChanged, Unsigned(castedPosition));
    });
}

auto AudioPlayer::setData(const AudioData &audioData) -> void
{
    this->_dataArray = audioData._data;

    _mediaPlayer.setSourceDevice(nullptr);

    this->_buffer = std::make_unique<QBuffer>(&_dataArray, nullptr);

    _mediaPlayer.setSourceDevice(_buffer.get());
}

void AudioPlayer::start()
{
    this->_mediaPlayer.play();
    W_EMIT_0(onAudioStart);
}

void AudioPlayer::stop()
{
    _mediaPlayer.stop();
    W_EMIT_0(onAudioStop);
}

auto AudioPlayer::isPlaying() const -> Bool
{
    return Bool(this->_mediaPlayer.isPlaying());
}

auto AudioPlayer::getCurrentTimeSecond() const -> Unsigned
{
    const unsigned currentSecond = this->_mediaPlayer.position() / 1000u;
    return Unsigned(currentSecond);
}

auto AudioPlayer::getCurrentTimeMilliseconds() const -> Unsigned
{
    return getCurrentTimeSecond() * Unsigned(1000u);
}

auto AudioPlayer::getCurrentDurationSeconds() const -> Unsigned
{
    const auto current = static_cast<unsigned>(this->_mediaPlayer.duration());

    W_ASSERT(_mediaPlayer.duration() >= 0 and _mediaPlayer.duration() <= UINT32_MAX);

    return Unsigned (current / 1000u);
}

void AudioPlayer::setPositionSecond(Unsigned i) noexcept
{
    this->_mediaPlayer.setPosition(i * 1000u);
}

#endif // USE_QT