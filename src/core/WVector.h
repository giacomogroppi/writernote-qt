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
#include "core/Index.h"

template <class T>
class WVector
{
private:

    using Size = size_t;

    T *_data;
    Size _size;
    Size _reserve;

    static constexpr const auto useReserve = true;

    /*std::vector<T> _data;*/
    static constexpr auto numberOfAllocation = 64;

    void test() const;

    template <typename ...Args>
    static void callConstructorOn(T* array, int index, Args&& ...args);
public:
    WVector() noexcept;
    WVector(const WVector<T> &other) noexcept;
    WVector(WVector<T> &&other) noexcept;
    WVector(std::initializer_list<T> &&object) noexcept;
    explicit WVector(int reserve) noexcept;
    ~WVector();

    void append(WVector<T> &&other) noexcept;
    void append(const WVector<T> &other) noexcept;

    auto append(const T &item) noexcept -> WVector<T>&;
    auto append(T &&item) noexcept -> WVector<T>&;

    auto get(Index i) const noexcept -> const T&;
    nd auto size() const noexcept -> Size;

    template <class T2 = T>
    auto isOrderLowToHigh() const -> bool;

    template <class T2 = T>
    auto isOrderHighToLow() const -> bool;

    /**
     * Remove all the object [from, to)
     * */
    void removeAll(Index from, Index to) noexcept;
    void removeAt(Index i) noexcept;
    void move(Index from, Index to) noexcept;
    void clear() noexcept;
    auto at(Index i) const noexcept -> const T&;
    void reserve(long numberOfElement) noexcept;
    auto takeAt(Index i) noexcept -> T;
    auto last() const noexcept ->  const T&;
    auto first() const noexcept -> const T&;
    auto operator[](int index) -> T&;
    auto operator[](Index index) -> T&;
    auto operator[](Index index) const -> T&;

    nd auto isEmpty() const noexcept -> bool;

    void insert(Index index, WVector<T> &&vector) noexcept;
    void insert(Index index, T &&data) noexcept;
    void insert(Index index, const T& data) noexcept;

    auto takeFirst() noexcept -> T;

    [[deprecated]]
    auto constData() const noexcept -> const T*;

    auto contains(const T& value) const noexcept -> bool;

    /**
     * \return The index of the object, -1 in case there is no object equals to object
     * */
    nd auto indexOf(const T &object) const noexcept -> Index;

    /**
     * \brief This method removeObjects all the occurrences of object in the array
     * \param object The object to be removed from the array
     * \return The number of object removed
     */
     // we want to disable this method if T is (int, unsigned, long, ...)
    template <class T2 = T> requires (std::is_convertible<T2, T>::value and not std::is_arithmetic<T2>::value)
    auto removeObjects(const T2& object) noexcept -> int;

    /**
     * \param index The lists of the positions to remove (need to be sorted from low index to high index)
     * */
    void removeAt(const WVector<Index> &index) noexcept;

    auto removeIf(Fn<bool(const T& object)> method) noexcept -> int;

    /**
     * \brief This method removeObjects the first occurrence of the object in the array
     * \param object The object to be removed from the array
     * \return True iff the function removed the object from the data structure
     */
     // TODO: rename to removeObjectSingle
    auto removeSingle(const T& object) -> bool;

    /**
     * This method requires that the list is order in ascending order [crescente]
     * \return True iff we have removeObjects the object passed
     * \param cmp return true iff v1 >= v2
     */
    auto removeOrderLowToHigh(const T& object, const Fn<bool(const T &, const T &)> &cmp) noexcept -> bool;

    /**
     * This method requires that the list is order in descending order
     * \param cmp return true iff v1 >= v2
     * \return true iff we have removeObjects the object passed
     * */
    auto removeOrderHighToLow(const T& object, const Fn<bool(const T &, const T &)> &cmp) noexcept -> bool;

    auto operator=(const WVector<T> &other) -> WVector<T>&;
    auto operator=(WVector &&other) noexcept -> WVector<T>&;
    auto operator==(const WVector<T> &other) const -> bool;
    auto operator!=(const WVector<T> &other) const -> bool;

    auto operator[](int i) const -> const T&;

    template <class T2>
    class AbstractIterator
    {
    private:
        T *array;
        long index;
#ifdef DEBUGINFO
        long max;
#endif // DEBUGINFO
    public:
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using iterator_category = std::random_access_iterator_tag;

#if !defined(DEBUGINFO)
        explicit AbstractIterator(T *data, int index, int max) : array(data), index(index) { unused(max); };
#else
        explicit AbstractIterator(T* data, long index, long max): array(data), index(index), max(max) {};
#endif // DEBUGINFO

        AbstractIterator(const AbstractIterator& other) noexcept = default;
        AbstractIterator(AbstractIterator&& other) noexcept = default;

        auto operator->() const -> T* { W_ASSERT(index >= 0 and index < max); return array[index]; };
        auto operator*()  const -> T& { W_ASSERT(index >= 0 and index < max); return array[index]; };

        auto operator++() -> AbstractIterator & { index ++; return *this; }
        auto operator--() -> AbstractIterator & { index --; return *this; }

        auto operator--(int) -> AbstractIterator { auto copy = *this; --*this; return copy; }
        auto operator++(int) -> AbstractIterator { auto copy = *this; ++*this; return copy; }

        auto operator+(int i) const -> AbstractIterator { AbstractIterator tmp(*this); tmp.index = index + i; return tmp; }
        auto operator-(int i) const -> AbstractIterator { AbstractIterator tmp(*this); tmp.index = index - i; return tmp; }

        auto operator-(const AbstractIterator& other) const -> difference_type { return index - other.index; };
        auto operator+(const AbstractIterator& other) const -> AbstractIterator { return index + other.index; };

        auto operator+=(int value) -> AbstractIterator& { index += value; return *this; };
        auto operator-=(int value) -> AbstractIterator& { index -= value; return *this; };

        auto operator<(const AbstractIterator& other) const -> bool { return this->index < other.index; }
        auto operator>(const AbstractIterator& other) const -> bool { return this->index > other.index; }
        auto operator<=(const AbstractIterator& other) const -> bool { return this->index <= other.index; }
        auto operator>=(const AbstractIterator& other) const -> bool { return this->index >= other.index; }
        auto operator<=>(const AbstractIterator& other) const -> int
        {
            if (index < other.index)
                return -1;
            if (index > other.index)
                return 1;
            return 0;
        }

        auto operator[](long i) -> T& { W_ASSERT(i >= 0 and i < max); return array[i]; };

        auto operator==(const AbstractIterator& iter) const -> bool = default;
        auto operator!=(const AbstractIterator& iter) const -> bool = default;

        auto operator=(const AbstractIterator& other) -> AbstractIterator& = default;
        auto operator=(AbstractIterator&& other) -> AbstractIterator& = default;

        explicit operator int() const { return index; };
    };

    using iterator = AbstractIterator<T>;
    using const_iterator = AbstractIterator<const T>;

    using reverse_iterator = std::reverse_iterator<iterator>;
    using reverse_const_iterator = std::reverse_iterator<const_iterator>;

    nd auto rbegin() noexcept -> reverse_iterator { return reverse_iterator(end()); }
    nd auto rend() noexcept -> reverse_iterator { return reverse_iterator(begin()); }

    nd auto begin() noexcept -> iterator { return iterator(this->_data, 0u, _size); };
    nd auto end()   noexcept -> iterator { return iterator(this->_data, size(), _size);  };

    nd auto constBegin() const noexcept { return const_iterator(_data, 0u, _size); }
    nd auto constEnd()   const noexcept { return const_iterator(_data, size(), _size); }
    nd auto cBegin() const noexcept { return const_iterator(_data, 0u, _size); }
    nd auto cEnd()   const noexcept { return const_iterator(_data, size(), _size); }
    nd auto begin() const noexcept { return const_iterator(_data, 0u, _size); }
    nd auto end()   const noexcept { return const_iterator(_data, size(), _size); }

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
            const Fn<SharedPtrThreadSafe<WTask>(
                    Fn<void()>
            )> &startNewThread
    ) noexcept -> WPair<int, WVector<T2>>;
};

template<class T>
void WVector<T>::removeAt(const WVector<Index> &index) noexcept
{
    // TODO: optimize
    if (index.isEmpty())
        return;

    for (Index i = index.size() - 1; i >= 0; i--) {
        removeAt(index[i]);
    }
}

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
        const Fn<SharedPtrThreadSafe<WTask>(
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
inline auto WVector<T>::contains(const T &value) const noexcept -> bool
{
    return indexOf(value).isValid();
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
auto WVector<T>::removeAll(Index from, Index to) noexcept -> void
{
    // TODO: optimize
    W_ASSERT(from.isValid() and to.isValid() and from < to and to.value() <= size());

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

    return *this;
}

template <class T>
inline auto WVector<T>::operator=(WVector<T> &&other) noexcept -> WVector<T> &
{
    if (this == &other)
        return *this;

    clear();

    _data = other._data;
    _size = other._size;
    _reserve = other._reserve;

    other._data = nullptr;
    other._size = 0u;
    other._reserve = 0u;

    return *this;
}

template<class T>
inline auto WVector<T>::takeAt(Index i) noexcept -> T
{
    W_ASSERT(i < size());

    T element = std::move(_data[i]);

    removeAt(i);

    return element;
}

template <class T>
inline void WVector<T>::insert(Index index, WVector<T> &&vector) noexcept {
    if (_reserve < vector.size()) {
        T* newData = (T*) malloc (sizeof(T) *
                (_size + vector.size() + WVector::numberOfAllocation)
        );

        for (size_t i = 0; i < index.value(); i++)
            callConstructorOn(newData, i, std::forward<T>(_data[i]));

        for (size_t i = index.value(); i < index.value() + vector.size(); i++){
            callConstructorOn(
                    newData,
                    i,
                    std::forward<T>(
                            vector._data[i - index.value()]
                    )
            );
        }

        for (size_t i = index.value() + vector.size(); i < _size + vector.size(); i++)
            callConstructorOn(newData, i, std::forward<T>(_data[i - vector.size()]));

        free(_data);
        _data = newData;
        _size = vector.size() + _size;
        _reserve = WVector::numberOfAllocation;
    } else {
        // we have enough memory
        // move memory
        for (Size i = _size - 1 + vector.size();
                    i >= index.value() + vector.size();
                    i--
                ) {
            callConstructorOn(_data, i, std::forward<T>(_data[i - vector.size()]));
        }

        // we copy the memory from the other WVector
        for (Size i = index.value(); i < vector.size() + index.value(); i++)
            callConstructorOn(_data, i, std::forward<T>(vector._data[i - index.value()]));

        _reserve -= vector.size();
        _size += vector.size();
    }

    free(vector._data);
    vector._data = nullptr;
    vector._size = 0;
    vector._reserve = 0;
}

template<class T>
inline void WVector<T>::insert(Index index, T &&data) noexcept {
    WVector<T> tmp(1);
    tmp.append(std::forward<T>(data));
    this->insert(index, std::move(tmp));
}

template<class T>
inline void WVector<T>::insert(Index index, const T &data) noexcept {
    T object = data;

    return insert(index, std::forward<T>(object));
}

template<class T>
inline auto WVector<T>::takeFirst() noexcept -> T
{
    W_ASSERT(size() > 0);

    const T item(std::move(_data[0]));

    if (_reserve < WVector::numberOfAllocation and WVector::useReserve) {
        // we "just" need to move the item
        for (int i = 0; i < _size - 1; i++) {
            callConstructorOn(_data, i, std::forward<T>(_data[i + 1]));
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
auto WVector<T>::at(Index i) const noexcept -> const T&
{
    return _data[i.value()];
}

template<class T>
inline void WVector<T>::move(Index from, Index to) noexcept {
    W_ASSERT(from.value() < size() and to.value() < size());

    if (from == to)
        return;

    T tmp = std::forward<T>(_data[from.value()]);
    this->_data[from.value()] = std::forward<T>(_data[to.value()]);
    this->_data[to.value()] = std::forward<T>(tmp);
}

template<class T>
inline void WVector<T>::removeAt(Index index) noexcept {
    W_ASSERT(index.value() < size());

    _data[index.value()].~T();

    if (_reserve >= WVector::numberOfAllocation) {
        T* newData = (T*) malloc (sizeof(T) * (_size - 1));

        for (Size i = 0; i < index.value(); i++) {
            callConstructorOn(newData, i, std::forward<T>(_data[i]));
        }

        for (Size i = index.value() + 1; i < _size; i++) {
            callConstructorOn(newData, i - 1, std::forward<T>(_data[i]));
        }

        free(_data);
        _data = newData;

        _reserve = 0u;
    } else {
        // we just move
        for (unsigned i = index.value() + 1; i < _size; i++) {
            this->move(Index(i), Index(i - 1));
        }
        _reserve ++;
    }

    _size --;
}

template<class T>
inline void WVector<T>::clear() noexcept {
    for (unsigned i = 0; i < _size; i++)
        _data[i].~T();

    free(_data);
    _data = nullptr;
    _size = 0;
    _reserve = 0;
}

template<class T>
auto WVector<T>::isEmpty() const noexcept -> bool
{
    return size() == 0;
}

template<class T>
inline auto WVector<T>::constData() const noexcept -> const T *
{
    return this->_data;
}

template<class T>
inline auto WVector<T>::indexOf(const T &object) const noexcept -> Index
{
    auto i = begin();

    for (; i != end(); i++)
        if (*i == object)
            break;

    if (i == end())
        return Index::makeInvalid();

    return Index(i - begin());
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
inline auto WVector<T>::last() const noexcept -> const T &
{
    return get(size() - 1);
}

template<class T>
inline auto WVector<T>::first() const noexcept -> const T &
{
    return at(0);
}

template<class T>
inline auto WVector<T>::size() const noexcept -> Size
{
    return _size;
}

template<class T>
inline auto WVector<T>::get(Index i) const noexcept -> const T &
{
    W_ASSERT(i.value() < size());
    return this->_data[i.value()];
}

template<class T>
inline WVector<T>::~WVector()
{
    clear();
}

template<class T>
inline void WVector<T>::reserve(long numberOfElement) noexcept {
    W_ASSERT(numberOfElement >= 0);
    if (numberOfElement == 0)
        return;

    auto *newData = (T *) malloc (sizeof (T) * (_size + _reserve + numberOfElement));

    for (Size i = 0; i < _size; i++) {
        callConstructorOn(newData, i, std::forward<T>(_data[i]));
    }

    _reserve += numberOfElement;

    free (_data);
    _data = newData;
}

template <class T>
inline auto WVector<T>::append(T &&item) noexcept -> WVector<T> &
{
    if (_reserve == 0)
        reserve(WVector::numberOfAllocation);

    this->callConstructorOn(_data, _size, std::forward<T>(item));

    _size ++;
    _reserve --;

    return *this;
}

template <class T>
inline auto WVector<T>::append(const T& item) noexcept -> WVector<T>&
{
    T object = item;
    return append(std::forward<T>(object));
}

template <class T>
inline void WVector<T>::append(WVector<T> &&item) noexcept {
    if (_reserve >= item.size())
        reserve(item.size() > WVector::numberOfAllocation
                    ? item.size()
                    : WVector::numberOfAllocation);

    for (Size i = 0; i < item.size(); i++) {
        callConstructorOn(_data, i + _size, std::forward<T>(item._data[i]));
    }

    _size += item.size();
    _reserve -= item.size();

    free(item._data);
    item._data = nullptr;
    item._size = 0u;
    item._reserve = 0u;
}

template<class T>
inline void WVector<T>::append(const WVector<T> &other) noexcept {
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
inline WVector<T>::WVector() noexcept
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
inline auto WVector<T>::removeIf(Fn<bool(const T &)> method) noexcept -> int
{
    WVector<Index> positions;

    for (Size i = 0; i < size(); i++) {
        if (method(at(i)))
            positions.append(i);
    }

    removeAt(positions);

    return positions.size();
}

template <class T>
template <class T2> requires (std::is_convertible<T2, T>::value and not std::is_arithmetic<T2>::value)
inline auto WVector<T>::removeObjects(const T2 &object) noexcept -> int
{
    // TODO: optimize
    int numberOfElementsRemoved = 0;
    auto value = indexOf(object);

    while (value.isValid()) {
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

template <class T>
inline auto WVector<T>::operator[](Index index) -> T &
{
    return _data[index.value()];
}

template <class T>
inline auto WVector<T>::operator[](Index index) const -> T &
{
    return _data[index.value()];
}
