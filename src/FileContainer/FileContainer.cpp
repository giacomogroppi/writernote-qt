#include "FileContainer.h"
#include "core/WFile.h"
#include "utils/WCommonScript.h"
#include "core/pointer/SharedPtr.h"

#include <utility>

FileContainer::FileContainer(WPath path)
    : _path(std::move(path))
    , _isOk(false)
    , _file(_path)
{
    WFile file (_path);
    size_t offset = 0;
    int version;

    if (!file.open(WFile::ReadOnly)) {
        this->_isOk = false;
        return;
    }

    static_assert_type(version, int);
    static_assert (sizeof (version) == 4);

    file.ReadableAbstract::read(version);

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

    if (not file.open(WFile::Write))
        return false;

    if (WListFast<Pair>::write(file, this->_subFiles) < 0)
        return false;

    return true;
}

int FileContainer::load_ver_0(WFile &file, size_t size) noexcept
{
    size_t stack = 0;

    const auto [result, versionFileController] = VersionFileController::load(file);

    if (result < 0)
        return -1;

    {
        auto [res, data] = WListFast<Pair>::load (versionFileController, file);
        if (res < 0)
            return -1;
        _subFiles = std::move (data);
    }

    return 0;
}

FileReader FileContainer::getFileReader(const WString &nameFile) const noexcept
{
    for (const auto &ref: std::as_const(_subFiles)) {
        if (ref.getKey() == nameFile) {
            SharedPtr<WByteArray> tmp = ref.getValue();
            return  {
                tmp
            };
        }
    }

    return {};
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

auto FileContainer::remove(const WString &path) -> bool
{
    int index = -1;
    for (int i = 0; i < _subFiles.size(); i++) {
        if (_subFiles.at(i).getKey() == path) {
            index = i;
            break;
        }
    }

    if (index == -1)
        return false;

    _subFiles.removeAt(index);
    return true;
}

auto FileContainer::sizeOfFile(const WString &path) const -> size_t
{
    return this->dataOf(path)->size();
}

auto FileContainer::dataOf(const WString &name) const -> SharedPtr<WByteArray>
{
    for (const auto & _subFile : _subFiles) {
        if (_subFile.getKey() == name)
            return _subFile.getValue();
    }
    return nullptr;
}

auto FileContainer::isOk() const -> bool
{
    return this->_isOk;
}
