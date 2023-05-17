#ifndef WRITERNOTE_WBYTEARRAY_H
#define WRITERNOTE_WBYTEARRAY_H

#include <iostream>

class WByteArray {
private:
    void test() const;
    char *_data;
    int _size;
public:
    WByteArray ();
    WByteArray(const char *data, size_t size = -1);

    std::string toStdString() const;
    const char *constData() const;
    size_t size() const;
    void append(char data);
    void append(const char *data, int size = -1);
    char at(int i) const;
    void clear() noexcept;

    WByteArray mid(int from, int to) const;

    bool operator==(const WByteArray &other) const;
    WByteArray &operator+(const WByteArray &other) const;
    WByteArray &operator+=(const WByteArray &other);

    static WByteArray fromRawData(const char *data, int size);

    class iterator{
    private:
        char *array;
        int index;
    public:
        explicit iterator(char *data) : array(data), index(0) {; };

        //T* operator->()         { return array._data[index]; };
        char &operator*() const    { return array[index]; };
        bool operator==(iterator i) const         { return index == i.index; }
        bool operator!=(iterator i) const         { return index != i.index; }
        iterator &operator++()                              { index ++; return *this; }
        iterator operator++(int) { auto copy = *this; ++*this; return copy; }
    };

    class const_iterator{
    private:
        const char* array;
        int index;
    public:
        explicit const_iterator(const char *data) : array(data), index(0) {  };

        const char &operator*() const    { return array[index]; };
        bool operator==(const_iterator i) const         { return index == i.index; }
        bool operator!=(const_iterator i) const         { return index != i.index; }
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


#endif //WRITERNOTE_WBYTEARRAY_H
