#ifndef EXTRACT_AUDIO_H
#define EXTRACT_AUDIO_H

#include <QString>

namespace extract {
    enum n_extract{
        ok,
        load_file,
        not_record,
        load_audio,
        open_to
    };
};


extract::n_extract extract_audio(const QString &path, const QString &path_to);

#endif // EXTRACT_AUDIO_H
