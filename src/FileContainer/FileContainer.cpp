//
// Created by Giacomo Groppi on 07/06/23.
//

#include "FileContainer.h"
#include "core/WFile.h"
#include "utils/WCommonScript.h"

#include <utility>

FileContainer::FileContainer(WString path)
    : _path(std::move(path))
    , _isOk(false)
{
    WFile file (_path);
    size_t offset = 0;
    int version;

    if (!file.open(WFile::WFileReadOnly)) {
        this->_isOk = false;
        return;
    }

    char d [file.size()];
    file.read(d, file.size());

    WCommonScript::WMemcpy(d, &version, sizeof (version));

    switch (version) {
        case 0:
            load_ver_0(d + sizeof (version), file.size());
            _isOk = true;
            break;
        default:
            break;
    }
}

void FileContainer::load_ver_0(const void *raw, size_t size) noexcept
{
    static_assert(0, "To implement");
}
