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
    static constexpr auto debug = false;
    void test() const;

    static_assert(std::is_move_constructible<T>::value);

    using Size = int;

    T **_data;
    Size _size;
    Size _reserved;

    static constexpr Size numberOfObjectReserved = 128;

    // this function remove the object from the list
    T *takeObject(int i) noexcept;

public:
    constexpr WListFast() noexcept;

    /**
     * \param reserve The reserve number of element
     * */
    explicit WListFast(int reserve) noexcept;

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
    [[nodiscard]]
    auto at(int i) const noexcept -> const T&;

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
    void append(const WListFast<T> &other) noexcept;

    template <class Iter>
    void removeAt(Iter begin, Iter end) noexcept;
    void removeAt(int index) noexcept;
    void remove(int from, int to) noexcept;
    void removeObjects(const T &item) noexcept;

    nd constexpr
    auto isEmpty() const noexcept -> bool;

    nd constexpr
    auto size() const noexcept -> int;

    nd constexpr
    auto inBound(long index) const noexcept -> bool;

    [[nodiscard]]
    auto first() const noexcept -> const T&;

    void clear() noexcept;
    void move(int from, int to) noexcept;
    void reserve(int reserve) noexcept;
    auto takeAt(int i) noexcept -> T;
    auto last() const noexcept -> const T&;
    auto indexOf(const T& value) const noexcept -> int;
    auto lastIndexOf(const T& object) const noexcept -> int;
    void insert(int index, const T& object) noexcept;
    auto takeFirst() noexcept -> T;

    void forAll(Fn<void(const T&)> method) const noexcept;
    void forAll(Fn<void(T&)> method) noexcept;

    [[nodiscard]]
    auto mid(int from, int to) const noexcept -> WListFast<T>;

    auto operator[](int i) noexcept -> T&;
    auto operator[](int i) const noexcept -> const T&;

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
        int _index;
    public:
        iterator(T ***data, int index) noexcept : _array(data), _index(index) {; };

        T* operator->()         { return (*_array)[_index]; };
        T &operator*() const    { return *(*_array)[_index]; };
        constexpr bool operator==(iterator i) const         { return _index == i._index; }
        constexpr bool operator!=(iterator i) const         { return _index != i._index || _array != i._array; }
        iterator &operator++()                              { _index ++; return *this; }
        iterator operator++(int) { auto copy = *this; ++*this; return copy; }
        iterator operator+(int i) const { return iterator(_array, _index + i); }
        iterator operator-(int i) const { return iterator(_array, _index - i); };

        explicit operator int () const { return this->_index; }
    };

    class const_iterator
    {
    private:
        const T ***_array;
        int _index;
    public:
        const_iterator(const T ***data, int index) noexcept : _array(data), _index(index) { W_ASSERT(index >= 0); };
        const_iterator(const const_iterator &other) : _array(other._array), _index(other._index) {}

        const T* operator->() const   { return (*_array)[_index]; };
        const T &operator*() const    { return *(*_array)[_index]; };

        auto operator=(const const_iterator &other) -> const_iterator & = default;

        constexpr auto operator==(const_iterator i) const -> bool         { return _index == i._index && _array == i._array; }
        constexpr auto operator!=(const_iterator i) const -> bool         { return _index != i._index && _array == i._array; }
        auto operator++() -> const_iterator &                             { _index ++; return *this; }
        auto operator++(int) -> const_iterator { auto copy = *this; ++*this; return copy; }
        
        const_iterator operator+(int i) const { return const_iterator(_array, _index + i); }
        const_iterator operator-(int i) const { return const_iterator(_array, _index - i); };

        explicit operator int () const { return _index; }
    };

    [[nodiscard]]
    auto begin() noexcept -> iterator { return iterator((T ***)&_data, 0); };

    [[nodiscard]]
    auto end()   noexcept -> iterator { return iterator((T ***)&_data, size());  };

    auto constBegin() const noexcept -> const_iterator { return const_iterator((const T ***)&_data, 0); }
    auto constEnd()   const noexcept -> const_iterator { return const_iterator((const T ***)&_data, size()); }
    auto cBegin() const noexcept -> const_iterator { return const_iterator((const T ***)&_data, 0); }
    auto cEnd()   const noexcept -> const_iterator { return const_iterator((const T ***)&_data, size()); }
    auto begin() const noexcept -> const_iterator { return const_iterator((const T ***)&_data, 0); }
    auto end()   const noexcept -> const_iterator { return const_iterator((const T ***)&_data, size()); }

    /**
     * \param writable needs to have write(const void *data, size_t size) and it needs to return < 0 in case
     *  of failure and it needs to have write(const T &param) for non class object
     *
     * \return &lt 0 in case of error
     * */
    template <class T2 = T>
    static
    auto write(WritableAbstract &writable, const WListFast<T2> &list) noexcept -> int;

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
    ) noexcept -> int;

    template<class T2 = T>
    static auto loadMultiThread (const VersionFileController &versionController,
                                 ReadableAbstract &readable,
                                 const auto &startNewThread
                        ) noexcept -> WPair<int, WListFast<T2>>;

    static auto load  (
                const VersionFileController &versionController,
                ReadableAbstract &readable,
                Fn<WPair<int, T>(const VersionFileController &versionController, ReadableAbstract &readable )> func
            ) noexcept -> WPair<int, WListFast<T>>;

    /**
     * You can use this method only if <T> has the method load
     * \return < 0 if error
     * */
    static auto load (const VersionFileController &versionController,
                      ReadableAbstract &file) noexcept -> WPair<int, WListFast<T>>;

    static auto write (WritableAbstract &writable, const WListFast<T> &list,
                       Fn<int(WritableAbstract &writable, const T&)> save) noexcept-> int;
};

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
inline WListFast<T>::WListFast(int reserve) noexcept
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
inline auto WListFast<T>::load(
        const VersionFileController &versionController,
        ReadableAbstract &readable,
        Fn<WPair<int, T>(
                const VersionFileController &versionController,
                ReadableAbstract &readable)
        > func
    ) noexcept -> WPair<int, WListFast<T>>
{
    return WAbstractList::load<WListFast, T>(versionController, readable, func);
}

template <class T>
inline auto WListFast<T>::write(
            WritableAbstract &writable,
            const WListFast<T> &list,
            Fn<int(WritableAbstract &writable, const T&)> save
        ) noexcept -> int
{
    return WAbstractList::write(writable, list, save);
}

template <class T>
template <class T2>
inline auto WListFast<T>::write(WritableAbstract &writable, const WListFast<T2> &list) noexcept -> int
{
    return WAbstractList::write(writable, list);
}

template<class T>
template<class T2>
inline auto WListFast<T>::writeMultiThread(
            WritableAbstract &writable,
            const WListFast<T2> &list,
            const auto &startNewThread
        ) noexcept -> int
{
    return WAbstractList::writeMultiThread<WListFast, T2>(writable, list, startNewThread);
}


template<class T>
template<class T2>
inline auto WListFast<T>::loadMultiThread(
            const VersionFileController &versionController,
            ReadableAbstract &readable,
            const auto &startNewThread
        ) noexcept -> WPair<int, WListFast<T2>>
{
    auto reserveUnsafe = [] (WListFast<T2>& list, int numberOfElements) {
        list._data = (T2**) malloc(sizeof(T*) * numberOfElements);
        list._size = numberOfElements;
        list._reserved = 0u;
    };

    auto insertUnsafe = [] (WListFast<T2>& list, T2 &&object, int index) {
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

    for (int i = size() - 1; i >= 0; i--) {
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
    this->reserve(other.size());
    this->append(other);
}

template<class T>
inline auto WListFast<T>::mid(int from, int to) const noexcept -> WListFast<T>
{
    W_ASSERT(from >= 0 && to <= size());
    W_ASSERT(from <= to);

    auto res = WListFast<T>();

    res.reserve(to - from);
    for (int i = from; i < to; i++) {
        res.append(at(i));
    }

    return res;
}

template<class T>
inline auto WListFast<T>::indexOf(const T &value) const noexcept -> int
{
    for (int i = 0; i < size(); ++i) {
        if (at(i) == value)
            return i;
    }

    return -1;
}

template<class T>
inline void WListFast<T>::insert(int index, const T &object) noexcept
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
        for (int i = 0; i < size(); i++)
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
            ) noexcept -> WPair<int, WListFast<T>>
{
    return WAbstractList::load<WListFast, T>(versionController, file);
}

template<class T>
void WListFast<T>::move(int from, int to) noexcept
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
auto WListFast<T>::operator=(const WListFast<T> &other) noexcept -> WListFast<T> &
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
inline bool WListFast<T>::operator==(const WListFast<T> &other) const noexcept
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
inline T &WListFast<T>::operator[](int i) noexcept
{
    return *this->_data[i];
}

template <class T>
inline auto WListFast<T>::operator[](int i) const noexcept -> const T&
{
    return *this->_data[i];
}

template<class T>
inline T WListFast<T>::takeFirst() noexcept
{
    return takeAt(0);
}

template<class T>
inline int WListFast<T>::lastIndexOf(const T &object) const noexcept
{
    for (int i = size() - 1; i >= 0; i--) {
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
inline T* WListFast<T>::takeObject(int index) noexcept
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
inline auto WListFast<T>::takeAt(int index) noexcept -> T
{
    W_ASSERT(index >= 0 && index < size());
    T* p = takeObject(index);
    T copy = T(*p);

    delete p;

    test();
    return copy;
}

template<class T>
inline void WListFast<T>::reserve(int numberOfObjects) noexcept
{
    W_ASSERT(numberOfObjects >= 0);

    if (numberOfObjects) {
        this->_reserved += numberOfObjects;
        this->_data = (T**) realloc(_data, sizeof (T*)  * (_size + _reserved));
    }

    test();
}

template<class T>
inline void WListFast<T>::clear() noexcept
{
    for (int i = 0; i < this->_size; i++) {
        delete _data[i];
    }

    free (_data);
    _data = nullptr;
    _size = 0;
    _reserved = 0;

    test();
}

template<class T>
inline auto WListFast<T>::first() const noexcept -> const T &
{
    return *this->_data[0];
}

template<class T>
constexpr inline auto WListFast<T>::size() const noexcept -> int
{
    return _size;
}

template<class T>
constexpr inline bool WListFast<T>::isEmpty() const noexcept
{
    return _size == 0;
}

template<class T>
inline void WListFast<T>::remove(int from, int to) noexcept
{
    W_ASSERT(from <= to);
    W_ASSERT(from >= 0 && to <= size());

    for (int i = from; i < to; i++) {
        delete this->_data[i];
    }

#ifdef DEBUGINFO
    WCommonScript::WMemset(&_data[from], 0, sizeof(T*) * (to - from));
#endif

    WCommonScript::WMemmove(&_data[from], &_data[to], sizeof(T*) * (_size - to));

    if (this->_reserved == 0) {
        this->_data = (T **) realloc(this->_data, (this->_size - (to - from)) * sizeof(T *));
    } else {
        this->_reserved += (to - from);
    }

    this->_size -= (to - from);

    test();
}

template<class T>
inline void WListFast<T>::removeAt(int index) noexcept 
{
    delete this->takeObject(index);
    test();
}

template<class T>
inline void WListFast<T>::append(const WListFast<T> &other) noexcept
{
    this->reserve(other.size());

    for (const auto &element: std::as_const(other)) {
        this->append(element);
    }

    test();
}

template<class T>
inline auto WListFast<T>::append(const T &element) noexcept -> WListFast<T>&
{
    if (_reserved == 0)
        reserve(WListFast::numberOfObjectReserved);

    _size++;
    this->_data[_size - 1] = new T(element);
    _reserved --;

    test();
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
inline const T &WListFast<T>::at(int i) const noexcept
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

    test();
}

template<class T>
inline constexpr WListFast<T>::WListFast() noexcept
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
template <class Iter>
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

    if (hasRealloc) {
        const Size s = *begin * sizeof(T*);
        memmove(to, static_cast<const void*>(_data), s);
        W_ASSERT(memcmp(to, _data,  s) == 0);
    }

    for (;;) {
        const auto isAtEnd = (begin + 1) == end;

        WDebug(true, "Current is:" << *begin
                << "Deliting object" << i
                << "next is: " << (((begin + 1) >= end) ? -1 : *(begin + 1))
        );

        numberOfDeletion ++;
        delete _data[i];
        _data[i] = nullptr;

        const Size s = isAtEnd ? size() : *(begin + 1);

        // TODO: use memcopy
        for (i++; i < s; i++) {
            W_ASSERT(i < size());
            W_ASSERT(i - numberOfDeletion < size() - diff);
            WDebug(true, "Move" << i << "to" << i - numberOfDeletion);
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

    test();
}
