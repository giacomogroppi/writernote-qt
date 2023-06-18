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

int FileContainer::load_ver_0(WFile &file, size_t size) noexcept
{
    size_t stack = 0;

    const auto versionFileController = VersionFileController::loadVersion(file);

    if (WListFast<Pair>::load(versionFileController, file, _subFiles) < 0) {
        return -1;
    }

    return 0;
}

FileReader FileContainer::getFileReader(const WString &nameFile) const noexcept
{
    const auto it = std::find_if(_subFiles.begin(), _subFiles.end(), [&nameFile](const Pair &pair) {
        return pair.getKey() == nameFile;
    });

    if (it == _subFiles.end())
        return {};

    SharedPtr<WByteArray> tmp = it->getValue();
    return {tmp};
}

auto FileContainer::addFile(FileWriter &&file) -> int
{
    return this->addFile(file.getName(), std::move(file._data));
}

auto FileContainer::addFile(const FileWriter &file) -> int
{
    return this->addFile(file.getName(), file.getData());
}

auto FileContainer::addFile(WString name, WByteArray data) -> int
{
    for (auto &ref: _subFiles) {
        if (ref.getKey() == name) {
            ref.setValue(
                    SharedPtr<WByteArray>(
                            new WByteArray(std::move (data))
                    )
            );
            return 0;
        }
    }

    _subFiles.append(
            Pair (
                    std::move(name),
                    SharedPtr(new WByteArray(std::move(data)))
            )
    );

    return 0;
}
