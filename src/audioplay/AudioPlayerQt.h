#pragma once

#include "Scheduler/WObject.h"
#include "Scheduler/Scheduler.h"
#include "core/WElement.h"
#include "core/ByteArray/WByteArray.h"
#include "core/pointer/SharedPtr.h"
#include "core/pointer/UniquePtr.h"
#include "Scheduler/WObject.h"
#include "AudioData/AudioData.h"

#ifdef USE_QT

#include <QMediaPlayer>
#include <QBuffer>

class AudioPlayer: public WObject
{
private:
    QMediaPlayer _mediaPlayer;
    UniquePtr<QBuffer> _buffer;
    QByteArray _dataArray;
public:
    AudioPlayer ();

    auto setData(const AudioData& audioData) -> void;

    void start();
    void stop();

    [[nodiscard]]
    auto getCurrentTimeSecond() const -> Unsigned;

    [[nodiscard]]
    auto getCurrentTimeMilliseconds() const -> Unsigned;

    [[nodiscard]]
    auto getCurrentDurationSeconds() const -> Unsigned;

    [[nodiscard]]
    auto isPlaying() const -> Bool;

    void setPositionSecond(Unsigned i) noexcept;;

    W_EMITTABLE_0(onAudioStart)

    W_EMITTABLE_0(onAudioStop);
    W_EMITTABLE_1(onTimeChanged, Unsigned, timeInSecond);
    W_EMITTABLE_0(onAudioEnd);
    W_EMITTABLE_0(onDurationChanged);
};

#endif // USE_QT
