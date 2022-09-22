#pragma once

class slash
{
public:
    static constexpr char __slash()
    {
#if defined (WIN32) || defined(WIN64) || defined(__OS2__)
        return '\\';
#elif defined(unix) || defined(MACOS) || defined(IOS_WRITERNOTE)
        return '/';
#endif
    };
};

