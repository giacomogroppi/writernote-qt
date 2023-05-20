//
// Created by Giacomo Groppi on 13/05/23.
//

#ifndef WRITERNOTE_WLISTFAST_H
#define WRITERNOTE_WLISTFAST_H

#include <iostream>
#include <algorithm>
#include <utility>
#include "utils/WCommonScript.h"

// do some refactoring
// this list if O(1) in index access
template <class T>
class WListFast {
private:
    void test() const;
    int _size;
    int _reserved;
    T **_data;

    // this function remove the object from the list
    T *takeObject(int i);

public:
    WListFast();
    WListFast(std::initializer_list<T> args);

    const T& at(int i) const;
    void append(const T& element);
    void append(const WListFast<T> &other);
    void remove(int index);
    void remove(int from, int to);
    constexpr bool isEmpty() const;
    constexpr int size() const;
    const T& first() const;
    bool isOrder() const;

    template<typename Func>
    bool isOrder(Func func) const;

    void clear();
    void move(int, int);
    void reserve(int);
    T takeAt(int i);
    const T& last() const;
    int indexOf(const T& value) const;
    int lastIndexOf(const T& object) const;
    void insert(int index, const T& object);
    T takeFirst();

    WListFast<T> mid(int from, int to) const;

    T& operator[](int i);
    bool operator==(const WListFast<T> &other) const;
    WListFast<T>& operator=(const WListFast<T> &other);

    template <typename Func>
    [[nodiscard]] bool anyMatch(Func func) const {
        for (const auto &value: *this) {
            if (func(value))
                return true;
        }
        return false;
    }

    class iterator{
    private:
        T ** _array;
        int _index;
    public:
        iterator(T **data, int index) noexcept : _array(data), _index(index) {; };

        T* operator->()         { return _array[_index]; };
        T &operator*() const    { return *_array[_index]; };
        constexpr bool operator==(iterator i) const         { return _index == i._index; }
        constexpr bool operator!=(iterator i) const         { return _index != i._index; }
        iterator &operator++()                              { _index ++; return *this; }
        iterator operator++(int) { auto copy = *this; ++*this; return copy; }
    };

    class const_iterator{
    private:
        const T ** const _array;
        int _index;
    public:
        const_iterator(const T **data, int index) noexcept : _array(data), _index(index) {};

        const T* operator->() const   { return _array[_index]; };
        const T &operator*() const    { return *_array[_index]; };
        constexpr bool operator==(const_iterator i) const         { return _index == i._index; }
        constexpr bool operator!=(const_iterator i) const         { return _index != i._index; }
        const_iterator &operator++()                              { _index ++; return *this; }
        const_iterator operator++(int) { auto copy = *this; ++*this; return copy; }
    };

    iterator begin() noexcept { return iterator((T **)_data, 0); };
    iterator end()   noexcept { return iterator((T **)_data, size());  };

    const_iterator constBegin() const noexcept { return const_iterator((const T **)_data, 0); }
    const_iterator constEnd()   const noexcept { return const_iterator((const T **)_data, size()); }
    const_iterator cBegin() const noexcept { return const_iterator((const T **)_data, 0); }
    const_iterator cEnd()   const noexcept { return const_iterator((const T **)_data, size()); }
    const_iterator begin() const noexcept { return const_iterator((const T **)_data, 0); }
    const_iterator end()   const noexcept { return const_iterator((const T **)_data, size()); }
};

template<class T>
inline WListFast<T> WListFast<T>::mid(int from, int to) const
{
    W_ASSERT(from >= 0 && to <= size());
    W_ASSERT(from <= to);

    auto &res = WListFast<T>();

    res.reserve(to - from);
    for (int i = from; i < to; i++) {
        res.append(at(i));
    }

    return res;
}

template<class T>
inline int WListFast<T>::indexOf(const T &value) const {
    for (int i = 0; i < size(); ++i) {
        if (at(i) == value)
            return i;
    }

    return -1;
}

template<class T>
inline void WListFast<T>::insert(int index, const T &object)
{
    W_ASSERT(index >= 0 && index <= size());

    if (_reserved > 0) {
        _reserved --;
    } else {
        this->_data = (T **) realloc(_data, sizeof(T*) * (size() + 1));
    }

    for (int i = size(); i > index; i--) {
        _data[i] = _data[i - 1];
    }

    _data[index] = new T(object);

    _size ++;

    test();

    W_ASSERT(at(index) == object);
}

template<class T>
force_inline void WListFast<T>::test() const
{
#ifdef DEBUGINFO
    W_ASSERT(this->_size >= 0);
    W_ASSERT(this->_reserved >= 0);
    if (_size > 0) {
        for (int i = 0; i < size(); i++) {
            for (int j = 0; j < size(); j++) {
                if (i != j) {
                    W_ASSERT(_data[i] != _data[j]);
                }
            }
        }
    } else {
        W_ASSERT(_data == nullptr);
    }
#endif // DEBUGINFO
}

template<class T>
inline void WListFast<T>::move(int from, int to)
{
    W_ASSERT(from >= 0 and from < size());
    W_ASSERT(to >= 0 and to < size());

    if (from == to)
        return;

    auto *r = this->_data[from];
    _data[from] = _data[to];
    _data[to] = r;

    test();
}

template<class T>
inline WListFast<T> &WListFast<T>::operator=(const WListFast<T> &other)
{
    if (this == &other)
        return *this;

    // TODO: optimize [also to use shared data between list]
    for (int i = 0; i < size(); i++) {
        delete _data[i];
    }
    free(_data);

    this->_size = other.size();
    this->_reserved = 0;
    this->_data = (T**) malloc(sizeof (T*) * other.size());

    for (int i = 0; i < size(); i++) {
        _data[i] = new T(other.at(i));
    }

    test();

    return *this;
}

template<class T>
inline bool WListFast<T>::operator==(const WListFast<T> &other) const
{
    if (size() != other.size())
        return false;
    for (int i = 0; i < size(); i++) {
        if (at(i) != other.at(i))
            return false;
    }

    test();

    return true;
}

template<class T>
inline T &WListFast<T>::operator[](int i)
{
    return *this->_data[i];
}

template<class T>
inline T WListFast<T>::takeFirst()
{
    return takeAt(0);
}

template<class T>
inline int WListFast<T>::lastIndexOf(const T &object) const
{
    for (int i = size() - 1; i >= 0; i--) {
        if (at(i) == object)
            return i;
    }

    return -1;
}

template<class T>
inline const T &WListFast<T>::last() const
{
    return at(size() - 1);
}

template <class T>
inline T*WListFast<T>::takeObject(int index)
{
    W_ASSERT(index >= 0 && index < size());
    T* object = this->_data[index];

    for (int i = index; i < size() - 1; i++) {
        this->_data[i] = this->_data[i + 1];
    }

    if (_reserved == 0) {
        this->_data = (T **) realloc(_data, (_size - 1) * sizeof (T *));
    } else {
        _reserved ++;
    }

    _size --;

    test();
    return object;
}

template<class T>
inline T WListFast<T>::takeAt(int index)
{
    W_ASSERT(index >= 0 && index < size());
    T* p = takeObject(index);
    T copy = T(*p);

    delete p;

    test();
    return copy;
}

template<class T>
inline void WListFast<T>::reserve(int reserve)
{
    this->_reserved += reserve;
    test();
}

template<class T>
inline void WListFast<T>::clear()
{
    *this = WListFast<T>();
    test();
}

template<class T>
inline bool WListFast<T>::isOrder() const
{
    for (int i = 0; i < size() - 1; i++) {
        if (at(i) < at(i + 1))
            return false;
    }
    return true;
}

template<class T>
inline const T &WListFast<T>::first() const {
    return *this->_data[0];
}

template<class T>
constexpr inline int WListFast<T>::size() const
{
    return _size;
}

template<class T>
constexpr inline bool WListFast<T>::isEmpty() const
{
    return _size == 0;
}

template<class T>
inline void WListFast<T>::remove(int from, int to)
{
    W_ASSERT(from <= to);
    W_ASSERT(from >= 0 && to < size());

    for (int i = from; i < to; i++) {
        delete this->_data[i];
    }

    for (int i = from; i < size(); i++) {
        this->_data[i] = this->_data[i + to - from];
    }

    if (this->_reserved == 0) {
        this->_data = (T **) realloc(this->_data, (this->_size - (to - from)) * sizeof(T *));
    } else {
        this->_reserved += (to - from);
    }

    this->_size -= (to - from);

    test();
}

template<class T>
inline void WListFast<T>::remove(int index)
{
    delete this->takeObject(index);
    test();
}

template<class T>
inline void WListFast<T>::append(const WListFast<T> &other)
{
    this->reserve(other.size());

    for (const auto &element: std::as_const(other)) {
        this->append(element);
    }

    test();
}

template<class T>
inline void WListFast<T>::append(const T &element)
{
    _size++;

    if (_reserved == 0) {
        this->_data = (T **)realloc(_data, (this->_size) * sizeof (T *));
        this->_data[_size - 1] = new T(element);
    } else {
        this->_data[_size - 1] = new T(element);
        _reserved --;
    }

    test();
}

template<class T>
inline const T &WListFast<T>::at(int i) const
{
    W_ASSERT(i >= 0 && i < this->size());
    return *this->_data[i];
}

template<class T>
inline WListFast<T>::WListFast(std::initializer_list<T> args)
    : _size(0)
    , _data(nullptr)
    , _reserved(0)
{
    this->reserve(args.size());
    for (auto &d: args) {
        this->append(d);
    }

    test();
}

template<class T>
inline WListFast<T>::WListFast()
    : _size(0)
    , _reserved(0)
    , _data(nullptr)
{
    test();
}


#endif //WRITERNOTE_WLISTFAST_H
