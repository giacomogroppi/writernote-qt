//
// Created by Giacomo Groppi on 28/04/23.
//

#ifndef WRITERNOTE_WVECTOR_H
#define WRITERNOTE_WVECTOR_H

#include <iostream>
#include "utils/WCommonScript.h"
#include "testing/memtest.h"
#include "WAbstractList.h"

/*

template <class T>
class WVectorIterator {
private:
    T *array;
    int index;
public:
    explicit WVectorIterator(T *data) : array(data), index(0) {; };

    T* operator->()         { return array[index]; };
    T &operator*() const    { return array[index]; };
    constexpr bool operator==(WVectorIterator i) const         { return index == i.index; }
    constexpr bool operator!=(WVectorIterator i) const         { return index != i.index; }
    WVectorIterator &operator++()                              { index ++; return *this; }
    WVectorIterator operator++(int) { auto copy = *this; ++*this; return copy; }
};

template <class T>
class WVectorConstIterator {
private:
    const T *array;
    int index;
public:
    explicit WVectorConstIterator(const T *data) : array(data), index(0) {  };

    const T* operator->() const   { return array[index]; };
    const T &operator*() const    { return array[index]; };
    constexpr bool operator==(WVectorConstIterator i) const         { return index == i.index; }
    constexpr bool operator!=(WVectorConstIterator i) const         { return index != i.index; }
    WVectorConstIterator &operator++()                              { index ++; return *this; }
    WVectorConstIterator operator++(int) { auto copy = *this; ++*this; return copy; }
};
*/

template <class T>
class WVector
{
private:
    /*
    T *_data;
    size_t _size;
    size_t _reserve;

    static constexpr const auto useReserve = false;
    */

    std::vector<T> _data;
    static constexpr auto numberOfAllocation = 64;

    void test() const;
public:
    WVector();
    WVector(const WVector<T> &other) noexcept;
    WVector(WVector<T> &&other) noexcept;
    ~WVector();

    using const_iterator = std::vector<T>::const_iterator;
    using iterator = std::vector<T>::iterator;

    void append(const WVector<T> &other);

    void append(const T &item);
    void append(T &&item);

    const T& get(int i) const;
    int size() const;
    void removeAt(int i);
    void move(int from, int to);
    void clear();
    const T& at(int i) const;
    void reserve(int numberOfElement);
    T takeAt(int i);
    const T& last() const;
    const T& first() const;
    T& operator[](int index);
    bool isEmpty() const;

    void insert(int index, T &&data);
    void insert(int index, const T& data);

    T takeFirst();
    const T* constData() const;
    int indexOf(const T &object);
    void remove(const T& object);
    WVector<T> &operator=(const WVector<T> &other);
    WVector<T> &operator=(WVector &&other) noexcept;
    bool operator==(const WVector<T> &other) const;
    bool operator!=(const WVector<T> &other) const;

    iterator begin()    noexcept { return _data.begin(); }
    iterator end()      noexcept { return _data.end(); }

    const_iterator constBegin()   const noexcept { return _data.cbegin(); }
    const_iterator constEnd()     const noexcept { return _data.cend(); }
    const_iterator begin()        const noexcept { return _data.cbegin(); }
    const_iterator end()          const noexcept { return _data.cend(); }
    /*
    iterator begin() noexcept { test(); return iterator(this->_data); };
    iterator end()   noexcept { test(); return iterator(nullptr);  };

    const_iterator constBegin() const noexcept { test(); return const_iterator(this->_data); }
    const_iterator constEnd()   const noexcept { test(); return const_iterator(nullptr); }
    const_iterator cBegin() const noexcept { test(); return const_iterator(this->_data); }
    const_iterator cEnd()   const noexcept { test(); return const_iterator(nullptr); }
    const_iterator begin() const noexcept { test(); return const_iterator(this->_data); }
    const_iterator end()   const noexcept { test(); return const_iterator(nullptr); }
     */
};

template<class T>
inline WVector<T> &WVector<T>::operator=(const WVector<T> &other)
{
    _data = other._data;
    return *this;
}

template <class T>
inline WVector<T> &WVector<T>::operator=(WVector<T> &&other) noexcept
{
    _data = std::move(other._data);
    return *this;
}

template<class T>
inline T WVector<T>::takeAt(int i)
{
    W_ASSERT(i >= 0 && i < size());
    auto iterator = this->_data.begin() + i;

    T element = std::move(_data[i]);

    _data.erase(iterator);

    return element;
}

template<class T>
inline void WVector<T>::insert(int index, T &&data)
{
    this->_data.insert(
            _data.begin() + index,
            std::move(data)
    );
}

template<class T>
inline void WVector<T>::insert(int index, const T &data)
{
    this->_data.insert(
            _data.begin() + index,
            data
    );
}

template<class T>
inline T WVector<T>::takeFirst()
{
    T copy = std::move(_data.at(0));
    _data.erase(_data.begin(), _data.begin() + 1);
    return copy;
    /*
    W_ASSERT(size() > 0);

    T item = std::move(at(0));

    if (_reserve > 0) {
        // we "just" need to move the item
        for (int i = 0; i < _size - 1; i++) {
            new (_data + sizeof(T) * i) T(_data[i]);
        }

        _reserve ++;
    } else {
        // we need to reallocate and move all the item
        T *newMem = (T*) malloc(sizeof (T) * (_size - 1));

        for (int i = 0; i < _size - 1; i++) {
            new (newMem + sizeof(T) * i) T(this->_data[i + 1]);
        }

        // dealloc last memory
        for (int i = 0; i < _size; i++) {
            _data[i].~T();
        }

        free(_data);
        _data = newMem;
    }

    _size --;
    return item;
     */
}

template<class T>
inline void WVector<T>::test() const
{

}

template<class T>
inline T &WVector<T>::operator[](int index)
{
    return _data[index];
}

template<class T>
inline const T &WVector<T>::at(int i) const
{
    return _data[i];
}

template<class T>
inline void WVector<T>::move(int from, int to)
{
    std::iter_swap(_data.begin() + from, _data.begin() + to);
    /*
    const auto &tmp = this->at(from);
    this->_data[from] = _data[to];
    this->_data[to] = tmp;
     */
}

template<class T>
inline void WVector<T>::removeAt(int index)
{
    W_ASSERT(index >= 0 && index < size());

    this->_data.erase(_data.begin() + index, _data.begin() + index + 1);

    /*
    // TODO: create a function removeAt that accept a vector of position

    if (_reserve == 0 && !useReserve) {
        auto *d = (T *) WMalloc(sizeof (T) * (_size - 1));

        for (int i = 0; i < index; i++) {
            d[i] = _data[i];
        }

        for (int i = index + 1; i < size(); i++) {
            d[i - 1] = _data[i];
        }
    } else {
        for (int i = index; i < size() - 1; i++) {
            this->_data[i] = _data[i + 1];
        }

        _reserve ++;
    }

    _size --;
     */
}

template<class T>
inline void WVector<T>::clear()
{
    /*
    free(_data);
    _data = nullptr;
    _size = 0;
    _reserve = 0;
     */
}

template<class T>
inline bool WVector<T>::isEmpty() const
{
    return size() == 0;
}

template<class T>
inline const T *WVector<T>::constData() const
{
    return this->_data.data();
}

template<class T>
inline int WVector<T>::indexOf(const T &object)
{
    const auto pos = std::find(_data.cbegin(), _data.cend(), object);
    if (pos == _data.cend())
        return -1;
    return pos - _data.cbegin();
}

template<class T>
inline bool WVector<T>::operator==(const WVector<T> &other) const
{
    return this->_data == other._data;
}

template<class T>
inline bool WVector<T>::operator!=(const WVector<T> &other) const
{
    return this->_data != other._data;
}

template<class T>
inline const T &WVector<T>::last() const
{
    return get(size() - 1);
}

template<class T>
inline const T &WVector<T>::first() const
{
    W_ASSERT(size() > 0);
    return _data[0];
}

template<class T>
inline int WVector<T>::size() const
{
    return _data.size();
}

template<class T>
inline const T &WVector<T>::get(int i) const
{
    W_ASSERT(i >= 0 && i < size());
    return this->_data[i];
}

template<class T>
inline WVector<T>::~WVector()
{
    /*
    free(_data);
    _data = nullptr;

    _size = 0;
    _reserve = 0;
    */
}

template<class T>
inline void WVector<T>::reserve(int numberOfElement)
{
    /*
    W_ASSERT(numberOfElement);

    auto *newData = (T *) malloc (sizeof (T) * (_size + _reserve + numberOfElement));

    for (size_t i = 0; i < _size; i++) {
        newData[i] = _data[i];
    }

    _reserve += numberOfElement;
     */
    _data.reserve(numberOfElement);
}

template <class T>
inline void WVector<T>::append(T &&item)
{
    _data.push_back(std::move(item));
}

template <class T>
inline void WVector<T>::append(const T& item)
{
    /*
    if (_reserve > 0) {
        new (_data + sizeof (T) * _size) T(item);
        _reserve --;
        _size ++;
    } else {
        this->reserve(32);
        return append(item);
    }
     */

    _data.push_back(item);
}

template<class T>
inline void WVector<T>::append(const WVector<T> &other)
{
    /*
    if (_reserve - other.size() > 0) {
        for (auto &item: std::as_const(other)) {
            this->append(item);
        }
    } else {
        reserve(other.size() - _reserve);
        return append(other);
    }
    */
    _data.insert(_data.end(), other._data.cbegin(), other._data.cend());
}

template<class T>
inline WVector<T>::WVector()
    : _data()
{

}

template <class T>
inline WVector<T>::WVector(const WVector<T> &other) noexcept
    : _data(other._data)
{}

template <class T>
inline WVector<T>::WVector(WVector<T> &&other) noexcept
    : _data(std::move(other._data))
{
}

#ifdef USE_QT
template <class T>
inline Q_CORE_EXPORT QDebug operator<<(QDebug d, const WVector<T> &p)
{
    d.nospace() << "WVector(";

    for (const auto &item: std::as_const(p)) {
        d.nospace() << item ;
    }

    d.nospace() << ")";

    return d.space();
}
#endif // USE_QT

#endif //WRITERNOTE_WVECTOR_H
