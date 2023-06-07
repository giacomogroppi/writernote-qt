//
// Created by giamg on 07/06/2023.
//

#ifndef WRITERNOTE_WABSTRACTLIST_H
#define WRITERNOTE_WABSTRACTLIST_H

#include "utils/WCommonScript.h"
#include <algorithm>

template <class T, class Iterator, class ConstIterator>
class WAbstractList {
protected:
    virtual Iterator begin() = 0;
    virtual Iterator end() = 0;

    virtual ConstIterator begin() const = 0;
    virtual ConstIterator end() const = 0;

public:
    WAbstractList () = default;

    [[nodiscard]]
    bool isOrder() const noexcept;

    template <class CompareFunc>
    [[nodiscard]]
    bool isOrder(CompareFunc func) const;
};

template<class T, class Iterator, class ConstIterator>
template<class CompareFunc>
inline bool WAbstractList<T, Iterator, ConstIterator>::isOrder(CompareFunc func) const
{
    auto b1 = this->begin();
    auto b2 = this->begin() + 1;
    auto e = this->end();

    for (; b2 != e; b1++, b2 ++) {
        if (func (b1, b2))
            return false;
    }

    return true;
}

template<class T, class Iterator, class ConstIterator>
inline bool WAbstractList<T, Iterator, ConstIterator>::isOrder() const noexcept
{
    return this->isOrder([](const T& t1, const T& t2) -> bool {
       return t1 > t2;
    });
}


#endif //WRITERNOTE_WABSTRACTLIST_H
