#pragma once

#include <filesystem>
#include "core/String/WString.h"

class WPath
{
private:
    std::filesystem::path _path;

public:
    WPath () = default;

    WPath(WPath&& path) noexcept = default;
    WPath(const WPath& path) noexcept = default;

    WPath (std::filesystem::path path) noexcept;
    explicit WPath (const char *path) noexcept;
    explicit WPath (WString path) noexcept;

    auto operator=(const std::filesystem::path& path) noexcept -> WPath&;
    auto operator=(std::filesystem::path&& path) noexcept -> WPath&;

    auto operator=(const WPath& path) noexcept -> WPath& = default;
    auto operator=(WPath&& path) noexcept -> WPath& = default;

    auto operator/ (const std::filesystem::path& other) const -> WPath;
    auto operator/ (const WPath& path) const -> WPath;
    auto operator/ (const WString& nameFile) const -> WPath;
    auto operator/ (const WByteArray& nameFile) const -> WPath;
    auto operator/ (const char *string) const -> WPath;

    auto operator/= (const std::filesystem::path& other) -> WPath&;
    auto operator/= (const WPath& path) -> WPath&;
    auto operator/= (const WString& nameFile) -> WPath&;

    auto operator==(const WPath& other) const -> bool;
    auto operator!=(const WPath& other) const -> bool;

    [[nodiscard]]
    auto toString() const -> WString;

    [[nodiscard]]
    auto getNameWithoutExtension() const -> WByteArray;

    [[nodiscard]]
    auto getNameWithExtension() const -> WByteArray;

    [[nodiscard]]
    auto getExtension() const -> WByteArray;

    [[nodiscard]]
    auto hasExtension() const -> bool;

    operator std::filesystem::path () const;

#ifdef USE_QT
    friend QDebug operator<<(QDebug d, const WPath& p);
#else
    friend std::ostream& operator<<(std::ostream& os, const WPath& dt);
#endif // USE_QT
};

#ifdef USE_QT
inline Q_CORE_EXPORT QDebug operator<<(QDebug d, const WPath &p)
{
    d.nospace() << "WPath(";

    d.space() << p._path.string();

    return d.space() << " )";
}
#endif // USE_QT


inline WPath::WPath (std::filesystem::path path) noexcept
    : _path(std::move(path))
{

}

inline WPath::WPath (const char *path) noexcept
    : _path(path)
{

}

inline WPath::WPath (WString path) noexcept
    : _path(path.toStdString())
{

}

inline auto WPath::operator=(const std::filesystem::path& path) noexcept -> WPath&
{
    _path = path;
    return *this;
}

inline auto WPath::operator=(std::filesystem::path&& path) noexcept -> WPath&
{
    _path = std::move(path);
    return *this;
}

inline auto WPath::operator/ (const std::filesystem::path& other) const -> WPath
{
    return _path / other;
}

inline auto WPath::operator/ (const WPath& path) const -> WPath
{
    return _path / path._path;
}

inline auto WPath::operator/ (const WString& nameFile) const -> WPath
{
    return _path / nameFile.toStdString();
}

inline auto WPath::operator/ (const WByteArray& nameFile) const -> WPath
{
    return _path / nameFile.toStdString();
}

inline auto WPath::operator/ (const char *string) const -> WPath
{
    return _path / string;
}

inline auto WPath::operator/= (const std::filesystem::path& other) -> WPath&
{
    _path /= other;
    return *this;
}

inline auto WPath::operator/= (const WPath& path) -> WPath&
{
    _path /= path._path;
    return *this;
}

inline auto WPath::operator/= (const WString& nameFile) -> WPath&
{
    _path /= nameFile.toStdString();
    return *this;
}

inline auto WPath::operator==(const WPath& other) const -> bool
{
    return _path == other._path;
}

inline auto WPath::operator!=(const WPath& other) const -> bool
{
    return !(*this == other);
}

inline auto WPath::toString() const -> WString
{
    return _path.string().c_str();
}

inline auto WPath::getNameWithoutExtension() const -> WByteArray
{
    return _path.stem().string();
}

inline auto WPath::getNameWithExtension() const -> WByteArray
{
    return _path.filename().string();
}

inline auto WPath::getExtension() const -> WByteArray
{
    return _path.extension().string();
}

inline auto WPath::hasExtension() const -> bool
{
    const auto fileWithExtension = this->toString();
    return fileWithExtension.contains({'.'});
}

inline WPath::operator std::filesystem::path () const
{
    return _path;
}

#ifndef USE_QT
inline std::ostream& operator<<(std::ostream& os, const WPath& dt)
{
    os << dt._path;
    return os;
}
#endif // USE_QT
