#pragma once

namespace slash
{
    consteval char __slash()
    {
#if defined (WIN32) || defined(WIN64) || defined(__OS2__)
        return '\\';
#else
        return '/';
#endif
    };
};

