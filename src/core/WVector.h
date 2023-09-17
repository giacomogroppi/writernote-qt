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
    WVector(std::initializer_list<T> &&object) noexcept;
    ~WVector();

    using const_iterator = typename std::vector<T>::const_iterator;
    using iterator = typename std::vector<T>::iterator;

    using riterator = typename std::vector<T>::reverse_iterator;
    using rconst_iterator = typename std::vector<T>::const_reverse_iterator;

    void append(const WVector<T> &other);

    void append(const T &item);
    void append(T &&item);

    const T& get(int i) const;
    int size() const;

    template <class T2 = T>
    bool isOrderLowToHigh() const;

    template <class T2 = T>
    bool isOrderHighToLow() const;



    /**
     * Remove all the object [from, to)
     * */
    void removeAll(int from, int to);
    void removeAt(int i);
    void move(int from, int to);
    void clear();
    const T& at(int i) const;
    void reserve(int numberOfElement);
    T takeAt(int i);
    auto last() const ->  const T&;
    auto first() const -> const T&;
    auto operator[](int index) -> T&;
    auto isEmpty() const -> bool;

    void insert(int index, T &&data);
    void insert(int index, const T& data);

    auto takeFirst() -> T;
    auto constData() const -> const T*;
    auto indexOf(const T &object) -> int;
    auto remove(const T& object) -> bool;


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

    auto rbegin() -> riterator;
    auto rend() -> riterator ;

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
    template <class Writable, class T2 = T> requires (std::is_base_of_v<WritableAbstract, Writable>)
    static
    auto save(Writable &writable, const WVector<T2> &list) noexcept -> int
    {
        const int size = list.size();
        static_assert_type(size, const int);

        if (writable.write(&size, sizeof (size)) < 0) {
            return -1;
        }

        for (const auto &ref: std::as_const(list)) {
            if (T2::write(writable, ref) < 0)
                return -1;
        }
        return 0;
    }

    template <class StartNewThreadFunction, class T2 = T>
    static
    auto writeMultiThread (WritableAbstract &writable, const WVector<T2> &list, StartNewThreadFunction startNewThread) noexcept -> int
    {
        int i = 0;
        static_assert_type(list._size, int);

        WVector<WTask> threads;
        MemWritable w[list._size];
        volatile bool result = false;

        threads.reserve(list.size());

        for (const auto &ref: std::as_const(list)) {
            threads.append(startNewThread ([ref, &result, &w, i]() {
                                               if (T2::write (w[i], ref) < 0)
                                                   result = true;
                                           }
            ));
            i++;
        }

        for (auto &thread: threads) {
            thread.join();
        }

        // implement write stack and write data to original writer
        if (writable.write(list._size) < 0)
            return -1;

        for (i = 0; i < list.size(); i++) {
            size_t s = w[i].getCurrentSize();
            if (writable.write(&s, sizeof(s)) < 0)
                return -1;
        }

        for (i = 0; i < list.size(); i++) {
            // optimize this copy
            if (w[i].merge(writable) < 0)
                return -1;
        }

        return 0;
    }
};

template<class T>
auto WVector<T>::removeOrderHighToLow(
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
    W_ASSERT(from >= 0 and from < to and to <= size());
    _data.erase(begin() + from, begin() + to);
}

template<class T>
inline WVector<T> &WVector<T>::operator=(const WVector<T> &other)
{
    _data = other._data;
    return *this;
}

template <class T>
auto WVector<T>::operator=(WVector<T> &&other) noexcept -> WVector<T> &
{
    _data = std::move(other._data);
    return *this;
}

template<class T>
auto WVector<T>::takeAt(int i) -> T
{
    W_ASSERT(i >= 0 && i < size());
    auto iterator = this->_data.begin() + i;

    T element = std::move(_data[i]);

    _data.erase(iterator);

    return element;
}

template<class T>
void WVector<T>::insert(int index, T &&data)
{
    this->_data.insert(
            _data.begin() + index,
            std::move(data)
    );
}

template<class T>
void WVector<T>::insert(int index, const T &data)
{
    this->_data.insert(
            _data.begin() + index,
            data
    );
}

template<class T>
auto WVector<T>::takeFirst() -> T
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
    _data.clear();
    /*
    free(_data);
    _data = nullptr;
    _size = 0;
    _reserve = 0;
     */
}

template<class T>
auto WVector<T>::isEmpty() const -> bool
{
    return size() == 0;
}

template<class T>
inline auto WVector<T>::constData() const -> const T *
{
    return this->_data.data();
}

template<class T>
inline auto WVector<T>::indexOf(const T &object) -> int
{
    const auto pos = std::find(_data.cbegin(), _data.cend(), object);
    if (pos == _data.cend())
        return -1;
    return pos - _data.cbegin();
}

template<class T>
inline auto WVector<T>::operator==(const WVector<T> &other) const -> bool
{
    return this->_data == other._data;
}

template<class T>
inline auto WVector<T>::operator!=(const WVector<T> &other) const -> bool
{
    return this->_data != other._data;
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
    return _data.size();
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
WVector<T>::WVector(std::initializer_list<T> &&object) noexcept
    : _data(std::move(object))
{

}

template <class T>
WVector<T>::WVector(WVector<T> &&other) noexcept
    : _data(std::move(other._data))
{
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
    // TODO: adjust
    WPair<int, WVector<T>> result(-1, WVector<T>());

    switch (versionController.getVersionWListFast()) {
        case 0:
            int i, element;

            if (file.read(element) < 0) {
                return result;
            }

            result.second.reserve(element);

            for (i = 0; i < element; i++) {
                const auto [res, object] = T::load (versionController, file);

                if (res < 0)
                    return result;

                result.second.append(
                        std::move (object)
                );
            }
            result.first = 0;
            return result;
    }

    result.first = -1;
    return result;
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
auto WVector<T>::rend() -> riterator
{
    return this->_data.rend();
}

template <class T>
auto WVector<T>::rbegin() -> riterator
{
    return this->_data.rbegin();
}

template <class T>
auto WVector<T>::remove(const T &object) -> bool
{
    return std::erase_if(_data, [&](const T& item) { return item == object; }) != 0;
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
