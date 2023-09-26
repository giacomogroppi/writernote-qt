#include "audioplay.h"

auto AudioPlayer::setData(const SharedPtr<WByteArray> &audioData) -> void
{
    this->_audioData = audioData;
}
