#pragma once

#include <iostream>
#include <cmath>
#include <math.h>
#include <cstdlib>
#include "utils/common_def.h"
#include <type_traits>
#include <algorithm>

#define __init__ __attribute((constructor))

#define BIT(bit) (1 << (bit-1))
#define un(exp) __builtin_expect(!!(exp), false)
#define likely(exp) __buildin_expect(!!(exp), true)
#define force_inline inline __attribute__((always_inline))
#define not_used __attribute__ ((__unused__))
#define static_assert_type(val, should_be) static_assert(std::is_same<decltype(val), should_be>::value, #val " must be " #should_be)
#define qstr QString
#define unused(expr) do { (void)(expr); } while (0)
#include "touch/dataTouch/Point.h"
#include <vector>
#include <iostream>
#include <list>

#ifdef DEBUGINFO
force_inline QString get_only_name(const char *name)
{
    QString res = name;
    int i;
    int from, to;

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

    return res.mid(from + 1, to - from - 1);
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
# define W_ASSERT(condition) W_ASSERT_TEXT(condition, "")

# define W_ASSERT_TEXT(condition, ...)                                                                  \
    do{                                                                                                 \
        if(un(!!(condition) == false)){                                                                 \
            qDebug() << __FUNCTION__ << __FILE__ << __LINE__ << #condition << __VA_ARGS__;              \
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


template <typename T>
force_inline int is_order_list(const std::vector<T> &list)
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
force_inline int is_order_vector(const std::vector<T> &list)
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
force_inline int is_order_multiple(const std::vector<std::vector<T>> &list)
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
inline void order_list(std::vector<T> &list)
{
    int i, j;
    int n = list.length();

    for (i = 0; i < n - 1; i++){
        for (j = 0; j < n - i - 1; j++){
            auto &val1 = list.operator[](j);
            auto &val2 = list.operator[](j+1);

            if (val1 > val2){
                list.swapItemsAt(j, j + 1);
            }
        }
    }
}

template <typename T>
inline void order_vector(std::vector<T> &list)
{
    int i, j;
    int n = list.length();

    for (i = 0; i < n - 1; i++){
        for (j = 0; j < n - i - 1; j++){
            auto &val1 = list.operator[](j);
            auto &val2 = list.operator[](j+1);

            if (val1 > val2){
                list.swapItemsAt(j, j + 1);
            }
        }
    }
}

template <typename T>
inline void order_multiple(std::vector<std::vector<T>> &list)
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
force_inline int is_present_in_list_order_list(const std::vector<T> &list, const T& element)
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

    template <class T>
    force_inline int contains(const std::vector<T> &vec, const T& value)
    {
        return std::count(vec.begin(),
                          vec.end(),
                          value);
    }

template <typename T>
force_inline int is_present_in_list_order_vector(const std::vector<T> &list, const T& element)
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
force_inline void append_if_not_present(std::vector<T> &list, const T& value)
{
    if(list.indexOf(value) == -1)
        list.append(value);
}

template<typename T>
force_inline void append_if_not_present_order(std::vector<T> &list, const T& value)
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
    unused(file);
    unused(line);
#endif
}

template <typename T>
inline void __order(std::vector<std::vector<T>> & list){
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

#if defined(DEBUGINFO)
# define WDebug(enable, message)                                                                    \
    if(enable){                                                                                     \
        qDebug() << get_only_name(__PRETTY_FUNCTION__).toUtf8().constData() << "\t" << message;     \
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
force_inline constexpr T Power(const T &value, cint power)
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

force_inline bool is_near(cdouble one, cdouble two, cdouble precision)
{
    W_ASSERT(precision >= 0.);
    return std::abs(one - two) <= precision;
}

force_inline double distance_not_square(const PointF& first, const PointF& second)
{
    const auto p = WCommonScript::Power(first.x() - second.x(), 2) + WCommonScript::Power(first.y() - second.y(), 2);
    if constexpr (WCommonScript::debug_enable()){
        const auto not_used res = WCommonScript::Power(first.x() - second.x(), 2) + std::pow(first.y() - second.y(), 2);
        W_ASSERT(is_near(res, p, 0.001));
    }

    return p;
}

force_inline double distance(double y1, double y2)
{
    return std::abs(y1 - y2);
}

force_inline double distance(const PointF& first, const PointF& second)
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
    const auto min = std::min(left, rigth);
    const auto max = std::min(left, rigth);

    return min <= value and value <= max;
}

force_inline bool is_near(const PointF &point1, const PointF &point2, cdouble prec)
{
    return  is_near(point1.x(), point2.x(), prec) and
            is_near(point1.y(), point2.y(), prec);
}

template<typename T>
force_inline void order_complex(const std::vector<T> &list, int (*cmpFunctions) (const T&, const T&))
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
force_inline bool is_order_complex(const std::vector<T> &list, int (*cmpFunctions) (const T&, const T&))
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
force_inline void append_order(std::vector<T> & list, const T& element)
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
 * @ensure (l1.length() != l2.length()) ? \result == false :
 *                                      \exists(int i; 0 <= i && i < l1.length();
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

template <class T>
force_inline void removeFromArray(std::vector<T> &vec, const T& object)
{
    for (int i = 0; i < (int) vec.size(); i++) {
        if(vec.at(i) == object) {
            /**
             * technically the compiler should execute the std::next function in O(1).
             * */
            vec.erase(
                    std::next(vec.begin(),
                              i)
            );
        }
    }
}

#define TIME_START(variable_name) \
    const auto variable_name = std::chrono::high_resolution_clock::now()

#define TIME_STOP(variable_name, message)                                                                                                           \
    do {                                                                                                                                            \
        const auto end_##variable_name = std::chrono::high_resolution_clock::now();                                                                                                                                          \
        const double delta##variable_name = std::chrono::duration_cast<std::chrono::nanoseconds>(end_##variable_name - variable_name).count();                                                                         \
        qDebug() << message << delta##variable_name << "ns"                                                                                                        \
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

}
