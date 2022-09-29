#pragma once

#include <QList>
#include <QtGlobal>
#include <QDebug>
#include <QPointF>
#include <QPoint>
#include <iostream>
#include <math.h>
#include <qmath.h>
#include "stdlib.h"
#include "utils/common_def.h"

#define __init__ __attribute((constructor))

#define BIT(bit) (1 << (bit-1))

#define ATOMIC(istr) do{istr}while(0);

#define unlikely(exp) Q_UNLIKELY(exp)
#define likely(exp) Q_LIKELY(exp)
#define unknown(exp) exp

#define force_inline Q_ALWAYS_INLINE
#define not_used __attribute__ ((__unused__))

#define qstr QString
#define qarr QByteArray

#ifdef DEBUGINFO
force_inline QString get_only_name(const char *name)
{
    QString res = name;
    int len, i;
    int from, to;

    for(i = 0; name[i] != '\0'; i++){
        if(name[i] == '('){
            to = i;
            break;
        }
    }


    for(; i >= 0; i--){
        if(name[i] == ' '){
            from = i;
        }
    }

    return res.mid(from + 1, to - from - 1);
}
#endif

force_inline constexpr not_used int debug_enable()
{
#ifdef DEBUGINFO
    return 1;
#else
    return 0;
#endif
}

#ifdef DEBUGINFO
# define W_ASSERT(condition)                                                    \
    do{                                                                         \
        if(unlikely(!!(condition) == false)){                                   \
            qDebug() << __FUNCTION__ << __FILE__ << __LINE__ << #condition;     \
            std::abort();                                                       \
        }                                                                       \
    }while(0)
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
force_inline int is_order_list(const QList<T> &list)
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

force_inline size_t WMemcpy(void *to, const void *from, size_t size)
{
    W_ASSERT(from);
    W_ASSERT(to);
    W_ASSERT(size);
    memcpy(to, from, size);
    return size;
}

template <typename T>
force_inline int is_order_vector(const QVector<T> &list)
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
        if(!is_order_vector(list.at(i)))
            return 0;
    }

    return 1;
}

/*
 * very fast when the list is almost sorted,
 * otherwise use std :: sort
*/
template <typename T>
inline void order_list(QList<T> &list)
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
inline void order_vector(QVector<T> &list)
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
        order_vector(__list);
    }

    if(!is_order_multiple(list)){
        std::abort();
    }
}

template <typename T>
Q_ALWAYS_INLINE int is_present_in_list_order_list(const QList<T> &list, const T& element)
{
    //the list must be sorted
    W_ASSERT(is_order_list(list));

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
Q_ALWAYS_INLINE int is_present_in_list_order_vector(const QVector<T> &list, const T& element)
{
    W_ASSERT(is_order_vector(list));

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
    if(double(int(num)) >= num){
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

force_inline void abortIfDebug(cchar *file, int line){
#ifdef DEBUGINFO
    qDebug() << __func__ << file << line;
    std::abort();
#else
    Q_UNUSED(file);
    Q_UNUSED(line);
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
# define DO_IF_DEBUG(istr) istr
# define DO_IF_DEBUG_ENABLE(enable, istr)   \
        if(enable)                          \
        {                                   \
            istr;                           \
        }
#else
# define DO_IF_DEBUG(istr) ;
# define DO_IF_DEBUG_ENABLE(enable, istr) ;
#endif //DEBUGINFO

#ifdef DEBUGINFO
# define WDebug(enable, message)                                                                    \
    if(enable){                                                                                     \
        qDebug() << get_only_name(__PRETTY_FUNCTION__).toUtf8().constData() << "\t" << message;     \
    }
# define WWarning(message) WDebug(true, message)
#else
# define WDebug(enable, message) { if(enable and false) qDebug() << message;}
# define WWarning(message) if (false) WDebug(false, message);
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

template <typename T>
force_inline Q_CONSTEXPR T wPower(const T &value, cint power)
{
    T res = value;

    if(unlikely(power) == 0){
        return 1;
    }

    int i = 1;
    for(; i < power; i ++){
        res *= value;
    }

    return res;
}

force_inline bool is_near(cdouble one, cdouble two, cdouble precision)
{
    W_ASSERT(precision >= 0.);
    return qAbs(one - two) <= precision;
}

force_inline double distance_not_square(const QPointF& first, const QPointF& second)
{
    const auto p = wPower(first.x() - second.x(), 2) + wPower(first.y() - second.y(), 2);
    if(debug_enable()){
        const auto not_used res = std::pow(first.x() - second.x(), 2) + std::pow(first.y() - second.y(), 2);
        W_ASSERT(is_near(res, p, 0.001));
    }

    return p;
}

force_inline double distance(const QPointF& first, const QPointF& second)
{
    return std::sqrt(distance_not_square(first, second));
}

// return true if left <= value <= rigth
force_inline bool is_between(const double left, const double value, const double rigth)
{
    return left <= value and value <= rigth;
}

// return true if qMin(left, rigth) <= value <= qMax(rigth, left)
force_inline bool is_between_change(const double left, const double value, const double rigth)
{
    const auto min = qMin(left, rigth);
    const auto max = qMax(left, rigth);

    return min <= value and value <= max;
}

force_inline bool is_near(const QPointF &point1, const QPointF &point2, cdouble prec)
{
    return  is_near(point1.x(), point2.x(), prec) &&
            is_near(point1.y(), point2.y(), prec);
}

template<typename T>
force_inline void order_complex(const QList<T> &list, int (*cmpFunctions) (const T&, const T&))
{
    int i, j;
    int n = list.length();

    for (i = 0; i < n - 1; i++){
        for (j = 0; j < n - i - 1; j++){
            auto &val1 = list.operator[](j);
            auto &val2 = list.operator[](j+1);

            // val1 > val2
            if (cmpFunctions(val1, val2)){
                list.swapItemsAt(j, j + 1);
            }
        }
    }
}

template<typename T>
force_inline bool is_order_complex(const QList<T> &list, int (*cmpFunctions) (const T&, const T&))
{
    int i;
    int n = list.length();

    for (i = 0; i < n - 1; i++){
        auto &val1 = list.operator[](i);
        auto &val2 = list.operator[](i+1);

        // val1 > val2
        if (cmpFunctions(val1, val2)){
            return true;
        }
    }

    return false;
}

template<typename T>
force_inline bool is_included(const T& val, const T& min, const T& max)
{
    W_ASSERT(min <= max);
    return min <= val && max >= val;
}

template<typename T>
force_inline void append_order(QVector<T> & list, const T& element)
{
    int i, len;

    len = list.length();

    for(i = 0; i < len; i++){
        if(unlikely(list.at(i) > element)){
            list.insert(i, element);
            return;
        }
    }

    list.append(element);
}

inline constexpr size_t WStrlen(const char *str)
{
    size_t s = 0;
    for(; *str != '\0'; str ++)
        s ++;
    return s;
}

inline constexpr bool WStrEqual(const char *s1, const char *s2)
{
    W_ASSERT(s1 and s2);
    while(*s1 != '\0' and *s2 != '\0'){
        if(*s1 != *s2)
            return false;

        s1 ++;
        s2 ++;
    }

    /* one of them is '\0' */
    if(*s1 != *s2)
        return false;

    W_ASSERT(*s1 == '\0' and *s2 == '\0');

    return true;
}
