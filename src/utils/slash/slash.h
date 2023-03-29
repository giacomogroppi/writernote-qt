#pragma once

#include "QtCore/qsystemdetection.h"
class slash
{
public:
    static constexpr char __slash()
    {
#if defined (WIN32) || defined(WIN64) || defined(__OS2__)
        return '\\';
#elif defined(unix) || defined(IOS_WRITERNOTE) || defined(Q_OS_MAC)
        return '/';
#endif
    };
};

