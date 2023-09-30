#pragma once

#include "utils/WCommonScript.h"
#include "core/ByteArray/WByteArray.h"

class AudioData
{
private:
    WByteArray _data;
public:
    explicit AudioData(WByteArray &&data);

    friend class AudioPlayer;
};

inline AudioData::AudioData(WByteArray &&data)
    : _data(std::move(data))
{

}