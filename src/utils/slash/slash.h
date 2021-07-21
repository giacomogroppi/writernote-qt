#ifndef SLASH_H
#define SLASH_H

class slash
{
public:
    static inline char __slash(){
#if defined (WIN32) || defined(WIN64) || defined(__OS2__)
        return '\\';
#elif defined(unix) || defined(MACOS)
        return '/';
#endif
    };
};

#endif // SLASH_H
