#ifndef STRUCT_USER_H
#define STRUCT_USER_H

#define STRLEN_NAME 100
#define STRLEN_MAIL 50

#define TYPE_ABBONATO 1
#define TYPE_NONABBONATO 0

struct name_s{
    char m_name[STRLEN_NAME+1];
    char m_surname[STRLEN_NAME+1];
};

struct mail_s{
    char m_mail[STRLEN_MAIL+1];
};

struct struct_user{
    struct name_s m_name;
    struct mail_s m_mail;

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

#endif // STRUCT_USER_H
