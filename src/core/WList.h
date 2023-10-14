#pragma once

#include "utils/WCommonScript.h"
#include "testing/memtest.h"

template <class T>
struct WListPrivate{
    struct WListPrivate *next;
    T *data;
};

template <class T>
class WList{
private:
    WListPrivate<T> *_first;
    WListPrivate<T> *_last;
    int _size;

    void test() const noexcept;

public:
    WList() noexcept;
    WList(WList<T> &&other) noexcept;
    WList(const WList<T> &l) noexcept;
    ~WList() noexcept;

    void append(const T &data) noexcept;
    void append(T &&data) noexcept;

    void clear() noexcept;
    bool equal(const WList<T> &l1, const WList<T> &l2) noexcept;

    [[nodiscard]] bool isEmpty() const noexcept;
    [[nodiscard]] constexpr int size() const noexcept;
    const T& last() const;
    const T& first() const;
    constexpr int indexOf(const T& index) const;
    constexpr bool isOrder() const;

    /**
     * This function remove and return the first object of the list
     * @return First object in the list
    */
    T takeFirst() noexcept;

    class iterator{
    private:
        WListPrivate<T> *_e;
    public:
        explicit iterator(WListPrivate<T> *e) { _e = e; };

        T* operator->()         { return _e->data; };
        T &operator*() const    { return *_e->data; };
        constexpr bool operator==(iterator i) const         { return _e == i._e; }
        constexpr bool operator!=(iterator i) const         { return _e != i._e; }
        iterator &operator++()                              { _e = _e->next; return *this; }
        iterator operator++(int) { auto copy = *this; ++*this; return copy; }
    };

    class const_iterator{
    private:
        const WListPrivate<T> *_e;
    public:
        explicit const_iterator(const WListPrivate<T> *e) { _e = e; };

        const T* operator->() const   { W_ASSERT(_e); return _e->data; };
        const T &operator*() const    { W_ASSERT(_e); return *_e->data; };
        constexpr bool operator==(const_iterator i) const         { return _e == i._e; }
        constexpr bool operator!=(const_iterator i) const         { return _e != i._e; }
        const_iterator &operator++()                              { _e = _e->next; return *this; }
        const_iterator operator++(int) { auto copy = *this; ++*this; return copy; }
    };

    iterator begin() noexcept { test(); return iterator(_first); };
    iterator end()   noexcept { test(); return iterator(nullptr);  };

    const_iterator constBegin() const noexcept { test(); return const_iterator(_first); }
    const_iterator constEnd()   const noexcept { test(); return const_iterator(nullptr); }
    const_iterator cBegin() const noexcept { test(); return const_iterator(_first); }
    const_iterator cEnd()   const noexcept { test(); return const_iterator(nullptr); }
    const_iterator begin() const noexcept { test(); return const_iterator(_first); }
    const_iterator end()   const noexcept { test(); return const_iterator(nullptr); }

    WList<T> &operator=(WList<T> &&other) noexcept;
    WList<T> &operator=(const WList<T> &other);
    bool operator==(const WList<T> &other);
};

template<class T>
constexpr bool WList<T>::isOrder() const
{
    return false;
}

template<class T>
inline WList<T> &WList<T>::operator=(WList<T> &&other) noexcept
{
    if (this == &other)
        return *this;

    WListPrivate<T> *next = _first;

    while (next != nullptr) {
        auto *n = next->next;
        delete next->data;
        delete next;
        next = n;
    }

    this->_first = other._first;
    this->_last = other._last;
    this->_size = other._size;

    other._first = nullptr;
    other._last = nullptr;
    other._size = 0;

    return *this;
}

template<class T>
inline T WList<T>::takeFirst() noexcept
{
    W_ASSERT(size() > 0);

    T res(std::forward<T>(*this->_first->data));

    auto *nextFirst = _first->next;

    delete _first->data;
    delete _first;

    this->_first = nextFirst;
    this->_size --;

    if (_size == 0)
        _last = nullptr;

    return std::move(res);
}

template<class T>
inline bool WList<T>::operator==(const WList<T> &other)
{
    return WList<T>::equal(*this, other);
}

template<class T>
inline WList<T>::WList() noexcept
    : _first(nullptr)
    , _last(nullptr)
    , _size(0)
{
}

template<class T>
inline WList<T>::WList(WList<T> &&other) noexcept
    : _first(other._first)
    , _last(other._last)
    , _size(other._size)
{
    other._first = nullptr;
    other._last = nullptr;
    other._size = 0;
}

template <class T>
inline WList<T>::~WList() noexcept
{
    this->clear();
}

template <class T>
inline void WList<T>::append(T &&data) noexcept
{
    auto *newNode = new struct WListPrivate<T>();
    newNode->data = new T(std::forward<T>(data));
    newNode->next = nullptr;

    if (isEmpty()) {
        this->_first = newNode;
        this->_first->next = nullptr;
        this->_last = this->_first;
    } else {
        this->_last->next = newNode;
        this->_last = _last->next;
    }

    _size ++;

    test();
}

template <class T>
inline void WList<T>::append(const T &data) noexcept
{
    auto *newNode = new struct WListPrivate<T>();

    newNode->data = new T(data);
    newNode->next = nullptr;

    if (isEmpty()) {
        this->_first = newNode;
        this->_last = this->_first;
    } else {
        this->_last->next = newNode;
        this->_last = _last->next;
    }

    _size ++;

    test();
}

template<class T>
inline WList<T>::WList(const WList<T> &l) noexcept
    :_first(nullptr)
    , _last(nullptr)
    , _size(0u)
{
    *this = l;
    test();
}

template<class T>
inline void WList<T>::clear() noexcept
{
    while (_first) {
        auto *next = _first->next;
        delete _first->data;
        delete _first;
        _first = next;
    }

    _last = nullptr;
    _size = 0;
    test();
}

template<class T>
inline bool WList<T>::equal(const WList<T> &l1, const WList<T> &l2) noexcept
{
    WListPrivate<T> *f1, *f2;

    if (l1._size != l2._size)
        return false;

    l1.test();
    l2.test();

    f1 = l1._first;
    f2 = l2._first;

    while(f1){
        W_ASSERT(f2);

        if(*(f2->data) != *(f1->data))
            return false;

        f2 = f2->next;
        f1 = f1->next;
    }

    W_ASSERT(f1 == f2);

    return true;
}

template <class T>
inline bool WList<T>::isEmpty() const noexcept
{
    test();
    return this->_size == 0;
}

template<class T>
inline constexpr int WList<T>::size() const noexcept
{
    test();
    return this->_size;
}

template<class T>
inline const T &WList<T>::last() const
{
    test();
    W_ASSERT(this->_size != 0 and _last != nullptr);
    return *_last->data;
}

template<class T>
inline const T &WList<T>::first() const
{
    test();
    return *_first->data;
}

template<class T>
WList<T> &WList<T>::operator=(const WList<T> &other)
{
    if (this == &other)
        return *this;

    WListPrivate<T> *tmp, *curr;

    other.test();

    tmp = other._first;
    curr = _first;

    while(tmp and curr){
        W_ASSERT(tmp->data);
        *curr->data = *tmp->data;

        tmp = tmp->next;
        curr = curr->next;
    }

    W_ASSERT((not tmp) or (not curr));

    if((not tmp) and (not curr)){
        W_ASSERT(this->_size == other._size);
        goto out;
    }

    /**
     * we need to allocate more memory
     * */
    if(tmp and (not curr)){
        while(tmp){
            this->append(*(tmp->data));
            tmp = tmp->next;
        }
    }else{
        /**
         * We need to dealloc the extra memory
         * */

        W_ASSERT(not tmp and curr);

        while(curr){
            auto *next = curr->next;
            delete curr->data;
            delete curr;
            curr = next;
        }
    }

    out:
    _size = other._size;

    if (not _size) {
        this->_first = nullptr;
        this->_last = nullptr;
    }

    W_ASSERT(WList<T>::equal(*this, other));

    test();
    other.test();

    return *this;
}

template<class T>
void WList<T>::test() const noexcept
{
#ifdef DEBUGINFO
    int i = 0;
    WListPrivate<T> *node = _first, *last;

    if(!_first){
        W_ASSERT(!_last);
        W_ASSERT(_size == 0);
        return;
    }

    last = node;
    while(node){
        node = node->next;
        if(node)
            last = node;
        i++;
    }

    W_ASSERT(last->next == nullptr);
    W_ASSERT(last == _last);
    W_ASSERT(i == _size);
#endif // DEBUGINFO
}
