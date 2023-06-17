//
// Created by Giacomo Groppi on 07/06/23.
//

#include "FileContainer.h"
#include "core/WFile.h"
#include "utils/WCommonScript.h"
#include "core/pointer/SharedPtr.h"

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

    static_assert_type(version, int);
    static_assert (sizeof (version) == 4);

    file.read(version);

    switch (version) {
        case 0:
            load_ver_0(file, file.size());
            _isOk = true;
            break;
        default:
            break;
    }

        file.close();
}

bool FileContainer::close()
{
    WFile file(this->_path);

    if (not file.open(WFile::WFileWrite))
        return false;

    if (WListFast<Pair>::save<WFile>(file, this->_subFiles) < 0)
        return false;

    return true;
}

// TODO: Use WFile
int FileContainer::load_ver_0(WFile &file, size_t size) noexcept
{
    size_t stack = 0;

    const auto versionFileController = VersionFileController::loadVersion(file);

    if (WListFast<Pair>::load(versionFileController, file, _subFiles) < 0) {
        // TODO manage error
        W_ASSERT(0);
    }

    // static_assert(0, "To implement");
}
