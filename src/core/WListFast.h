//
// Created by Giacomo Groppi on 13/05/23.
//

#ifndef WRITERNOTE_WLISTFAST_H
#define WRITERNOTE_WLISTFAST_H

#include <iostream>

// do some refactoring
// this list if O(1) in index access
template <class T>
class WListFast {
private:
    void test() const;
public:
    WListFast();

    const T& at(int i) const;
    void append(const T& element);
    void append(const WListFast<T> &other);
    void remove(int i);
    void remove(int from, int to);
    bool isEmpty() const;
    int size() const;
    const T& first() const;
    bool isOrder() const;
    void clear();
    void move(int, int);
    void reserve(int);
    T& takeAt(int i);
    const T& last() const;
    int lastIndexOf(const T& object) const;
    void insert(int index, const T& object);

    T& operator[](int i);
    bool operator==(const WListFast<T> &other) const;
    WListFast<T>& operator=(const WListFast<T> &other);

    template <typename Func>
    [[nodiscard]] bool anyMatch(Func func) const {
        for (const auto &value: *this) {
            if (func(value))
                return true;
        }
        return false;
    }

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

    // TODO: IMPORTANT
    iterator begin() noexcept { test(); return iterator(nullptr); };
    iterator end()   noexcept { test(); return iterator(nullptr);  };

    const_iterator constBegin() const noexcept { test(); return const_iterator(nullptr); }
    const_iterator constEnd()   const noexcept { test(); return const_iterator(nullptr); }
    const_iterator cBegin() const noexcept { test(); return const_iterator(nullptr); }
    const_iterator cEnd()   const noexcept { test(); return const_iterator(nullptr); }
    const_iterator begin() const noexcept { test(); return const_iterator(nullptr); }
    const_iterator end()   const noexcept { test(); return const_iterator(nullptr); }

    constexpr int indexOf(const int i) const noexcept;
};


#endif //WRITERNOTE_WLISTFAST_H
