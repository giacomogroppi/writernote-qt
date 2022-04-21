#ifndef UTILS_H
#define UTILS_H

#include <QSize>
#include "qapplication.h"
#include "utils/common_script.h"
#include <QList>
#include <QScreen>

namespace utils{

force_inline QSize get_size_screen()
{
    QSize size = qApp->screens()[0]->size();
    return size;
}

}
#endif // UTILS_H
