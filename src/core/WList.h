#pragma once

#include "utils/common_script.h"
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
public:
    WList();
    ~WList();


    void append(const T &data) noexcept;
    T* get_first();

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

    Iterator begin() { return Iterator(_first); };
    Iterator end()   { return Iterator(_last);  };
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
    while(this->_first){
        auto *next = this->_first->next;
        delete _first->data;
        delete _first;
        _first = next;
    }
    _first = nullptr;
    _last = nullptr;
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

