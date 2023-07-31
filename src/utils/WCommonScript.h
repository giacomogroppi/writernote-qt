#pragma once

#include <iostream>
#include <cmath>
#include <cmath>
#include <cstdlib>
#include "utils/common_def.h"
#include <type_traits>
#include <algorithm>

#if DEBUG
# define DEBUGINFO
#endif

#define __init__ __attribute((constructor))

#define BIT(bit) (1 << (bit-1))
#define un(exp) __builtin_expect(!!(exp), false)
#define likely(exp) __builtin_expect(!!(exp), true)
#define force_inline inline __attribute__((always_inline))
#define not_used __attribute__ ((__unused__))
#define static_assert_type(val, should_be) static_assert(std::is_same<decltype(val), should_be>::value, #val " must be " #should_be)
#define qstr WString
#define unused(expr) do { (void)(expr); } while (0)
#define WDISABILE_COPY(ClassName) \
    ClassName(const ClassName &) = delete;\
    ClassName &operator=(const ClassName &) = delete;

#include <iostream>
#include <cstring>

#ifdef USE_QT
# include <QtGlobal>
# include <QDebug>
#endif // USE_QT

#ifdef DEBUGINFO
force_inline std::string get_only_name(const char *name)
{
    std::string res = name;
    int i;
    int from = 0, to;

    for (i = 0; name[i] != '\0'; i++) {
        if (name[i] == '(') {
            to = i;
            break;
        }
    }

    for (; i >= 0; i--) {
        if (name[i] == ' ') {
            from = i;
        }
    }

    return res.substr(from + 1, to - from - 1);
}
#endif

namespace WCommonScript{
force_inline constexpr not_used int debug_enable()
{
#ifdef DEBUGINFO
    return 1;
#else
    return 0;
#endif
}

#ifdef DEBUGINFO
#if defined(USE_QT)
# define DEBUGGER_OUT qDebug()
#else
# define DEBUGGER_OUT std::cout
#endif

# define W_ASSERT(condition) W_ASSERT_TEXT(condition, "")

# define W_ASSERT_TEXT(condition, ...)                                                                  \
    do{                                                                                                 \
        if(un(!!(condition) == false)){                                                                 \
            DEBUGGER_OUT << __FUNCTION__ << __FILE__ << __LINE__ << #condition << __VA_ARGS__;              \
            std::abort();                                                                               \
        }                                                                                               \
    }while(0)
#else
# define W_ASSERT(condition) do {} while(0)
# define W_ASSERT_TEXT(condition, ...)
#endif // DEBUGINFO

#define discordant(first, second) ((first) * (second) < 0.0)

/*
 * this function controll an enum load from setting
 * When you load a value from QSetting, and apply an implicit cast to it, it is saved correctly
 * inside the variable. If in a later version of the application, this option is removed,
 * QSetting will still load the previously saved value, which maybe is outside the enumeration, so you have to fix it.
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
force_inline void swap(T &t1, T &t2)
{
    const T tmp = t1;
    t1 = t2;
    t2 = tmp;
}

#define IS_ORDER_WITH_FUNCTION(list, function) \
    [&](const WListFast<stroke> &list) -> int{ \
        int i, len = list.length(); \
        for(i = 0; i < len; i++) { \
            if(list.at(i).function() < list.at(i+1).function()) \
                return 0; \
        } \
        return 1; \
    }


force_inline void WMemmove(void *to, const void *from, unsigned long size)
{
    W_ASSERT(from);
    W_ASSERT(to);
    W_ASSERT(size != 0);
    memmove(to, from, size);
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
force_inline int is_present_in_list_order_list(const T &list, const T& element)
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
force_inline void append_if_not_present(T &list, const T& value)
{
    if(list.indexOf(value) == -1)
        list.append(value);
}

template<typename T>
force_inline void append_if_not_present_order(T &list, const T& value)
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
    return min <= value and value <= max;
}

/**
 * The function rounds down the passed number.
 *
 * @param num The double number to compute the difference for.
 * @return The difference between the given number and its rounded-down value.
 */
force_inline int diff(double num)
{
    if(double(int(num)) > num){
        // bisogna troncare
        return double(int(num) - 1);
    }
    return double(int(num));
}

/**
 * TODO: translate in english
 * \return L'approssimazione per eccesso del numbero
 * */
force_inline int ecc(double num)
{
    if (double(int(num)) >= num) {
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
    std::cout << __func__ << file << line;
    std::abort();
#else
    unused(file);
    unused(line);
#endif
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

#if defined(DEBUGINFO)
# define WDebug(enable, message)                                                                    \
    if(enable){                                                                                     \
        DEBUGGER_OUT << get_only_name(__PRETTY_FUNCTION__).c_str() << "\t" << message;     \
    }
# define WWarning(message) WDebug(true, message)
#else
# define WDebug(enable, message) {  }
# define WWarning(message) if (false) WDebug(false, message);
#endif //DEBUGINFO

template <typename T>
force_inline void set_zero(T &value)
{
#ifdef DEBUGINFO
    memset(&value, 0, sizeof(T));
#else
    (void)(value);
#endif
}

template <typename T>
inline constexpr T Power(const T &value, cint power)
{
    T res = value;

    if(un(power) == 0){
        return 1;
    }

    int i = 1;
    for(; i < power; i ++){
        res *= value;
    }

    return res;
}

    template <class T>
    inline T abs(T one)
    {
        if (one < T(0))
            return -one;
        return one;
    }

template <class T, class Z>
force_inline bool is_near(T one, T two, Z precision)
{
    W_ASSERT(precision >= Z(0));
    return WCommonScript::abs(one - two) <= precision;
}

force_inline double distance(double y1, double y2)
{
    return WCommonScript::abs(y1 - y2);
}

// return true if left <= value <= right
template <class T>
force_inline bool is_between(T left, T value, T right, T precision = T(0))
{
    W_ASSERT(precision >= T(0));
    return left - precision <= value and value <= right + precision;
}

// return true if qMin(left, rigth) <= value <= qMax(right, left)
force_inline bool is_between_change(double left, double value, double rigth)
{
    const auto min = std::min(left, rigth);
    const auto max = std::min(left, rigth);

    return min <= value and value <= max;
}

template<typename T>
force_inline void order_complex(const T &list, int (*cmpFunctions) (const T&, const T&))
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
force_inline bool is_order_complex(const T &list, int (*cmpFunctions) (const T&, const T&))
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
    return min <= val and max >= val;
}

template<typename T>
force_inline void append_order(T & list, const T& element)
{
    int i, len;

    W_ASSERT(is_order_list(list));

    len = list.length();

    for (i = 0; i < len; i++) {
        if (un(list.at(i) > element)) {
            list.insert(i, element);
            return;
        }
    }

    list.append(element);

    W_ASSERT(is_order_list(list));
    W_ASSERT(list.contains(element));
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

/**
 * //OVERVIEW:  objectEqual(a1, a2) have to return true if a1 == a2
 *              function will return true if the two list are equal
 * @requires !\exists( W a1, a2; a1 != a2; objectEqual(a1, a2) == true)
 * @ensure (l1.length() != l2.size()) ? \result == false :
 *                                      \exists(int i; 0 <= i && i < l1.size();
 *                                                  objectEqual(l1[i], l2[i]) == false
 *                                              ) ? \result == false : \result == true;
 * */
template <class T, typename Z>
inline bool cmp_list(const T &l1, const T &l2, Z objectEqual)
{
    auto b1 = l1.constBegin();
    auto b2 = l2.constBegin();

    const auto e1 = l1.constEnd();
    const auto e2 = l2.constEnd();

    while (b1 != e1 and b2 != e2) {
        if (!objectEqual(*b1, *b2)) {
            return false;
        }
        b1 ++;
        b2 ++;
    }

    if(b1 == e1 and b2 == e2) {
        return true;
    }

    return false;
}

template <class T, typename Z>
inline void for_each(T &l, Z function)
{
    for (const auto &tmp : l) {
        function(tmp);
    }
}

template <class T1, class T2, typename Z>
inline void for_each(T1 &l1, T2 &l2, Z function)
{
    const auto e1 = l1.end();
    const auto e2 = l2.end();

    auto b1 = l1.begin();
    auto b2 = l2.begin();

    for (; b1 != e1 and b2 != e2;) {
        function(*b1, *b2);
        b1++;
        b2++;
    }
}

#define TIME_START(variable_name) \
    const auto variable_name = std::chrono::high_resolution_clock::now()

#define TIME_STOP(variable_name, message)                                                                                                           \
    do {                                                                                                                                            \
        const auto end_##variable_name = std::chrono::high_resolution_clock::now();                                                                                                                                          \
        const double delta##variable_name = std::chrono::duration_cast<std::chrono::nanoseconds>(end_##variable_name - variable_name).count();                                                                         \
            std::cout << message << delta##variable_name << "ns"                                                                                                        \
                    << 1./(delta##variable_name * (1e-9)) << "hz";                                                                                              \
    } while (0)


#define TIME_START_STATIC(variable_name) \
        static auto variable_name = std::chrono::high_resolution_clock::now()

#define TIME_STOP_NO_OUT(variable_name) \
    const auto end_##variable_name = std::chrono::high_resolution_clock::now();

#define TIME_STOP_STATIC(variable_name, message)                                                                                                    \
    do {                                                                                                                                            \
        const auto end_##variable_name = std::chrono::high_resolution_clock::now();                                                                 \
        const double delta##variable_name = (end_##variable_name - variable_name).count();                                                          \
        variable_name = end_##variable_name;                                                                                                        \
        qDebug() << message << delta##variable_name << "ns"                                                                                         \
        << 1./(delta##variable_name * (1e-9)) << "hz";                                                                                              \
} while (0)

template <class T>
inline T max (T one, T two)
{
    if (one >= two)
        return one;
    return two;
}

template <class T>
inline T min(T one, T two)
{
    if (one >= two)
        return two;
    return one;
}

template <class T>
inline T toRad (T value)
{
    return value / 180 * M_PI;
}

}
