#pragma once

#include "Scheduler/WObject.h"
#include "Scheduler/Scheduler.h"
#include "core/WElement.h"
#include "core/ByteArray/WByteArray.h"
#include "core/pointer/SharedPtr.h"

class AudioPlayer
{
private:
    SharedPtr<WByteArray> _audioData;
public:
    AudioPlayer ();

    auto setData(const SharedPtr<WByteArray>& audioData) -> void;

    auto getCurrentTime() const -> Unsigned;
    auto isPlaying() const -> Bool;
};