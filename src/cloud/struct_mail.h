#pragma once

#ifdef CLOUD

#define STRLEN_NAME 100
#define STRLEN_MAIL 50

struct name_s{
    char m_name[STRLEN_NAME+1];
    char m_surname[STRLEN_NAME+1];
};

struct mail_s{
    char m_mail[STRLEN_MAIL+1];
};

#endif // CLOUD
