#pragma once

#include <iostream>
#include "utils/WCommonScript.h"
#include "VersionFileController.h"

#ifdef USE_QT
# error "Trying using WByteArrayUniversal with Qt"
#endif

class WByteArray
{
private:
    void test() const;
    char *_data;
    unsigned _size;
    unsigned _reserved;

    using CharType = char;
public:
    WByteArray ();
    WByteArray(const char *data, int size = -1);
    WByteArray (WByteArray &&other) noexcept;
    WByteArray (const WByteArray &other) noexcept;
    ~WByteArray();

    std::string toStdString() const;
    const char *constData() const;
    size_t size() const;
    void append(char data);
    void append(const char *data, unsigned int size);
    char at(int i) const;
    void clear() noexcept;
    void insert(const WByteArray &data, unsigned index);
    bool isEmpty() const;

    void reserve(unsigned numberOfChar);

    WByteArray mid(int from, int to) const;

    bool operator==(const WByteArray &other) const;
    WByteArray operator+(const WByteArray &other) const;
    WByteArray &operator+=(const WByteArray &other);
    WByteArray &operator=(const char *data);
    char &operator[](int i);

    static WByteArray fromRawData(const char *data, int size);

    auto capacity() const -> int;

    static auto load  (const VersionFileController& versionController, ReadableAbstract &readable) -> std::pair<int, WByteArray>;
    static auto write (WritableAbstract &writable, const WByteArray &data) -> int;

    class iterator{
    private:
        char *_array;
        unsigned _index;
    public:
        explicit iterator(char *data, unsigned index) : _array(data), _index(index) { };
        explicit iterator(WByteArray &d, int index) : _array(d._data), _index(index) {};

        //T* operator->()         { return array._data[_index]; };
        char &operator*() const    { return _array[_index]; };
        bool operator==(iterator i) const         { return _index == i._index; }
        bool operator!=(iterator i) const         { return _index != i._index; }
        iterator &operator++()                              { _index ++; return *this; }
        iterator operator++(int) { auto copy = *this; ++*this; return copy; }
    };

    class const_iterator{
    private:
        const char* _array;
        unsigned _index;
    public:
        explicit const_iterator(const char *data, unsigned index) : _array(data), _index(index) {  };
        const_iterator(const WByteArray &d, unsigned index) : _array(d._data), _index(index) {};

        const char &operator*() const    { return _array[_index]; };
        bool operator==(const_iterator i) const         { return _index == i._index; }
        bool operator!=(const_iterator i) const         { return _index != i._index; }
        const_iterator &operator++()                              { _index ++; return *this; }
        const_iterator operator++(int) { auto copy = *this; ++*this; return copy; }
    };

    iterator begin() noexcept { test(); return iterator(this->_data, 0); };
    iterator end()   noexcept { test(); return iterator(nullptr, _size);  };

    auto constBegin() const noexcept { test(); return const_iterator(this->_data, 0); }
    auto constEnd()   const noexcept { test(); return const_iterator(nullptr, _size); }
    auto cBegin() const noexcept { test(); return const_iterator(this->_data, 0); }
    auto cEnd()   const noexcept { test(); return const_iterator(nullptr, _size); }
    auto begin() const noexcept { test(); return const_iterator(this->_data, 0); }
    auto end()   const noexcept { test(); return const_iterator(nullptr, _size); }

    friend class iterator;
    friend class const_iterator;

    auto operator=(WByteArray &&other) noexcept -> WByteArray &;
    auto operator=(const WByteArray &other) noexcept -> WByteArray &;
};

inline const char *WByteArray::constData() const
{
    return _data;
}

inline size_t WByteArray::size() const
{
    return _size;
}

inline void WByteArray::append(char data)
{
    this->append(&data, 1);
}

inline std::string WByteArray::toStdString() const
{
    return {_data, _size};
}

inline void WByteArray::append(const char *data, unsigned int size)
{
    W_ASSERT(size >= 0);
    W_ASSERT(data);

    if (_reserved > 0) {
        if (_reserved < size) {
            _data = (char *) realloc(_data, _size + size);
            _reserved = 0;
        } else {
            _reserved -= size;
        }
    } else {
        _data = (char *) realloc(_data, _size + size);
    }

    memcpy(_data + _size, data, size);
    _size += size;
}

inline char WByteArray::at(int i) const
{
    return _data[i];
}

void WByteArray::clear() noexcept
{
    free(_data);
    _data = nullptr;
    _size = 0;
}

inline bool WByteArray::isEmpty() const
{
    return this->_size == 0;
}

inline WByteArray WByteArray::mid(int from, int to) const
{
    W_ASSERT(from >= 0 && to <= _size);
    W_ASSERT(from <= to);

    WByteArray res;

    res.reserve(to - from);
    res.append(_data + from, to - from);

    return res;
}

inline bool WByteArray::operator==(const WByteArray &other) const
{
    if (this->_size != other._size)
        return false;
    return WCommonScript::WMemcpy(_data, other._data, _size) == 0;
}

inline WByteArray WByteArray::operator+(const WByteArray &other) const
{
    WByteArray res;

    res.reserve(this->_size + other._size);
    res.append(_data, _size);
    res.append(other._data, other._size);

    return res;
}

inline WByteArray &WByteArray::operator+=(const WByteArray &other)
{
    this->append(other._data, other._size);
    return *this;
}

inline WByteArray WByteArray::fromRawData(const char *data, int size)
{
    WByteArray res;
    res.append(data, size);
    return res;
}

inline void WByteArray::reserve(unsigned int numberOfChar)
{
    W_ASSERT(numberOfChar > 0);
    this->_data = (char *) realloc(_data, _size + _reserved + numberOfChar);
    _reserved += numberOfChar;
}

inline void WByteArray::test() const
{

}

inline WByteArray::WByteArray(WByteArray &&other) noexcept
    : _data (other._data)
    , _size (other._size)
    , _reserved (other._reserved)
{
    other._data = nullptr;
    other._size = 0;
    other._reserved = 0;
}

inline auto WByteArray::operator=(WByteArray &&other) noexcept -> WByteArray &
{
    this->clear();

    this->_data = other._data;
    this->_size = other._size;
    this->_reserved = other._reserved;

    other._data = nullptr;
    other._size = 0;
    other._reserved = 0;

    return *this;
}

inline auto WByteArray::operator=(const WByteArray &other) noexcept -> WByteArray &
{
    if (this == &other)
        return *this;

    free(_data);
    _data = (char *) malloc(sizeof (CharType) * other._size);

    WCommonScript::WMemcpy(_data, other._data, other.size());

    this->_size = other._size;
    this->_reserved = 0;

    return *this;
}

inline WByteArray::WByteArray(const WByteArray &other) noexcept
    : _data(nullptr)
    , _size(0)
    , _reserved(0)
{
    this->reserve(other.size());
    this->append(other.constData(), other.size());
}

inline auto WByteArray::capacity() const -> int
{
    return this->_reserved;
}
