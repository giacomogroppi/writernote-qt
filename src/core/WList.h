#pragma once

#include "utils/common_script.h"
#include "testing/memtest.h"
#include <QList>

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
public:
    WList();
    WList(const WList<T> &l);
    ~WList();


    void append(const T &data) noexcept;
    void clear();
    bool equal(const WList<T> &l1, const WList<T> &l2);

    /**
     * Warn: You have to dealloc the object after.
     * */
    [[nodiscard]] T* get_first();

    class Iterator{
    private:
        WListPrivate<T> *_e;
    public:
        explicit Iterator(WListPrivate<T> *e) { _e = e; };

        T* operator->()         { return _e->data; };
        T &operator*() const    { return *_e->data; };
        constexpr bool operator==(Iterator i) const         { return _e == i._e; }
        constexpr bool operator!=(Iterator i) const         { return _e != i._e; }
        Iterator &operator++()                              { _e = _e->next; return *this; }
    };

    class ConstIterator{
    private:
        const WListPrivate<T> *_e;
    public:
        explicit ConstIterator(const WListPrivate<T> *e) { _e = e; };

        const T* operator->()         { return _e->data; };
        const T &operator*() const    { return *_e->data; };
        constexpr bool operator==(ConstIterator i) const         { return _e == i._e; }
        constexpr bool operator!=(ConstIterator i) const         { return _e != i._e; }
        ConstIterator &operator++()                              { _e = _e->next; return *this; }
    };

    Iterator begin() noexcept { return Iterator(_first); };
    Iterator end()   noexcept { return Iterator(_last);  };
    ConstIterator constBegin() const noexcept { return ConstIterator(_first); }
    ConstIterator constEnd()   const noexcept { return ConstIterator(_last); }
};

template<class T>
inline WList<T>::WList()
{
    _first = nullptr;
    _last = nullptr;
    _size = 0;
}

template <class T>
inline WList<T>::~WList()
{
    this->clear();
}

template <class T>
inline void WList<T>::append(const T &data) noexcept
{
    struct WListPrivate<T> *tmp;

    WNew(tmp, struct WListPrivate<T>, ());
    tmp->data = new T(data);

    if(unlikely(this->_first == NULL)){
        W_ASSERT(this->_last == this->_first);
        this->_first = tmp;
        this->_first->next = NULL;
        this->_last = this->_first;
    }else{
        W_ASSERT(this->_last->next == NULL);
        this->_last->next = tmp;
        this->_last = tmp;
        this->_last->next = NULL;
    }

    _size ++;
}

template <class T>
inline T *WList<T>::get_first()
{
    T *ret = this->_first->data;
    struct WListPrivate<T> *next = this->_first->next;

    WDelete(this->_first);
    this->_first = next;

    return ret;
}

template<class T>
inline WList<T>::WList(const WList<T> &l)
{
    int done;
    WListPrivate<T> *tmp, *curr;

    done = 0;
    tmp = l._first;
    curr = _first;

    while(tmp and curr){
        W_ASSERT(tmp->data);
        curr->data = tmp->data;

        tmp = tmp->next;
        curr = curr->next;
    }

    if((not tmp) and (not curr)){
        W_ASSERT(this->_size == l._size);
        goto out;
    }

    /**
     * we need to allocate more memory
     * */
    if(tmp and not curr){
        while(tmp){
            this->append(tmp->data);
            tmp = tmp->next;
        }
    }else{
        /**
         * We need to dealloc the extra memory
         * */
         while(curr){
             auto *next = curr->next;
             delete curr->data;
             delete curr;
             curr = next;
         }
    }

out:
    _size = l._size;
    W_ASSERT(WList<T>::equal(*this, l));
}

template<class T>
inline void WList<T>::clear()
{
    while(_first){
        auto *next = _first->next;
        delete _first->data;
        delete _first;
        _first = next;
    }

    _last = nullptr;
    _size = 0;
}

template<class T>
inline bool WList<T>::equal(const WList<T> &l1, const WList<T> &l2)
{
    WListPrivate<T> *f1, *f2;
    if(l1._size != l2._size)
        return false;

    f1 = l1._first;
    f2 = l2._first;

    while(f1){
        W_ASSERT(f2);

        if(f2->data != f1->data)
            return false;

        f2 = f2->next;
        f1 = f1->next;
    }

    return true;
}

