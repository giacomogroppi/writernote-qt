//
// Created by Giacomo Groppi on 28/04/23.
//

#ifndef WRITERNOTE_WVECTOR_H
#define WRITERNOTE_WVECTOR_H

template <class T>
class WVector {
private:
    T *_data;
    size_t _size;

    void test();
public:
    WVector();

    void append(const T &data);
    const T& get(int i) const;
    int size() const;
    void removeAt(int i);

    class iterator{
    private:
        WVector<T> &array;
        int index;
    public:
        explicit iterator(WVector<T> *data) : array(*data), index(0) {; };

        T* operator->()         { return array._data[index]; };
        T &operator*() const    { return array._data[index]; };
        constexpr bool operator==(iterator i) const         { return index == i.index; }
        constexpr bool operator!=(iterator i) const         { return index != i.index; }
        iterator &operator++()                              { index ++; return *this; }
        iterator operator++(int) { auto copy = *this; ++*this; return copy; }
    };

    class const_iterator{
    private:
        WVector<T> &array;
        int index;
    public:
        explicit const_iterator(const WVector<T> &data) : array(data), index(0) {  };

        const T* operator->() const   { W_ASSERT(_e); return array._data[index]; };
        const T &operator*() const    { W_ASSERT(_e); return array._data[index]; };
        constexpr bool operator==(const_iterator i) const         { return index == i.index; }
        constexpr bool operator!=(const_iterator i) const         { return index != i.index; }
        const_iterator &operator++()                              { index ++; return *this; }
        const_iterator operator++(int) { auto copy = *this; ++*this; return copy; }
    };

    iterator begin() noexcept { test(); return iterator(this); };
    iterator end()   noexcept { test(); return iterator(nullptr);  };

    const_iterator constBegin() const noexcept { test(); return const_iterator(this->_data); }
    const_iterator constEnd()   const noexcept { test(); return const_iterator(nullptr); }
    const_iterator cBegin() const noexcept { test(); return const_iterator(this->_data); }
    const_iterator cEnd()   const noexcept { test(); return const_iterator(nullptr); }
    const_iterator begin() const noexcept { test(); return const_iterator(this->_data); }
    const_iterator end()   const noexcept { test(); return const_iterator(nullptr); }
};


#endif //WRITERNOTE_WVECTOR_H
