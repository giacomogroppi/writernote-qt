//
// Created by Giacomo Groppi on 13/05/23.
//

#ifndef WRITERNOTE_WLISTFAST_H
#define WRITERNOTE_WLISTFAST_H

#include <iostream>
#include <algorithm>
#include <utility>
#include "utils/WCommonScript.h"
#include "WAbstractList.h"
#include "VersionFileController.h"
#include "Writable.h"
#include "Readable.h"

// do some refactoring
// this list if O(1) in index access
template <class T>
class WListFast
{
private:
    void test() const;

    T **_data;
    int _size;
    int _reserved;

    static constexpr int numberOfObjectReserved = 128;

    // this function remove the object from the list
    T *takeObject(int i);

public:
    WListFast();
    WListFast(const WListFast<T> &other);
    WListFast(std::initializer_list<T> args);
    WListFast(WListFast<T> &&other) noexcept;

    const T& at(int i) const;
    void append(const T& element);
    void append(T &&object);
    void append(const WListFast<T> &other);

    void remove(int index);
    void remove(int from, int to);
    void remove(const T &item);

    [[nodiscard]] constexpr bool isEmpty() const;
    [[nodiscard]] constexpr int size() const;
    const T& first() const;

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
    WListFast<T>& operator=(WListFast<T> &&other) noexcept;

    template <typename Func>
    [[nodiscard]] bool anyMatch(Func func) const {
        for (const auto &value: *this) {
            if (func(value))
                return true;
        }
        return false;
    }

    /**
     * You can use this method only if <T> has the method load
     * \return < 0 if error
     * */
    static auto load (const VersionFileController &versionController, ReadableAbstract &file) -> std::pair<int, WListFast<T>>;


    class iterator
    {
    private:
        T ** _array;
        int _index;
    public:
        iterator(T **data, int index) noexcept : _array(data), _index(index) {; };

        T* operator->()         { return _array[_index]; };
        T &operator*() const    { return *_array[_index]; };
        constexpr bool operator==(iterator i) const         { return _index == i._index; }
        constexpr bool operator!=(iterator i) const         { return _index != i._index || _array != i._array; }
        iterator &operator++()                              { _index ++; return *this; }
        iterator operator++(int) { auto copy = *this; ++*this; return copy; }
        iterator operator+(int i) const { return iterator(_array, _index + i); }
        iterator operator-(int i) const { return iterator(_array, _index - i); };
    };

    class const_iterator
    {
    private:
        const T ** _array;
        int _index;
    public:
        const_iterator(const T **data, int index) noexcept : _array(data), _index(index) {};
        const_iterator(const const_iterator &other) : _array(other._array), _index(other._index) {}

        const T* operator->() const   { return _array[_index]; };
        const T &operator*() const    {
            return *_array[_index];
        };

        auto operator=(const const_iterator &other) -> const_iterator & = default;

        constexpr auto operator==(const_iterator i) const -> bool         { return _index == i._index && _array == i._array; }
        constexpr auto operator!=(const_iterator i) const -> bool         { return _index != i._index && _array == i._array; }
        auto operator++() -> const_iterator &                             { _index ++; return *this; }
        auto operator++(int) -> const_iterator { auto copy = *this; ++*this; return copy; }
    };

    [[nodiscard]] auto begin() noexcept -> iterator { return iterator((T **)_data, 0); };
    [[nodiscard]] auto end()   noexcept -> iterator { return iterator((T **)_data, size());  };

    [[nodiscard]] auto constBegin() const noexcept -> const_iterator { return const_iterator((const T **)_data, 0); }
    [[nodiscard]] auto constEnd()   const noexcept -> const_iterator { return const_iterator((const T **)_data, size()); }
    [[nodiscard]] auto cBegin() const noexcept -> const_iterator { return const_iterator((const T **)_data, 0); }
    [[nodiscard]] auto cEnd()   const noexcept -> const_iterator { return const_iterator((const T **)_data, size()); }
    [[nodiscard]] auto begin() const noexcept -> const_iterator { return const_iterator((const T **)_data, 0); }
    [[nodiscard]] auto end()   const noexcept -> const_iterator { return const_iterator((const T **)_data, size()); }

    /**
     * \param writable needs to have write(const void *data, size_t size) and it needs to return < 0 in case
     *  of failure and it needs to have write(const T &param) for non class object
     *
     * \return -1 in case of error
     * */
    template <class T2 = T>
    static
    auto write(WritableAbstract &writable, const WListFast<T2> &list) noexcept -> int
    {
        static_assert_type(list._size, int);

        if (writable.write(&list._size, sizeof(list._size)) < 0) {
            return -1;
        }

        for (const auto &ref: std::as_const(list)) {
            if (T2::write(writable, ref) < 0)
                return -1;
        }
        return 0;
    }
};

template<class T>
inline WListFast<T> &WListFast<T>::operator=(WListFast<T> &&other) noexcept
{
    if (this == &other)
        return *this;

    for (int i = size() - 1; i >= 0; i--) {
        delete this->_data[i];
    }

    free (this->_data);

    this->_data = other._data;
    this->_size = other._size;
    this->_reserved = other._reserved;

    other._data = nullptr;
    other._size = 0;
    other._reserved = 0;

    return *this;
}

template<class T>
inline void WListFast<T>::remove(const T &item)
{
    const auto index = indexOf(item);
    if (index >= 0) {
        remove(index);
    }
}

template<class T>
inline WListFast<T>::WListFast(const WListFast<T> &other)
    : _data(nullptr)
    , _size(0)
    , _reserved(0)
{
    this->reserve(other.size());
    this->append(other);
}

template<class T>
inline auto WListFast<T>::mid(int from, int to) const -> WListFast<T>
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
inline auto WListFast<T>::indexOf(const T &value) const -> int
{
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
        return;
        for (int i = 0; i < size(); i++) {
            for (int j = 0; j < size(); j++) {
                if (i != j) {
                    W_ASSERT(_data[i] != _data[j]);
                }
            }
        }
    } else {
        if (_reserved) {
            W_ASSERT(_data != nullptr);
        } else {
            W_ASSERT(_data == nullptr);
        }
    }
#endif // DEBUGINFO
}

template <class T>
inline auto WListFast<T>::load(
                const VersionFileController &versionController,
                ReadableAbstract &file
            ) -> std::pair<int, WListFast<T>>
{
    std::pair<int, WListFast<T>> result (-1, WListFast<T>());

    switch (versionController.getVersionWListFast()) {
        case 0:
            int i, element;

            if (file.read(&element, sizeof (element)) < 0) {
                return result;
            }

            result.second.reserve(element);

            for (i = 0; i < element; i++) {
                T tmp;

                auto [res, data] = T::load (versionController, file);
                if (res < 0 ) {
                    return {-1, WListFast<T>()};
                }

                result.second.append(
                    std::move (tmp)
                );
            }
            result.first = 0;
            return result;
        default:
            return {-1, WListFast<T>()};
    }
    return {-1, WListFast<T>()};
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
    free(this->_data);
    this->_data = nullptr;

    this->_size = other.size();
    this->_reserved = 0;

    if (_size) {
        this->_data = (T**) malloc(sizeof (T*) * other.size());

        for (int i = 0; i < size(); i++) {
            _data[i] = new T(other.at(i));
        }
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
        if (_size != 1) {
            this->_data = (T **) realloc(_data, (_size - 1) * sizeof (T *));
        } else {
            this->_data = nullptr;
        }
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
    if (reserve) {
        this->_reserved += reserve;
        this->_data = (T**) realloc(_data, sizeof (T*)  * (_size + _reserved));
    }

    test();
}

template<class T>
inline void WListFast<T>::clear()
{
    *this = WListFast<T>();
    test();
}

template<class T>
inline const T &WListFast<T>::first() const
{
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

    if (_reserved == 0)
        reserve(WListFast::numberOfObjectReserved);

    this->_data[_size - 1] = new T(element);
    _reserved --;

    test();
}

template <class T>
inline void WListFast<T>::append(T &&object)
{
    if (_reserved == 0)
        reserve(WListFast::numberOfObjectReserved);

    this->_data[_size] = new T(std::move(object));

    _size ++;
    _reserved --;
}

template<class T>
inline const T &WListFast<T>::at(int i) const
{
    W_ASSERT(i >= 0 && i < this->size());
    return *this->_data[i];
}

template<class T>
inline WListFast<T>::WListFast(std::initializer_list<T> args)
    : _data(nullptr)
    , _size(0)
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
    : _data(nullptr)
    , _size(0)
    , _reserved(0)
{
    test();
}

template <class T>
inline WListFast<T>::WListFast(WListFast && other) noexcept
    : _data(other._data)
    , _size(other._size)
    , _reserved(other._reserved)
{
    other._data = nullptr;
    other._size = 0;
    other._reserved = 0;
}

#ifdef USE_QT
template <class T>
inline Q_CORE_EXPORT QDebug operator<<(QDebug d, const WListFast<T> &p)
{
    d.nospace() << "ListFast(";

    for (const auto &item: std::as_const(p)) {
        d.nospace() << item ;
    }

    return d.space();
}
#endif // USE_QT

#endif //WRITERNOTE_WLISTFAST_H
