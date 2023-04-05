#pragma once

template <class K, class T>
class WPair
{
private:
    K _key;
    T _value;
public:
    explicit WPair() = default;
    explicit WPair(const K &key, const T &value);
    ~WPair() = default;

    const K& getKey() const {return this->_key;};
    K& getKey() {return this->_key;};

    const T& getValue() const {return this->_value;};
    T& getValue () {return this->_value;};

    void setKey(const K &key) {this->_key = key;};
    void setValue(const T &value) {this->_value = value;};


    bool operator==(const WPair<K, T> &other)
    {
        if (this == &other)
            return true;
        return this->_key == other._key &&
               this->_value == other._value;
    }
};


template<class K, class T>
inline WPair<K, T>::WPair(const K &key, const T &value)
    : _key(key)
    , _value(value)
{
}

