#pragma once

#include <iostream>
#include "utils/WCommonScript.h"
#include "testing/memtest.h"
#include "WAbstractList.h"
#include "Writable.h"
#include "Readable.h"
#include "VersionFileController.h"
#include "FileContainer/MemWritable.h"
#include "Scheduler/WTask.h"
#include "core/WPair.h"



template <class T>
class WVector
{
private:

    T *_data;
    size_t _size;
    size_t _reserve;

    static constexpr const auto useReserve = false;

    /*std::vector<T> _data;*/
    static constexpr auto numberOfAllocation = 64;

    void test() const;

    template <typename ...Args>
    static void callConstructorOn(T* array, int index, Args&& ...args);
public:
    WVector();
    WVector(const WVector<T> &other) noexcept;
    WVector(WVector<T> &&other) noexcept;
    WVector(std::initializer_list<T> &&object) noexcept;
    explicit WVector(int reserve) noexcept;
    ~WVector();

    //using const_iterator = typename std::vector<T>::const_iterator;
    //using iterator = typename std::vector<T>::iterator;

    //using riterator = typename std::vector<T>::reverse_iterator;
    //using rconst_iterator = typename std::vector<T>::const_reverse_iterator;

    void append(const WVector<T> &other);

    void append(const T &item);
    auto append(T &&item) -> WVector<T> &;

    auto get(int i) const -> const T&;
    nd auto size() const -> int;

    template <class T2 = T>
    auto isOrderLowToHigh() const -> bool;

    template <class T2 = T>
    auto isOrderHighToLow() const -> bool;

    /**
     * Remove all the object [from, to)
     * */
    void removeAll(int from, int to);
    void removeAt(int i);
    void move(int from, int to);
    void clear();
    auto at(int i) const -> const T&;
    void reserve(int numberOfElement);
    auto takeAt(int i) -> T;
    auto last() const ->  const T&;
    auto first() const -> const T&;
    auto operator[](int index) -> T&;
    nd auto isEmpty() const -> bool;

    void insert(int index, T &&data);
    void insert(int index, const T& data);

    auto takeFirst() -> T;

    [[deprecated]]
    auto constData() const -> const T*;

    auto contains(const T& value) const -> bool;

    /**
     * \return The index of the object, -1 in case there is no object equals to object
     * */
    auto indexOf(const T &object) const -> int;

    /**
     * \brief This method remove all the occurrences of object in the array
     * \param object The object to be removed from the array
     * \return The number of object removed
     * */
    auto remove(const T& object) -> int;

    /**
     * \brief This method remove the first occurrence of the object in the array
     * \param object The object to be removed from the array
     * \return True iff the function removed the object from the data structure
     */
    auto removeSingle(const T& object) -> bool;

    /**
     * This method requires that the list is order in ascending order [crescente]
     * \return True iff we have remove the object passed
     * \param cmp return true iff v1 >= v2
     */
    auto removeOrderLowToHigh(const T& object, const Fn<bool(const T &, const T &)> &cmp) noexcept -> bool;

    /**
     * This method requires that the list is order in descending order
     * \param cmp return true iff v1 >= v2
     * \return true iff we have remove the object passed
     * */
    auto removeOrderHighToLow(const T& object, const Fn<bool(const T &, const T &)> &cmp) noexcept -> bool;

    auto operator=(const WVector<T> &other) -> WVector<T>&;
    auto operator=(WVector &&other) noexcept -> WVector<T>&;
    auto operator==(const WVector<T> &other) const -> bool;
    auto operator!=(const WVector<T> &other) const -> bool;

    auto operator[](int i) const -> const T&;

    class iterator
    {
    private:
        T *array;
        int index;
    public:
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using iterator_category = std::random_access_iterator_tag;

        explicit iterator(T *data, int index) : array(data), index(index) {  };

        iterator(const iterator& other) noexcept = default;
        iterator(iterator&& other) noexcept = default;

        auto operator->() const -> T* { return array[index]; };
        auto operator*()  const -> T& { return array[index]; };

        auto operator++() -> iterator & { index ++; return *this; }
        auto operator--() -> iterator & { index --; return *this; }

        iterator operator--(int) { auto copy = *this; --*this; return copy; }
        iterator operator++(int) { auto copy = *this; ++*this; return copy; }

        iterator operator+(int i) const { iterator tmp(*this); tmp.index = index + i; return tmp; }
        iterator operator-(int i) const { iterator tmp(*this); tmp.index = index - i; return tmp; }

        auto operator-(const iterator& other) const -> difference_type { return index - other.index; };
        auto operator+(const iterator& other) const -> iterator { return index + other.index; };

        auto operator+=(int value) -> iterator& { index += value; return *this; };
        auto operator-=(int value) -> iterator& { index -= value; return *this; };

        auto operator<(const iterator& other) const -> bool { return this->index < other.index; }
        auto operator>(const iterator& other) const -> bool { return this->index > other.index; }
        auto operator<=(const iterator& other) const -> bool { return this->index <= other.index; }
        auto operator>=(const iterator& other) const -> bool { return this->index >= other.index; }
        auto operator<=>(const iterator& other) const -> int
        {
            if (index < other.index)
                return -1;
            if (index > other.index)
                return 1;
            return 0;
        }

        auto operator[](int i) -> T& { return array[i]; };

        /*
        auto operator<(const class const_iterator& other) const -> bool;
        auto operator>(const class const_iterator& other) const -> bool;
        auto operator<=(const class const_iterator& other) const -> bool;
        auto operator>=(const class const_iterator& other) const -> bool;
        auto operator<=>(const class const_iterator& other) const -> int;
        */

        auto operator==(const iterator& iter) const -> bool = default;
        auto operator!=(const iterator& iter) const -> bool = default;

        auto operator=(const iterator& other) -> iterator& = default;
        auto operator=(iterator&& other) -> iterator& = default;

        explicit operator int() const { return index; };
    };

    class const_iterator
    {
    private:
        const T *array;
        int index;
    public:
        explicit const_iterator(const T *data, int index) : array(data), index(index) {};

        auto operator->() const -> const T*  { return array[index]; };
        auto operator*() const -> const T&   { return array[index]; };

        auto operator++() -> const_iterator& { index ++; return *this; }
        auto operator--() -> const_iterator& { index --; return *this; }

        const_iterator operator--(int) { auto copy = *this; --*this; return copy; }
        const_iterator operator++(int) { auto copy = *this; ++*this; return copy; }

        const_iterator operator+(int i) { const_iterator tmp(*this); tmp.index = index + i; return tmp; }

        const_iterator operator-(int i) { const_iterator tmp(*this); tmp.index = index - i; return tmp; }

        auto operator-(const const_iterator& other) const -> const_iterator
            { return const_iterator(array, index - other.index); }

        auto operator+(const const_iterator& other) const -> const_iterator
            { return const_iterator(array, index + other.index); }

        auto operator<(const const_iterator& other) const -> bool { return this->index < other.index; }

        auto operator+=(int value) -> const_iterator& { index += value; return *this; };
        auto operator-=(int value) -> const_iterator& { index -= value; return *this; };

        auto operator>(const const_iterator& other) const -> bool { return this->index > other.index; }
        auto operator<=(const const_iterator& other) const -> bool { return this->index <= other.index; }
        auto operator>=(const const_iterator& other) const -> bool { return this->index >= other.index; }
        auto operator<=>(const const_iterator& other) const -> int
        {
            if (index < other.index)
                return -1;
            if (index > other.index)
                return 1;
            return 0;
        }

        /*
        auto operator>(const iterator& other) const -> bool { return this->index > other.index; }
        auto operator<=(const iterator& other) const -> bool { return this->index <= other.index; }
        auto operator>=(const iterator& other) const -> bool { return this->index >= other.index; }
        auto operator<=>(const iterator& other) const -> int
        {
            if (index < other.index)
                return -1;
            if (index > other.index)
                return 1;
            return 0;
        }
        */

        auto operator==(const const_iterator& iter) const -> bool = default;
        auto operator!=(const const_iterator& iter) const -> bool = default;

        explicit operator int() const { return index; };
    };

    using reverse_iterator = std::reverse_iterator<iterator>;
    using reverse_const_iterator = std::reverse_iterator<const_iterator>;

    nd auto rbegin() noexcept -> reverse_iterator { return reverse_iterator(end()); }
    nd auto rend() noexcept -> reverse_iterator { return reverse_iterator(begin()); }

    nd auto begin() noexcept -> iterator { return iterator(this->_data, 0u); };
    nd auto end()   noexcept -> iterator { return iterator(this->_data, size());  };

    nd auto constBegin() const noexcept -> const_iterator { return const_iterator(_data, 0u); }
    nd auto constEnd()   const noexcept -> const_iterator { return const_iterator(_data, size()); }
    nd auto cBegin() const noexcept -> const_iterator { return const_iterator(_data, 0u); }
    nd auto cEnd()   const noexcept -> const_iterator { return const_iterator(_data, size()); }
    nd auto begin() const noexcept -> const_iterator { return const_iterator(_data, 0u); }
    nd auto end()   const noexcept -> const_iterator { return const_iterator(_data, size()); }

    /**
     * \return < 0 if error
     * */
    static auto load (const VersionFileController &versionController, ReadableAbstract &file) -> WPair<int, WVector<T>>;

    /**
     * \param writable needs to have write(const void *data, size_t size) and it needs to return < 0 in case
     *  of failure and it needs to have write(const T &param) for non class object
     *
     * \return -1 in case of error
     * */
    template <class T2 = T>
    static auto write(WritableAbstract &writable, const WVector<T2> &list) noexcept -> int;

    template <class StartNewThreadFunction, class T2 = T>
    static auto writeMultiThread (WritableAbstract &writable, const WVector<T2> &list,
                           StartNewThreadFunction startNewThread) noexcept -> int;

    template<class T2 = T>
    static
    auto loadMultiThread (
            const VersionFileController &versionController,
            ReadableAbstract &readable,
            const Fn<WTask *(
                    Fn<void()>
            )> &startNewThread
    ) noexcept -> WPair<int, WVector<T2>>;
};

template <class T>
inline WVector<T>::WVector(int reserve) noexcept
    : WVector()
{
    this->reserve(reserve);
}

template <class T>
template <class StartNewThreadFunction, class T2>
inline auto WVector<T>::writeMultiThread(WritableAbstract &writable, const WVector<T2> &list,
                                         StartNewThreadFunction startNewThread) noexcept -> int
{
    return WAbstractList::writeMultiThread<WVector, T2>(writable, list, startNewThread);
}

template <class T>
template <class T2>
inline auto WVector<T>::loadMultiThread(
        const VersionFileController &versionController, ReadableAbstract &readable,
        const Fn<WTask *(
                    Fn<void()>
                )> &startNewThread
    ) noexcept -> WPair<int, WVector<T2>>
{
    const auto reserveUnsafe = [] (WVector<T2>& list, int size) {
        list.reserve(size);
        list._size = list._reserve;
        list._reserve = 0;
    };

    const auto insertUnsafe = [] (WVector<T2> &list, T2 &&object, int index) {
        list.callConstructorOn(list._data, index, std::forward<T2>(object));
    };

    return WAbstractList::loadMultiThread<WVector, T2>(versionController, readable, startNewThread,
                                                       reserveUnsafe, insertUnsafe);
}

template <class T>
template <class T2>
inline auto WVector<T>::write(WritableAbstract &writable, const WVector<T2> &list) noexcept -> int
{
    return WAbstractList::write(writable, list);
}

template<class T>
inline auto WVector<T>::contains(const T &value) const -> bool
{
    return indexOf(value) != -1;
}

template<class T>
inline auto WVector<T>::removeOrderHighToLow(
            const T &object,
            const Fn<bool(const T &, const T &)> &cmp
        ) noexcept -> bool
{
    // ordine decrescente
    const auto iterator = WAbstractList::binary_search<WVector<T>::iterator, T, false>(
            begin(),
            end(),
            object,
            cmp
    );

    if (iterator == end())
        return false;

    const int indexFirst = iterator - begin();
    int lastIndex = indexFirst;

    for (auto i = indexFirst; i < size(); i++) {
        if (at(i) != object)
            break;
        lastIndex = i;
    }

    WDebug(true, "Before" << *this);
    removeAll(indexFirst, lastIndex + 1);
    WDebug(true, "After" << *this);

    return true;
}

template <class T>
auto WVector<T>::removeAll(int from, int to) -> void
{
    // TODO: optimize

    W_ASSERT(from >= 0 and from < to and to <= size());

    for (to --; to >= from ; to --)
        removeAt(to);
}

template<class T>
inline auto WVector<T>::operator=(const WVector<T> &other) -> WVector<T> &
{
    if (this == &other)
        return *this;

    clear();

    reserve(other.size());

    for (const auto& ref: std::as_const(other))
        append(ref);
}

template <class T>
inline auto WVector<T>::operator=(WVector<T> &&other) noexcept -> WVector<T> &
{
    if (this == &other)
        return *this;

    for (auto i = 0; i < size(); i++)
        _data[i].~T();

    free(_data);

    _data = other._data;
    _size = other._size;
    _reserve = other._reserve;

    other._data = nullptr;
    other._size = 0u;
    other._reserve = 0u;

    return *this;
}

template<class T>
inline auto WVector<T>::takeAt(int i) -> T
{
    W_ASSERT(i >= 0 && i < size());

    T element = std::move(_data[i]);

    removeAt(i);

    return element;
}

template<class T>
inline void WVector<T>::insert(int index, T &&data)
{
    // TODO: implement
    W_ASSERT(0);
}

template<class T>
inline void WVector<T>::insert(int index, const T &data)
{
    T object = data;

    return insert(index, std::forward<T>(object));
}

template<class T>
inline auto WVector<T>::takeFirst() -> T
{
    W_ASSERT(size() > 0);

    T item = std::move(at(0));

    if (_reserve > 0) {
        // we "just" need to move the item
        for (int i = 0; i < _size - 1; i++) {
            callConstructorOn(_data, i, std::forward<T>(_data[i]));
        }

        _reserve ++;
    } else {
        // we need to reallocate and move all the item
        T *newMem = (T*) malloc(sizeof (T) * (_size - 1));

        for (int i = 0; i < _size - 1; i++) {
            callConstructorOn(newMem, i, std::forward<T>(_data[i + 1]));
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
}

template<class T>
inline void WVector<T>::test() const
{

}

template<class T>
inline auto WVector<T>::operator[](int index) -> T &
{
    return _data[index];
}

template<class T>
auto WVector<T>::at(int i) const -> const T&
{
    return _data[i];
}

template<class T>
inline void WVector<T>::move(int from, int to)
{
    W_ASSERT(from >= 0 and from < size() && to >= 0 and to < size());

    if (from == to)
        return;

    T tmp = std::forward<T>(_data[from]);
    this->_data[from] = std::forward<T>(_data[to]);
    this->_data[to] = std::forward<T>(tmp);
}

template<class T>
inline void WVector<T>::removeAt(int index)
{
    W_ASSERT(index >= 0 && index < size());

    _data[index].~T();

    if (_reserve >= WVector::numberOfAllocation) {
        T* newData = (T*) malloc (sizeof(T) * (_size - 1));

        auto *now = _data;
        _data = newData;

        for (unsigned i = 0; i < index; i++) {
            callConstructorOn(_data, i, std::forward<T>(now[i]));
        }

        for (unsigned i = index + 1; i < _size; i++) {
            callConstructorOn(_data, i, std::forward<T>(now[i + 1]));
        }

        free(now);

        _reserve = 0u;
    } else {
        // we just move
        for (unsigned i = index + 1; i < _size; i++) {
            this->move(i, i - 1);
        }
        _reserve ++;
    }

    _size --;
}

template<class T>
inline void WVector<T>::clear()
{
    for (unsigned i = 0; i < _size; i++)
        _data[i].~T();

    free(_data);
    _data = nullptr;
    _size = 0;
    _reserve = 0;
}

template<class T>
auto WVector<T>::isEmpty() const -> bool
{
    return size() == 0;
}

template<class T>
inline auto WVector<T>::constData() const -> const T *
{
    return this->_data;
}

template<class T>
inline auto WVector<T>::indexOf(const T &object) const -> int
{
    auto b = begin();

    for (; b != end(); b++)
        if (*b == object)
            break;

    if (b == end())
        return -1;

    return (b - begin()).operator int();
}

template<class T>
inline auto WVector<T>::operator==(const WVector<T> &other) const -> bool
{
    if (size() != other.size())
        return false;

    auto b1 = begin();
    auto b2 = other.begin();
    auto e1 = end();

    for (; b1 != e1; b1++, b2 ++) {
        if (*b1 != *b2)
            return false;
    }

    return true;
}

template<class T>
inline auto WVector<T>::operator!=(const WVector<T> &other) const -> bool
{
    return not (*this == other);
}

template<class T>
inline auto WVector<T>::last() const -> const T &
{
    return get(size() - 1);
}

template<class T>
inline auto WVector<T>::first() const -> const T &
{
    return at(0);
}

template<class T>
inline auto WVector<T>::size() const -> int
{
    return _size;
}

template<class T>
inline auto WVector<T>::get(int i) const -> const T &
{
    W_ASSERT(i >= 0 && i < size());
    return this->_data[i];
}

template<class T>
inline WVector<T>::~WVector()
{
    clear();
}

template<class T>
inline void WVector<T>::reserve(int numberOfElement)
{
    W_ASSERT(numberOfElement >= 0);
    if (numberOfElement == 0)
        return;

    auto *newData = (T *) malloc (sizeof (T) * (_size + _reserve + numberOfElement));

    for (unsigned i = 0; i < _size; i++) {
        newData[i] = std::forward<T>(_data[i]);
    }

    _reserve += numberOfElement;

    free (_data);
    _data = newData;
}

template <class T>
inline auto WVector<T>::append(T &&item) -> WVector<T> &
{
    if (_reserve == 0) {
        reserve(WVector::numberOfAllocation);
    }

    this->callConstructorOn(_data, _size, std::forward<T>(item));

    _size ++;
    _reserve --;

    return *this;
}

template <class T>
inline void WVector<T>::append(const T& item)
{
    if (_reserve > 0) {
        callConstructorOn(_data, _size, item);
        _reserve --;
        _size ++;
    } else {
        this->reserve(WVector::numberOfAllocation);
        return append(item);
    }
}

template<class T>
inline void WVector<T>::append(const WVector<T> &other)
{
    if (_reserve - other.size() > 0) {
        for (auto &item: std::as_const(other)) {
            this->append(item);
        }
    } else {
        reserve(other.size() - _reserve);
        return append(other);
    }
}

template<class T>
inline WVector<T>::WVector()
    : _data(nullptr)
    , _size(0)
    , _reserve(0)
{

}

template <class T>
inline WVector<T>::WVector(const WVector<T> &other) noexcept
    : WVector(other.size())
{
    for (const auto& ref: std::as_const(other)) {
        append(ref);
    }
}

template <class T>
WVector<T>::WVector(std::initializer_list<T> &&object) noexcept
    : WVector(object.size())
{
    for (const auto& ref: std::as_const(object)) {
        append(ref);
    }
}

template <class T>
WVector<T>::WVector(WVector<T> &&other) noexcept
    : _data(other._data)
    , _size(other._size)
    , _reserve(other._reserve)
{
    other._data = nullptr;
    other._size = 0;
    other._reserve = 0;
}

#ifdef USE_QT
template <class T>
inline Q_CORE_EXPORT QDebug operator<<(QDebug d, const WVector<T> &p)
{
    d.nospace() << "WVector(";

    for (const auto &item: std::as_const(p)) {
        d.nospace() << item << ",";
    }

    d.nospace() << ")";

    return d.space();
}
#endif // USE_QT

template <class T>
inline auto WVector<T>::load(
        const VersionFileController &versionController,
        ReadableAbstract &file
) -> WPair<int, WVector<T>>
{
    return WAbstractList::load<WVector, T>(versionController, file);
}

template <class T>
inline auto WVector<T>::removeOrderLowToHigh(
            const T &object,
            const Fn<bool(const T &, const T &)> &cmp
        ) noexcept -> bool
{
#ifdef DEBUGINFO
    for (int i = 0; i < size() - 1; i++) {
        const bool isEquals = at(i) == at(i+1);
        const auto cmpResult = !cmp(at(i), at(i+1));
        W_ASSERT(isEquals || cmpResult);
    }
#endif // DEBUGINFO

    // ordine crescente
    const auto iterator = WAbstractList::binary_search<WVector<T>::iterator, T, true>(
            begin(),
            end(),
            object,
            [cmp](const T& v1, const T& v2) -> bool {
                return cmp(v1, v2);
            }
    );

    if (iterator == end())
        return false;
    const int index = iterator - begin();
    int lastIndex = index;

    for (int i = index; i < size(); i++) {
        if (at(i) != object) {
            break;
        }
        lastIndex = i;
    }

    removeAll(index, lastIndex + 1);

    return true;
}

template <class T>
inline auto WVector<T>::remove(const T &object) -> int
{
    // TODO: optimize
    int numberOfElementsRemoved = 0;
    int value = indexOf(object);

    while (value != -1) {
        removeAt(value);
        numberOfElementsRemoved ++;

        value = indexOf(object);
    }

    return numberOfElementsRemoved;
}

template <class T>
auto WVector<T>::removeSingle(const T &object) -> bool
{
    const auto index = this->indexOf(object);

    if (index == -1)
        return false;

    this->removeAt(index);

    return true;
}

template <class T>
inline std::ostream& operator<<(std::ostream& os, const WVector<T>& dt)
{
    // TODO: implement
    for (const auto &ref : dt) {
        os << ref;
    }
    return os;
}

template <class T>
template <typename... Args>
inline void WVector<T>::callConstructorOn(T* array, int index, Args&& ...args)
{
    new (&array [index]) T (std::forward<Args>(args)...);
}

template <class T>
inline auto WVector<T>::operator[](int i) const -> const T &
{
    W_ASSERT(i >= 0 and i < size());
    return this->_data[i];
}
