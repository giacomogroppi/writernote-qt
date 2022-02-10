#ifndef COMMON_SCRIPT_H
#define COMMON_SCRIPT_H

#include <QList>
#include <QtGlobal>
#include <QDebug>
#include "time.h"
#include "stdio.h"
#include "stdlib.h"
#include "utils/common_def.h"

#define __init__ __attribute((constructor))

#define BIT(bit) (1 << (bit-1))

#define ATOMIC(istr) do{istr}while(0);

#define unlikely(exp) Q_UNLIKELY(exp)
#define likely(exp) Q_LIKELY(exp)
#define force_inline Q_ALWAYS_INLINE
#define not_used __attribute__ ((__unused__))

force_inline constexpr not_used int debug_enable()
{
#ifdef DEBUGINFO
    return 1;
#else
    return 0;
#endif
}

#ifdef DEBUGINFO
# define W_ASSERT(condition)                                    \
    do{                                                         \
        if(unlikely(!(condition))){                             \
            qDebug() << __FUNCTION__ << __FILE__ ;              \
            std::abort();                                       \
        }                                                       \
    }while(0);
#else
# define W_ASSERT(condition) ;
#endif // DEBUGINFO

force_inline FILE *__fopen(const QString &path, const char *flag)
{
#if defined(WIN32) || defined(WIN64)
    FILE *fp;
    if(*flag == 'r'){
        fp = fopen(path.toUtf8().constData(), "rb");
    }else{
        fp = fopen(path.toUtf8().constData(), "wb");
    }

    return fp;
#endif

    return fopen(path.toUtf8().constData(), flag);
}

#define discordant(first, second) ((first) * (second) < 0.0)

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
force_inline void __swap(T &t1, T &t2)
{
    const T tmp = t1;
    t1 = t2;
    t2 = tmp;
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
force_inline int is_order(const QList<T> &list)
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

template <typename T>
force_inline int is_order(const QVector<T> &list)
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

template <typename T>
force_inline int is_order_multiple(const QList<QVector<T>> &list)
{
    int i, len;
    len = list.length();
    for(i = 0; i < len; i++){
        if(!is_order(list.at(i)))
            return 0;
    }

    return 1;
}

/*
 * very fast when the list is almost sorted,
 * otherwise use std :: sort
*/
template <typename T>
inline void order(QList<T> &list)
{
    int i, j;
    int n = list.length();

    for (i = 0; i < n - 1; i++){
        for (j = 0; j < n - i - 1; j++){
            auto &val1 = list.operator[](j);
            auto &val2 = list.operator[](j+1);

            if (val1 > val2){
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
                __swap(val1, val2);
#else
                list.swapItemsAt(j, j + 1);
#endif
            }
        }
    }
}

template <typename T>
inline void order(QVector<T> &list)
{
    int i, j;
    int n = list.length();

    for (i = 0; i < n - 1; i++){
        for (j = 0; j < n - i - 1; j++){
            auto &val1 = list.operator[](j);
            auto &val2 = list.operator[](j+1);

            if (val1 > val2){
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
                __swap(val1, val2);
#else
                list.swapItemsAt(j, j + 1);
#endif
            }
        }
    }
}

template <typename T>
inline void order_multiple(QList<QVector<T>> &list)
{
    int i, len = list.length();

    for(i = 0; i < len; i++){
        auto & __list = list.operator[](i);
        order(__list);
    }

    if(!is_order_multiple(list)){
        std::abort();
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
force_inline void append_if_not_present(QList<T> &list, const T& value)
{
    if(list.indexOf(value) == -1)
        list.append(value);
}

template<typename T>
force_inline void append_if_not_present_order(QList<T> &list, const T& value)
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
force_inline bool included(const T min, const T max, const T value)
{
    W_ASSERT(min <= max);
    return min <= value && value <= max;
}

force_inline int diff(cdouble num)
{
    if(double(int(num)) > num){
        // bisogna troncare
        return double(int(num) - 1);
    }
    return double(int(num));
}

force_inline int ecc(cdouble num)
{
    if(double(int(num)) > num){
        return double(int(num));
    }

    return double(int(num) + 1);
}

template <typename T>
force_inline int div_ecc(T num, T den)
{
    return ecc(double(num) / double(den));
}

template <typename T>
force_inline int div_diff(T num, T den)
{
    return diff(double(num) / double(den));
}

template <typename T>
inline int is_present_in_list(const T *list, size_t len, const T val)
{
    size_t i;

    for(i = 0; i < len; i ++){
        if(list[i] == val)
            return 1;
    }

    return 0;
}

force_inline void abortIfDebug(){
#ifdef DEBUGINFO
    std::abort();
#else
    ;
#endif
}

template <typename T>
inline void __order(QList<QVector<T>> & list){
    int i, len = list.length();

    for(i = 0; i < len; i++){
        order(list.operator[](i));
    }
}

#define EXEC_TIME(message, function)                \
    do{                                             \
        const auto time = clock();                  \
        function;                                   \
        qDebug() << message << clock() - time;      \
    }                                               \
    while(0);

#ifdef DEBUGINFO
# define EXEC_TIME_IF_DEBUG(message, enable, function) \
    if(enable) {EXEC_TIME(message, function)} else {function};
#else
# define EXEC_TIME_IF_DEBUG(message, enable, function) function
#endif

#ifdef DEBUGINFO
# define DO_IF_DEBUG(istr) istr;
# define DO_IF_DEBUG_ENABLE(enable, istr) if(enable) {istr;}
#else
# define DO_IF_DEBUG(istr) ;
# define DO_IF_DEBUG_ENABLE(enable, istr) ;
#endif //DEBUGINFO

#ifdef DEBUGINFO
# define WDebug(enable, message) \
    if(enable){                 \
        do{                     \
            qDebug() << message; \
        } while(0);             \
    }
#else
# define WDebug(enable, message) ;
#endif //DEBUGINFO

template <typename T>
force_inline void set_zero(T &value)
{
#ifdef DEBUGINFO
    memset(&value, 0, sizeof(T));
#else
    Q_UNUSED(value);
#endif
}

template <typename T>
force_inline T __MIN(T first, T second)
{
    if(first < second)
        return first;
    return second;
}

template <typename T>
force_inline T __MAX(T first, T second)
{
    if(first < second)
        return first;
    return second;
}

#endif // COMMON_SCRIPT_H
