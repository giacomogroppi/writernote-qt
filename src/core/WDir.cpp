#include <sys/stat.h>
#include <filesystem>
#include "WDir.h"
#include "core/ByteArray/WByteArray.h"
#include "core/String/WString.h"

int WDir::removeDir(const WByteArray &path)
{
    const auto res = std::filesystem::remove_all(path.constData());

    if (res < 1)
        return -1;

    return 0;
}

int WDir::createDir(const WByteArray &path)
{
    if (std::filesystem::create_directories(path.constData()))
        return 0;
    return -1;
}

bool WDir::exists(const WByteArray &path)
{
    return std::filesystem::is_directory(path.toStdString());
}
