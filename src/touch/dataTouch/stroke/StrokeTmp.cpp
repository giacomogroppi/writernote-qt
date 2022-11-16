#include "StrokeTmp.h"

Stroke* Stroke::load(WZipReaderSingle &reader, int version)
{
    W_ASSERT(0);
}

size_t Stroke::getSizeInFile() const
{
    size_t s = 0;
    s += sizeof(this->_metadata);
    s += sizeof(int); // prop

    static_assert(sizeof(type()) == sizeof(int));

    return s;
}

void Stroke::reset()
{
    _flag = UPDATE_BIGGER_DATA | UPDATE_PRESSURE;

    _metadata = {
            .posizione_audio = -1,
            .color = colore_s()
    };
}