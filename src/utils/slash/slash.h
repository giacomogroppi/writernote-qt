#ifndef SLASH_H
#define SLASH_H

class slash
{
public:
    static inline const char *__slash(){
#if defined (WIN32) || defined(WIN64)
        return "\";
#elif defined(unix)
        return "/";
#endif
    };
};

#endif // SLASH_H
