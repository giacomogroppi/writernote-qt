#ifndef UTILS_H
#define UTILS_H

#include <QList>

#define IF_NOT_PRESENT_APPEND(list, element) \
    if(list.indexOf(element) == -1) list.append(element);

#define IS_PRESENT_IN_LIST(list, element) (list.indexOf(element) == -1)

#endif // UTILS_H
