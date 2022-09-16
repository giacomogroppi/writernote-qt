#pragma once

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
