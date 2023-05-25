#ifndef WRITERNOTE_WBYTEARRAY_H
#define WRITERNOTE_WBYTEARRAY_H

#include <iostream>
#include "utils/WCommonScript.h"

/**
 * TODO: implement a reserved memory
 * */

class WByteArray {
private:
    void test() const;
    char *_data;
    unsigned _size;
    unsigned _reserved;
public:
    WByteArray ();
    WByteArray(const char *data, int size = -1);
    ~WByteArray();

    std::string toStdString() const;
    const char *constData() const;
    size_t size() const;
    void append(char data);
    void append(const char *data, unsigned size);
    char at(int i) const;
    void clear() noexcept;

    void reserve(unsigned numberOfChar);

    WByteArray mid(int from, int to) const;

    bool operator==(const WByteArray &other) const;
    WByteArray operator+(const WByteArray &other) const;
    WByteArray &operator+=(const WByteArray &other);
    WByteArray &operator=(const char *data);

    static WByteArray fromRawData(const char *data, int size);

    class iterator{
    private:
        char *_array;
        unsigned _index;
    public:
        explicit iterator(char *data, unsigned index) : _array(data), _index(index) {; };

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

        const char &operator*() const    { return _array[_index]; };
        bool operator==(const_iterator i) const         { return _index == i._index; }
        bool operator!=(const_iterator i) const         { return _index != i._index; }
        const_iterator &operator++()                              { _index ++; return *this; }
        const_iterator operator++(int) { auto copy = *this; ++*this; return copy; }
    };

    iterator begin() noexcept { test(); return iterator(this->_data, 0); };
    iterator end()   noexcept { test(); return iterator(nullptr, _size);  };

    const_iterator constBegin() const noexcept { test(); return const_iterator(this->_data, 0); }
    const_iterator constEnd()   const noexcept { test(); return const_iterator(nullptr, _size); }
    const_iterator cBegin() const noexcept { test(); return const_iterator(this->_data, 0); }
    const_iterator cEnd()   const noexcept { test(); return const_iterator(nullptr, _size); }
    const_iterator begin() const noexcept { test(); return const_iterator(this->_data, 0); }
    const_iterator end()   const noexcept { test(); return const_iterator(nullptr, _size); }
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

inline void WByteArray::append(const char *data, unsigned size)
{
    W_ASSERT(size >= 0);
    W_ASSERT(data);

    if (_reserved > 0) {
        if (_reserved - size < 0) {
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
    return memcmp(_data, other._data, _size) == 0;
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

#endif //WRITERNOTE_WBYTEARRAY_H
