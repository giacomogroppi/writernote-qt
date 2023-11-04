#pragma once

#include "VersionFileController.h"
#include "Writable.h"
#include "Readable.h"
#include "core/WPair.h"

#ifdef USE_QT
# include <QDebug>
#endif // USE_QT

class AudioPosition {
private:
    int _position;

    explicit AudioPosition(int position)
        : _position(position)
    {

    };
public:
    AudioPosition () : _position(-1) {};

    static auto makeInvalid() -> AudioPosition;

    /**
     * \param time Time in seconds
     * */
    static auto make(int time) -> AudioPosition;

    static auto zero() -> AudioPosition;

    auto isValid() const -> bool;

    auto operator<(const AudioPosition& other) const -> bool;
    auto operator>(const AudioPosition& other) const -> bool;
    auto operator<=(const AudioPosition& other) const -> bool;
    auto operator>=(const AudioPosition& other) const -> bool;

    auto operator-(int seconds) const -> AudioPosition;
    auto operator+(int seconds) const -> AudioPosition;

    auto operator==(const AudioPosition& other) const noexcept -> bool = default;

    nd static auto write (WritableAbstract& writable, const AudioPosition& object) -> Error;
    nd static auto load (const VersionFileController& version, ReadableAbstract& readable)
        -> WPair<Error, AudioPosition>;

    friend QDebug operator<<(QDebug d, const AudioPosition &p);

    nd constexpr auto seconds() const noexcept -> unsigned;
};

inline auto AudioPosition::write (WritableAbstract& writable, const AudioPosition& object) -> Error
{
    return writable.write(object._position);
}

inline auto AudioPosition::load (const VersionFileController& version, ReadableAbstract& readable)
    -> WPair<Error, AudioPosition>
{
    if (version.getVersionAudioPosition() != 0)
        return {Error::makeErrVersion(), AudioPosition::makeInvalid()};

    int value;

    if (auto err = readable.read(value))
        return {err, {}};

    return {Error::makeOk(), AudioPosition {value}};
}

inline constexpr auto AudioPosition::seconds() const noexcept -> unsigned
{
    W_ASSERT(_position != -1);
    return this->_position;
}

inline auto AudioPosition::zero() -> AudioPosition
{
    return AudioPosition {0};
}

inline auto AudioPosition::makeInvalid() -> AudioPosition
{
    return AudioPosition {-1};
}

inline auto AudioPosition::make(int time) -> AudioPosition
{
    return AudioPosition {time};
}

inline auto AudioPosition::isValid() const -> bool
{
    return _position >= 0;
}

inline auto AudioPosition::operator<(const AudioPosition& other) const -> bool
{
    return _position < other._position;
}

inline auto AudioPosition::operator>(const AudioPosition& other) const -> bool
{
    return _position > other._position;
}

inline auto AudioPosition::operator<=(const AudioPosition& other) const -> bool
{
    return _position <= other._position;
}

inline auto AudioPosition::operator>=(const AudioPosition& other) const -> bool
{
    return _position >= other._position;
}

inline auto AudioPosition::operator-(int seconds) const -> AudioPosition
{
    return AudioPosition {_position - seconds};
}

inline auto AudioPosition::operator+(int seconds) const -> AudioPosition
{
    return AudioPosition {_position + seconds};
}

#ifdef USE_QT
template <class T>
inline QDebug operator<<(QDebug d, const AudioPosition &p)
{
    d.nospace() << p._position;
    return d.nospace();
}
#endif // USE_QT
