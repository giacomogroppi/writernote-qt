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
public:
    WList();
    ~WList();

    void append(T *data);
    T* get_first();
};

template<class T>
inline WList<T>::WList()
{
    _first = nullptr;
    _last = nullptr;
}

template <class T>
inline WList<T>::~WList()
{
    /*
     * the destructor don't deallocate nothing
     * you have to destroy all the object yourself
    */

    W_ASSERT(_first == nullptr);
    W_ASSERT(_last == nullptr);
}

template <class T>
inline void WList<T>::append(T *data)
{
    struct WListPrivate<T> *tmp;

    W_ASSERT(data);

    WNew(tmp, struct WListPrivate<T>, ());
    tmp->data = data;

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

