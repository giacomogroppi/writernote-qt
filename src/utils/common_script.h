#ifndef COMMON_SCRIPT_H
#define COMMON_SCRIPT_H

#include <QList>
#include <QtGlobal>

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

#define IS_PRESENT_IN_LIST(list, element) (list.indexOf(element) != -1)
#define IS_NOT_PRESENT_IN_LIST(list, element) (!IS_PRESENT_IN_LIST(list, element))

#define IF_NOT_PRESENT_APPEND(list, element) \
    if(list.indexOf(element) == -1) list.append(element);

template <typename T>
Q_ALWAYS_INLINE void __swap(T &t1, T &t2)
{
    T tmp = t1;
    t1 = t2;
    t2 = t1;
}

template <typename T>
Q_ALWAYS_INLINE int is_order(QList<T> &list)
{
    int i, len;
    len = list.length();
    for(i = 0; i < len - 1; i++){
        if(list.at(i) > list.at(i+1))
            return 0;
    }

    return 1;
}

template <typename T>
Q_ALWAYS_INLINE int order(QList<T> &list)
{
    /* we use bubble-sort */
    int i, j, mod;
    int n = list.length();

    for (i = 0, mod = 0; i < n - 1; i++){
        for (j = 0; j < n - i - 1; j++){
            if (list[j] > list[j+1]){
                __swap(&list[j], &list[j+1]);
                mod = 1;
            }
        }
    }
    return mod;
}

#endif // COMMON_SCRIPT_H
