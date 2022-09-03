#pragma once

#ifdef CLOUD

#define TYPE_ABBONATO 1
#define TYPE_NONABBONATO 0

#define NOT_USER_RECENT -1

#include "struct_mail.h"

/*
 * sha 256 len is 64 caracter [one for termination caracter]
*/
#define STRPASS 65

struct struct_user{
    struct name_s m_name;
    struct mail_s m_mail;

    char password[STRPASS];

    int m_money_into_account;
};

/*
 * it's store the last friend, in qsetting under
 * $HOME/.config/writernote/...
*/
struct struct_friend{
    int m_type_frind;
    struct name_s m_name;

    struct mail_s m_mail;
};

void save_recent_user(struct struct_user *);
struct struct_user * load_recent_user();

void save_recent_friend(struct struct_friend *);
struct struct_friend * load_recent_friend();

#endif // CLOUD
