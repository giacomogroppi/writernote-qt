#pragma once

#include <iostream>
#ifdef USE_QT
# include <QDebug>
#endif // USE_QT

namespace core {

using Output =
#ifdef USE_QT

        QDebug;
#else
        std::ostream&;
#endif // !USE_QT

}
