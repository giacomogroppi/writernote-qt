#ifndef COMMON_SCRIPT_H
#define COMMON_SCRIPT_H

#include <QList>
#include <QtGlobal>
#include "utils/common_def.h"

#define BIT(bit) (1 << (bit-1))

/*
 * this function controll an enum load from setting
 * When you load a value from QSetting, and apply an implicit cast to it, it is saved correctly
 * inside the variable. If in a later version of the application, this option is removed,
 * QSetting will still read the previously saved value, which maybe is outside the enumeration, so you have to fix it.
 *
 * Quando si carica un valore da QSetting, e gli si applica un cast implicito, esso viene salvato correttamente
 * all'interno della variabile. Se in una versione successiva dell'applicazione, questa opzione viene tolta,
 * QSetting leggerà comunque il valore salvato precedentemente, che magari è fuori dall'enumerazione, quindi bisogna aggiustarlo.
*/
#define CONTROLL_ENUM(current_val, max_val, def_val) \
{ \
    if((uint)current_val > (uint) current_val) \
        current_val = def_val; \
}

#define IS_PRESENT_IN_LIST(list, element) ((list).indexOf(element) != -1)
#define IS_NOT_PRESENT_IN_LIST(list, element) (!IS_PRESENT_IN_LIST(list, element))

#define IF_NOT_PRESENT_APPEND(list, element) \
    if(list.indexOf(element) == -1) list.append(element);

template <typename T>
Q_ALWAYS_INLINE void __swap(T &t1, T &t2)
{
    const T tmp = t1;
    t1 = t2;
    t2 = t1;
}

#define IS_ORDER_WITH_FUNCTION(list, function) \
    [&](const QList<stroke> &list) -> int{ \
        int i, len = list.length(); \
        for(i = 0; i < len; i++) { \
            if(list.at(i).function() < list.at(i+1).function()) \
                return 0; \
        } \
        return 1; \
    }


template <typename T>
Q_ALWAYS_INLINE int is_order(const QList<T> &list)
{
    int i, len;
    len = list.length();
    for(i = 0; i < len - 1; i++){
        if(list.at(i) > list.at(i+1))
            return 0;
        //if(list.at(i) > list.at(i+1))
        //    return 0;
    }

    return 1;
}

/*
 * very fast when the list is almost sorted,
 * otherwise use std :: sort
*/
template <typename T>
inline void order(T &list)
{
    int i, j;
    int n = list.length();

    for (i = 0; i < n - 1; i++){
        for (j = 0; j < n - i - 1; j++){
            const T *val1 = (T *)&list.at(j);
            const T *val2 = (T *)&list.at(j+1);

            if (*val1 > *val2){
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
                __swap(*val1, *val2);
#else
                list.swapItemsAt(j, j + 1);
#endif
            }
        }
    }
}

template <typename T>
Q_ALWAYS_INLINE int is_present_in_list_order(const QList<T> &list, const T& element)
{
#if defined(DEBUGINFO)
    //the list must be sorted
    Q_ASSERT(is_order(list));
#endif

    int i, len = list.length();
    for(i = 0; i < len; i++){
        const T& tmp = list.at(i);
        if(tmp == element)
            return i;

        if(tmp > element)
            return -1;
    }

    return -1;
}

template <typename T>
Q_ALWAYS_INLINE void append_if_not_present(QList<T> &list, const T& value)
{
    if(list.indexOf(value) == -1)
        list.append(value);
}

template<typename T>
Q_ALWAYS_INLINE void append_if_not_present_order(QList<T> &list, const T& value)
{
#if defined(DEBUGINFO)
    Q_ASSERT(is_order(list));
#endif

    if(is_present_in_list_order(list, value) == -1){
        // we can append
        list.append(value);
    }
}

template <typename T>
Q_ALWAYS_INLINE bool included(const T min, const T max, const T value)
{
    Q_ASSERT(min <= max);
    return min <= value && value <= max;
}

Q_ALWAYS_INLINE int diff(cdouble num)
{
    if(double(int(num)) > num){
        // bisogna troncare
        return double(int(num) - 1);
    }
    return double(int(num));
}

Q_ALWAYS_INLINE int ecc(cdouble num)
{
    if(double(int(num)) > num){
        return double(int(num));
    }

    return double(int(num) + 1);
}

template <typename T>
Q_ALWAYS_INLINE int div_ecc(T num, T den)
{
    return ecc(double(num) / double(den));
}

template <typename T>
Q_ALWAYS_INLINE int div_diff(T num, T den)
{
    return diff(double(num) / double(den));
}

template <typename T>
inline int is_present_in_list(const T *list, size_t len, T val)
{
    for(;len > 0; list ++, len --){
        if(*list == val)
            return 1;
    }

    return 0;
}

#endif // COMMON_SCRIPT_H
