#pragma once

#include <iostream>
#include <algorithm>
#include <utility>
#include "utils/WCommonScript.h"
#include "WAbstractList.h"
#include "VersionFileController.h"
#include "Writable.h"
#include "Readable.h"
#include "core/AtomicSafe.h"
#include "Scheduler/WTask.h"
#include "FileContainer/MemWritable.h"
#include "FileContainer/MemReadable.h"
#include "core/WPair.h"
#include "WElement.h"
#include "Index.h"
#include <algorithm>

// TODO: do some refactoring
// this list if O(1) in index access
template <class T>
class WListFast
{
private:
    using Size = long;
    static constexpr auto debug = false;
    void rep() const noexcept;

    static_assert(std::is_move_constructible<T>::value);

#ifndef TEST_W
    static_assert(sizeof(T) > sizeof(void*)
                    or std::is_same<T, UnsignedLong>::value, "Use WVector for object smaller than sizeof(void*)");
#endif // TEST_W

    T **_data;
    Size _size;
    Size _reserved;

    static constexpr Size numberOfObjectReserved = 128;

    // this function remove the object from the list
    auto takeObject(Size i) noexcept -> T;

public:
    constexpr WListFast() noexcept;

    /**
     * \param reserve The reserve number of element
     * */
    explicit WListFast(Size reserve) noexcept;

    /**
     * \brief Copy constructor
     * */
    WListFast(const WListFast<T> &other) noexcept;

    /**
     * \brief Copy constructor
     * \param args The items to add to the list
     * */
    WListFast(std::initializer_list<T> args) noexcept;

    /**
     * \brief Move constructor.
     *  This method will erase the list pass as parameter
     * \param other The list to move
     * */
    WListFast(WListFast<T> &&other) noexcept;
    ~WListFast() noexcept;

    /**
     * \brief It's a O(1) operation
     * \param i The index of the item you want, it must be in the range [0, size())
     * \return The item in position "i"
     * */
    nd
    auto at(Size i) const noexcept -> const T&;

    /**
     * \brief Append the element to the list with a copy
     *  This method is O(1) if the list have some reserve space, otherwise is O(n)
     * \param element The element to append
     * \return The reference to the list
     * */
    auto append(const T& element) noexcept -> WListFast<T>&;

    /**
     * \brief Append the element to the list with a copy
     *  This method is O(1) if the list have some reserve space, otherwise is O(n)
     * \param object The element to append
     * \return The reference to the list
     * */
    auto append(T &&object) noexcept -> WListFast<T>&;

    /**
     * \brief This method make a copy of each elements in the list passed
     * as parameter and append in the current list
     * This method is faster that call multiple times append(const T&) because
     * it's gonna reallocate only once.
     *
     * \param other The list to append
     * */
    void append(const WListFast<T> &other) noexcept;

    /**
     * \brief This method move all the objects own by the list passed as parameter
     * into *this.
     * This method is faster that call multiple times append(const T&) because
     * it's gonna reallocate only once.
     *
     * \param other The list to append
     * */
    void append(WListFast<T> &&other) noexcept;

    template <class Iter>
    void append(Iter begin, Iter end);

    /**
     * \brief This method is gonna remove all elements that is contained in the range [*begin, *end)
     * \tparam Iter The class iterator
     * \param begin begin of the collection
     * \param end end of the collection
     * */
    template <class Iter> requires(std::is_class<Iter>::value)
    void removeAt(Iter begin, Iter end) noexcept;

    /**
     * \brief This method remove the object in position index
     * \param index The position of the item to remove
     * */
    void removeAt(Size index) noexcept;

    /**
     * \brief This method remove all the objected which indexes is between [begin, end)
     * */
    void remove(Size from, Size to) noexcept;
    void removeObjects(const T &item) noexcept;

    /**
     * return True iff the list is empty
     * */
    nd constexpr
    auto isEmpty() const noexcept -> bool;

    nd constexpr
    auto size() const noexcept -> Size;

    nd constexpr
    auto inBound(long index) const noexcept -> bool;

    /**
     * \param from The first element of the sublist
     * \param to The last element, not included, in the sublist
     * \return A const WAbstractList::SplitList in which the begin is defined by 'from' and
     *  the end defined by 'to'
     * */
    nd auto refMid(int from, int to) const noexcept -> WAbstractList::SplitList<WListFast, const T, Size>;

    /**
     * \param from The first element of the sublist
     * \param to The last element, not included, in the sublist
     * \return A WAbstractList::SplitList in which the begin is defined by 'from' and
     *  the end defined by 'to'
     * */
    nd auto refMid(int from, int to) noexcept -> WAbstractList::SplitList<WListFast, T, Size>;

    nd
    auto first() const noexcept -> const T&;

    void clear() noexcept;
    void move(Size from, Size to) noexcept;
    void reserve(Size numberOfElements) noexcept;
    auto takeAt(Size i) noexcept -> T;
    auto last() const noexcept -> const T&;
    auto indexOf(const T& value) const noexcept -> Size;
    auto lastIndexOf(const T& object) const noexcept -> Size;
    void insert(Size index, const T& object) noexcept;
    auto takeFirst() noexcept -> T;

    // functional programming
    void forAll(Fn<void(const T&)> method) const noexcept;
    void forAll(Fn<void(T&)> method) noexcept;

    auto filter(Fn<bool(const T&)> method) const -> WAbstractList::FilterList<WListFast, const T>;
    auto filter(Fn<bool(const T&)> method) -> WAbstractList::FilterList<WListFast, T>;


    [[nodiscard]]
    auto mid(Size from, Size to) const noexcept -> WListFast<T>;

    auto operator[](Size i) noexcept -> T&;

    auto operator==(const WListFast<T> &other) const noexcept -> bool;

    auto operator=(const WListFast<T> &other) noexcept -> WListFast<T>&;
    auto operator=(WListFast<T> &&other) noexcept -> WListFast<T>&;

    template <class T2 = T>
    T2 average () const noexcept;

    template <typename Func>
    nd bool anyMatch(Func func) const noexcept;

    class iterator
    {
    private:
        T *** _array;
        Size _index;
    public:
        iterator(T ***data, Size index) noexcept : _array(data), _index(index) {; };

        T* operator->()         { return (*_array)[_index]; };
        T &operator*() const    { return *(*_array)[_index]; };
        constexpr bool operator==(iterator i) const         { return _index == i._index; }
        constexpr bool operator!=(iterator i) const         { return _index != i._index || _array != i._array; }
        iterator &operator++()                              { _index ++; return *this; }
        iterator operator++(int) { auto copy = *this; ++*this; return copy; }
        iterator operator+(Size i) const { return iterator(_array, _index + i); }
        iterator operator-(Size i) const { return iterator(_array, _index - i); };

        auto operator-(const iterator &other) const -> int { return _index - other._index; }

        explicit operator Size () const { return this->_index; }
    };

    class const_iterator
    {
    private:
        const T *const * const *_array;
        Size _index;
    public:
        const_iterator(const T * const * const *data, Size index) noexcept : _array(data), _index(index) { W_ASSERT(index >= 0); };

        const T* operator->() const   { return (*_array)[_index]; };
        const T &operator*() const    { return *(*_array)[_index]; };

        constexpr auto operator==(const_iterator i) const -> bool         { return _index == i._index && _array == i._array; }
        constexpr auto operator!=(const_iterator i) const -> bool         { return _index != i._index && _array == i._array; }
        auto operator++() -> const_iterator &                             { _index ++; return *this; }
        auto operator++(int) -> const_iterator { auto copy = *this; ++*this; return copy; }

        auto operator-(const const_iterator &other) const -> int { return _index - other._index; }

        const_iterator operator+(Size i) const { return const_iterator(_array, _index + i); }
        const_iterator operator-(Size i) const { return const_iterator(_array, _index - i); };

        explicit operator Size () const { return _index; }
    };

    template <class Func>
    auto equals (const WListFast<T>& other, Func method) const noexcept -> bool;

    nd auto begin() noexcept { return iterator(&_data, 0); };
    nd auto end()   noexcept { return iterator(&_data, size()); };

    nd auto begin() const noexcept { return const_iterator(&_data, 0); };
    nd auto end()   const noexcept { return const_iterator(&_data, size()); };

    auto constBegin() const noexcept { return const_iterator(&_data, 0); }
    auto constEnd()   const noexcept { return const_iterator(&_data, size()); }

    /**
     * \param writable needs to have write(const void *data, size_t size) and it needs to return < 0 in case
     *  of failure and it needs to have write(const T &param) for non class object
     *
     * \return &lt 0 in case of error
     * */
    template <class T2 = T>
    static
    auto write(WritableAbstract &writable, const WListFast<T2> &list) noexcept -> Error;

    /**
     * To load the data save with this method it's required to call WListFast::loadMultiThread
     * \param list The list to save
     * \param startNewThread Function that receive a Fn<void()> and return the WTask * associated
     * \param writable Writable
     * \return &lt 0 in case of error
     * */
    template <class T2 = T>
    static
    auto writeMultiThread (
            WritableAbstract &writable,
            const WListFast<T2> &list,
            const auto &startNewThread
    ) noexcept -> Error;

    template<class T2 = T>
    static auto loadMultiThread (const VersionFileController &versionController,
                                 ReadableAbstract &readable,
                                 const auto &startNewThread
                        ) noexcept -> WPair<Error, WListFast<T2>>;

    static auto load  (
                const VersionFileController &versionController,
                ReadableAbstract &readable,
                Fn<WPair<Error, T>(const VersionFileController &versionController, ReadableAbstract &readable )> func
            ) noexcept -> WPair<Error, WListFast<T>>;

    /**
     * You can use this method only if <T> has the method load
     * \return < 0 if error
     * */
    static auto load(const VersionFileController &versionController,
                      ReadableAbstract &file) noexcept -> WPair<Error, WListFast<T>>;

    static auto write (WritableAbstract &writable, const WListFast<T> &list,
                       Fn<Error(WritableAbstract &writable, const T&)> save) noexcept-> Error;
};

template <class T>
auto WListFast<T>::filter(Fn<bool(const T&)> method) const -> WAbstractList::FilterList<WListFast, const T>
{
    return WAbstractList::FilterList<WListFast, const T>(*this, method);
}

template <class T>
auto WListFast<T>::filter(Fn<bool(const T&)> method) -> WAbstractList::FilterList<WListFast, T>
{
    return WAbstractList::FilterList<WListFast, T>(*this, method);
}

template <class T>
auto WListFast<T>::refMid(int from, int to) noexcept -> WAbstractList::SplitList<WListFast, T, Size>
{
    return WAbstractList::SplitList<WListFast, T, Size>(*this, from, to);
}

template <class T>
auto WListFast<T>::refMid(int from, int to) const noexcept -> WAbstractList::SplitList<WListFast, const T, Size>
{
    return WAbstractList::SplitList<WListFast, const T, Size>(*this, from, to);
}

template <class T>
template <class T2>
inline T2 WListFast<T>::average () const noexcept
{
    T2 result = T2(0);

    for (const auto &ref: std::as_const(*this)) {
        result += ref;
    }

    return result / T2(size());
}

template<class T>
inline WListFast<T>::WListFast(Size reserve) noexcept
    : _data(nullptr)
    , _size(0)
    , _reserved(0)
{
    this->reserve(reserve);
}

template <class T>
inline void WListFast<T>::forAll(Fn<void(const T &)> method) const noexcept
{
    for (const auto &ref: std::as_const(*this)) {
        method(ref);
    }
}

template <class T>
inline void WListFast<T>::forAll(Fn<void(T &)> method) noexcept
{
    for (auto &ref: *this) {
        method(ref);
    }
}

template <class T>
void WListFast<T>::append(WListFast<T> &&other) noexcept
{
    W_ASSERT_TEXT(&other != this, "Wrong call to append(WListFast&&)");

    if (other.isEmpty())
        return;

    if (_reserved < other.size())
        reserve(WAbstractList::numberOfAllocationNeeded(other.size(), WListFast::numberOfObjectReserved));

    WUtils::WMemcpy(&_data[size()], other._data, sizeof (T*) * other.size());

    this->_reserved -= other.size();
    this->_size += other.size();

    other._data = nullptr;
    other._reserved = 0;
    other._size = 0;
}


template <class T>
inline auto WListFast<T>::load(
        const VersionFileController &versionController,
        ReadableAbstract &readable,
        Fn<WPair<Error, T>(
                const VersionFileController &versionController,
                ReadableAbstract &readable)
        > func
    ) noexcept -> WPair<Error, WListFast<T>>
{
    return WAbstractList::load<WListFast, T>(versionController, readable, func);
}

template <class T>
inline auto WListFast<T>::write(
            WritableAbstract &writable,
            const WListFast<T> &list,
            Fn<Error(WritableAbstract &writable, const T&)> save
        ) noexcept -> Error
{
    return WAbstractList::write(writable, list, save);
}

template <class T>
template <class T2>
inline auto WListFast<T>::write(WritableAbstract &writable, const WListFast<T2> &list) noexcept -> Error
{
    return WAbstractList::write(writable, list);
}

template<class T>
template<class T2>
inline auto WListFast<T>::writeMultiThread(
            WritableAbstract &writable,
            const WListFast<T2> &list,
            const auto &startNewThread
        ) noexcept -> Error
{
    return WAbstractList::writeMultiThread<WListFast, T2>(writable, list, startNewThread);
}


template<class T>
template<class T2>
auto WListFast<T>::loadMultiThread(
            const VersionFileController &versionController,
            ReadableAbstract &readable,
            const auto &startNewThread
        ) noexcept -> WPair<Error, WListFast<T2>>
{
    auto reserveUnsafe = [] (auto& list, auto numberOfElements) {
        list._data = (T2**) malloc(sizeof(T*) * numberOfElements);
        list._size = numberOfElements;
        list._reserved = 0u;
    };

    auto insertUnsafe = [] (auto& list, auto &&object, auto index) {
        list._data[index] = new T2(std::forward<T2>(object));
    };

    return WAbstractList::loadMultiThread<WListFast, T2>(versionController, readable, startNewThread,
                                                         reserveUnsafe, insertUnsafe);
}

template<class T>
inline auto WListFast<T>::operator=(WListFast<T> &&other) noexcept -> WListFast<T> &
{
    if (this == &other)
        return *this;

    for (Size i = size() - 1; i >= 0; i--) {
        delete this->_data[i];
    }

    free (this->_data);

    this->_data         = other._data;
    this->_size         = other._size;
    this->_reserved     = other._reserved;

    other._data = nullptr;
    other._size = 0;
    other._reserved = 0;

    return *this;
}

template<class T>
inline void WListFast<T>::removeObjects(const T &item) noexcept
{
    const auto index = indexOf(item);
    if (index >= 0) {
        removeAt(index);
    }
}

template<class T>
inline WListFast<T>::WListFast(const WListFast<T> &other) noexcept
    : _data(nullptr)
    , _size(0)
    , _reserved(0)
{
    append(other);
}

template<class T>
inline auto WListFast<T>::mid(Size from, Size to) const noexcept -> WListFast<T>
{
    W_ASSERT(from >= 0 && to <= size());
    W_ASSERT(from <= to);

    auto res = WListFast<T>();

    res.append(begin() + from, begin() + to);

    return res;
}

template <class T>
template <class Iter>
void WListFast<T>::append(Iter begin, Iter end)
{
    static_assert(std::is_convertible<decltype(*begin), T>::value);

    const Size numberOfAllocation = end - begin;

    if (_reserved < numberOfAllocation)
        reserve(WAbstractList::numberOfAllocationNeeded(numberOfAllocation, WListFast::numberOfObjectReserved));

    for (; begin != end; begin ++) {
        _data[_size] = new T(*begin);

        _size ++;
        _reserved --;
    }
}

template<class T>
inline auto WListFast<T>::indexOf(const T &value) const noexcept -> Size
{
    for (Size i = 0; i < size(); ++i)
        if (at(i) == value)
            return i;

    return -1;
}

template<class T>
inline void WListFast<T>::insert(Size index, const T &object) noexcept
{
    W_ASSERT(index >= 0 && index <= size());

    if (_reserved == 0)
        reserve(WListFast::numberOfObjectReserved);

    if (_reserved == 0) {
        T** newData = (T**) malloc (sizeof(T*) * (_size + WListFast::numberOfObjectReserved));

        WUtils::WMemcpy(newData, _data, sizeof(T*) * index);

        newData[index] = new T(object);

        for (Size i = index + 1; i < size() + 1; i++)
            newData[i] = _data[i - 1];

        free(_data);
    } else {
        for (Size i = size() + 1; i > index; i++)
            _data[i] = _data[i - 1];

        _data[index] = new T(object);
    }

    _size ++;
    _reserved --;

    rep();

    W_ASSERT(at(index) == object);
}

template<class T>
force_inline void WListFast<T>::rep() const noexcept
{
#ifdef DEBUGINFO
    W_ASSERT(this->_size >= 0);
    W_ASSERT(this->_reserved >= 0);

    if (_size > 0) {
        for (Size i = 0; i < size(); i++)
            W_ASSERT(_data[i] != nullptr);
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
            ) noexcept -> WPair<Error, WListFast<T>>
{
    return WAbstractList::load<WListFast, T>(versionController, file);
}

template<class T>
void WListFast<T>::move(Size from, Size to) noexcept
{
    W_ASSERT(from >= 0 and from < size());
    W_ASSERT(to >= 0 and to < size());

    if (from == to)
        return;

    auto *r = this->_data[from];
    _data[from] = _data[to];
    _data[to] = r;

    rep();
}

template<class T>
auto WListFast<T>::operator=(const WListFast<T> &other) noexcept -> WListFast<T> &
{
    if (this == &other)
        return *this;

    // TODO: optimize [also to use shared data between list]
    for (Size i = 0; i < size(); i++) {
        delete _data[i];
    }

    free(this->_data);
    this->_data = nullptr;

    this->_size = other.size();
    this->_reserved = 0;

    if (_size) {
        this->_data = (T**) malloc(sizeof (T*) * other.size());

        for (Size i = 0; i < size(); i++) {
            _data[i] = new T(other.at(i));
        }
    }

    rep();

    return *this;
}

template<class T>
inline bool WListFast<T>::operator==(const WListFast<T> &other) const noexcept
{
    if (size() != other.size())
        return false;

    for (Size i = 0; i < size(); i++) {
        if (at(i) != other.at(i))
            return false;
    }

    rep();

    return true;
}

template<class T>
auto WListFast<T>::operator[](Size i) noexcept -> T &
{
    W_ASSERT(inBound(i));
    return *this->_data[i];
}

template<class T>
inline T WListFast<T>::takeFirst() noexcept
{
    return takeAt(0);
}

template<class T>
inline auto WListFast<T>::lastIndexOf(const T &object) const noexcept -> Size
{
    for (Size i = size() - 1; i >= 0; i--) {
        if (at(i) == object)
            return i;
    }

    return -1;
}

template<class T>
inline const T &WListFast<T>::last() const noexcept
{
    W_ASSERT(size() != 0);
    return at(size() - 1);
}

template <class T>
inline auto WListFast<T>::takeObject(Size index) noexcept -> T
{
    W_ASSERT(inBound(index));
    T object(std::forward<T>(*this->_data[index]));

    delete _data[index];

    for (Size i = index; i < size() - 1; i++) {
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

    rep();
    return object;
}

template<class T>
inline auto WListFast<T>::takeAt(Size index) noexcept -> T
{
    W_ASSERT(inBound(index));
    T object(takeObject(index));

    rep();
    return object;
}

template<class T>
void WListFast<T>::reserve(Size numberOfElements) noexcept
{
    W_ASSERT(numberOfElements >= 0);

    if (numberOfElements) {
        this->_reserved += numberOfElements;
        this->_data = (T**) realloc(_data, sizeof (T*)  * (_size + _reserved));
    }

    rep();
}

template<class T>
inline void WListFast<T>::clear() noexcept
{
    for (Size i = 0; i < this->_size; i++) {
        delete _data[i];
    }

    free (_data);
    _data = nullptr;
    _size = 0;
    _reserved = 0;

    rep();
}

template<class T>
inline auto WListFast<T>::first() const noexcept -> const T &
{
    W_ASSERT(inBound(0));
    return *this->_data[0];
}

template<class T>
constexpr inline auto WListFast<T>::size() const noexcept -> Size
{
    return _size;
}

template<class T>
constexpr inline bool WListFast<T>::isEmpty() const noexcept
{
    return size() == Size(0);
}

template<class T>
inline void WListFast<T>::remove(Size from, Size to) noexcept
{
    W_ASSERT(from <= to);
    W_ASSERT(from >= 0 && to <= size());

    for (Size i = from; i < to; i++) {
        delete this->_data[i];
    }

#ifdef DEBUGINFO
    WUtils::WMemset(&_data[from], 0, sizeof(T*) * (to - from));
#endif

    WUtils::WMemmove(&_data[from], &_data[to], sizeof(T*) * (_size - to));

    if (this->_reserved == 0) {
        this->_data = (T **) realloc(this->_data, (this->_size - (to - from)) * sizeof(T *));
    } else {
        this->_reserved += (to - from);
    }

    this->_size -= (to - from);

    rep();
}

template<class T>
inline void WListFast<T>::removeAt(Size index) noexcept
{
    (void)this->takeObject(index);
    rep();
}

template<class T>
inline void WListFast<T>::append(const WListFast<T> &other) noexcept
{
    this->reserve(other.size());

    for (const auto &element: std::as_const(other)) {
        this->append(element);
    }

    rep();
}

template<class T>
inline auto WListFast<T>::append(const T &element) noexcept -> WListFast<T>&
{
    if (_reserved == 0)
        reserve(WListFast::numberOfObjectReserved);

    _size++;
    this->_data[_size - 1] = new T(element);
    _reserved --;

    rep();
    return *this;
}

template <class T>
inline auto WListFast<T>::append(T &&object) noexcept -> WListFast<T>&
{
    if (_reserved == 0)
        reserve(WListFast::numberOfObjectReserved);

    this->_data[_size] = new T(std::move(object));

    _size ++;
    _reserved --;
    return *this;
}

template<class T>
inline const T &WListFast<T>::at(Size i) const noexcept
{
    W_ASSERT(i >= 0 && i < this->size());
    return *this->_data[i];
}

template<class T>
inline WListFast<T>::WListFast(std::initializer_list<T> args) noexcept
    : _data(nullptr)
    , _size(0)
    , _reserved(0)
{
    this->reserve(args.size());
    for (auto &d: args) {
        this->append(d);
    }

    rep();
}

template<class T>
inline constexpr WListFast<T>::WListFast() noexcept
        : _data(nullptr)
    , _size(0)
    , _reserved(0)
{
    rep();
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

    auto begin = p.begin();
    auto end = p.end();

    for (; begin != end; begin++) {
        d.nospace() << *begin;
        if ((begin + 1) != end)
            d.nospace() << ", ";
    }

    return d.space() << " )";
}
#endif // USE_QT

template <class T>
inline WListFast<T>::~WListFast() noexcept
{
    this->clear();
}

template <class T>
inline std::ostream& operator<<(std::ostream& os, const WListFast<T>& dt)
{
    for (const auto &ref: dt) {
        os << ref;
    }
    return os;
}

template <class T>
template <class Func>
inline auto WListFast<T>::anyMatch(Func func) const noexcept -> bool
{
    for (const auto &value: *this) {
        if (func(value))
            return true;
    }
    return false;
}

template <class T>
constexpr inline auto WListFast<T>::inBound(long index) const noexcept -> bool
{
    return index >= 0 and index < size();
}

template <class T>
template <class Iter> requires(std::is_class<Iter>::value)
inline void WListFast<T>::removeAt(Iter begin, Iter end) noexcept
{
    if (begin == end)
        return;

    W_ASSERT(WAbstractList::isSorted(begin, end));
    W_ASSERT(*(end - 1) < size());
    W_ASSERT(*begin >= 0);

    const Size diff = end - begin;
    const auto hasRealloc = (_reserved + diff > WListFast::numberOfObjectReserved);
    T** to = hasRealloc
                ? (T**) malloc (sizeof(T*) * (_size - diff + WListFast::numberOfObjectReserved))
                : _data;

    Size i = *begin;

    /**
     * number of elements that is already deleted from the original data structure
     * */
    Size numberOfDeletion = 0u;

    if (hasRealloc)
        memmove(to, static_cast<const void*>(_data), (*begin) * sizeof(T*));

    for (;;) {
        const auto isAtEnd = (begin + 1) == end;

        numberOfDeletion ++;
        delete _data[i];
        _data[i] = nullptr;

        const Size s = isAtEnd ? size() : *(begin + 1);

        // TODO: use memcopy
        for (i++; i < s; i++) {
            W_ASSERT(i < size());
            W_ASSERT(i - numberOfDeletion < size() - diff);
            
            to[i - numberOfDeletion] = _data[i];
        }

        if (isAtEnd)
            break;

        begin++;
    }

    if (hasRealloc) {
        _reserved = WListFast::numberOfObjectReserved;
        free(_data);
        _data = to;
    } else {
        _reserved += diff;
    }
    _size -= diff;

    rep();
}

template <class T>
template <class Func>
auto WListFast<T>::equals (const WListFast<T>& other, Func method) const noexcept -> bool
{
    if (size() != other.size())
        return false;

    auto b = other.begin();

    for (const auto& ref: std::as_const(*this)) {
        if (!method(ref, *b))
            return false;
        b++;
    }

    return true;
}
