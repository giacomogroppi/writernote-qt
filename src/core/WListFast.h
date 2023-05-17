//
// Created by Giacomo Groppi on 13/05/23.
//

#ifndef WRITERNOTE_WLISTFAST_H
#define WRITERNOTE_WLISTFAST_H

#include <iostream>
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
    int lastIndexOf(const T& object) const;
    void insert(int index, const T& object);
    T takeFirst();

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
        T *array;
        int index;
    public:
        explicit iterator(T *data) : array(data), index(0) {; };

        T* operator->()         { return array[index]; };
        T &operator*() const    { return array[index]; };
        constexpr bool operator==(iterator i) const         { return index == i.index; }
        constexpr bool operator!=(iterator i) const         { return index != i.index; }
        iterator &operator++()                              { index ++; return *this; }
        iterator operator++(int) { auto copy = *this; ++*this; return copy; }
    };

    class const_iterator{
    private:
        const T *array;
        int index;
    public:
        explicit const_iterator(const T *data) : array(data), index(0) {  };

        const T* operator->() const   { return array[index]; };
        const T &operator*() const    { return array[index]; };
        constexpr bool operator==(const_iterator i) const         { return index == i.index; }
        constexpr bool operator!=(const_iterator i) const         { return index != i.index; }
        const_iterator &operator++()                              { index ++; return *this; }
        const_iterator operator++(int) { auto copy = *this; ++*this; return copy; }
    };

    // TODO: IMPORTANT
    iterator begin() noexcept { test(); return iterator(nullptr); };
    iterator end()   noexcept { test(); return iterator(nullptr);  };

    const_iterator constBegin() const noexcept { test(); return const_iterator(nullptr); }
    const_iterator constEnd()   const noexcept { test(); return const_iterator(nullptr); }
    const_iterator cBegin() const noexcept { test(); return const_iterator(nullptr); }
    const_iterator cEnd()   const noexcept { test(); return const_iterator(nullptr); }
    const_iterator begin() const noexcept { test(); return const_iterator(nullptr); }
    const_iterator end()   const noexcept { test(); return const_iterator(nullptr); }

    constexpr int indexOf(const int i) const noexcept;
};

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

    return *this;
}

template<class T>
bool WListFast<T>::operator==(const WListFast<T> &other) const
{
    if (size() != other.size())
        return false;
    for (int i = 0; i < size(); i++) {
        if (at(i) != other.at(i))
            return false;
    }

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

    return object;
}

template<class T>
inline T WListFast<T>::takeAt(int index)
{
    W_ASSERT(index >= 0 && index < size());
    T* p = takeObject(index);
    T copy = T(*p);

    delete p;

    return copy;
}

template<class T>
void WListFast<T>::reserve(int reserve)
{
    this->_reserved += reserve;
}

template<class T>
void WListFast<T>::clear()
{
    *this = WListFast<T>();
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
const T &WListFast<T>::first() const {
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
}

template<class T>
inline void WListFast<T>::remove(int index)
{
    delete this->takeObject(index);
}

template<class T>
inline void WListFast<T>::append(const WListFast<T> &other)
{
    this->reserve(other.size());

    for (const auto &element: std::as_const(other)) {
        this->append(element);
    }
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
}

template<class T>
inline WListFast<T>::WListFast()
    : _size(0)
    , _data(nullptr)
    , _reserved(0)
{
}


#endif //WRITERNOTE_WLISTFAST_H
