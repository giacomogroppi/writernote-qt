//
// Created by Giacomo Groppi on 28/04/23.
//

#ifndef WRITERNOTE_WVECTOR_H
#define WRITERNOTE_WVECTOR_H

#include <iostream>

template <class T>
class WVector {
private:
    T *_data;
    size_t _size;

    void test() const;
public:
    WVector();

    void append(const WVector<T> &other);
    void append(const T &data);
    const T& get(int i) const;
    int size() const;
    void removeAt(int i);
    void move(int from, int to);
    void clear();
    const T& at(int i) const;
    void reserve(int numberOfElement);
    T& takeAt(int i);
    const T& last() const;
    const T& first() const;
    T& operator[](int index);
    bool isEmpty() const;
    bool isOrder() const;
    void order();
    void insert(int index, const T& data);
    T& takeFirst();
    const T* constData() const;

    class iterator{
    private:
        T *array;
        int index;
    public:
        explicit iterator(T *data) : array(data), index(0) {; };

        T* operator->()         { return array[index]; };
        T &operator*() const    { return array[index]; };
        constexpr bool operator==(iterator i) const         { return index == i.index; }
        constexpr bool operator!=(iterator i) const         { return index != i.index; }
        iterator &operator++()                              { index ++; return *this; }
        iterator operator++(int) { auto copy = *this; ++*this; return copy; }
    };

    class const_iterator{
    private:
        const T *array;
        int index;
    public:
        explicit const_iterator(const T *data) : array(data), index(0) {  };

        const T* operator->() const   { return array[index]; };
        const T &operator*() const    { return array[index]; };
        constexpr bool operator==(const_iterator i) const         { return index == i.index; }
        constexpr bool operator!=(const_iterator i) const         { return index != i.index; }
        const_iterator &operator++()                              { index ++; return *this; }
        const_iterator operator++(int) { auto copy = *this; ++*this; return copy; }
    };

    iterator begin() noexcept { test(); return iterator(this->_data); };
    iterator end()   noexcept { test(); return iterator(nullptr);  };

    const_iterator constBegin() const noexcept { test(); return const_iterator(this->_data); }
    const_iterator constEnd()   const noexcept { test(); return const_iterator(nullptr); }
    const_iterator cBegin() const noexcept { test(); return const_iterator(this->_data); }
    const_iterator cEnd()   const noexcept { test(); return const_iterator(nullptr); }
    const_iterator begin() const noexcept { test(); return const_iterator(this->_data); }
    const_iterator end()   const noexcept { test(); return const_iterator(nullptr); }
};


#endif //WRITERNOTE_WVECTOR_H
