#ifndef STRUCT_LAST_FILE_H
#define STRUCT_LAST_FILE_H

#include <QSettings>

#include <QMetaType>
#define MAXSTR 200
#define MAXMOD 10

/* the file is in your computer or in cloud */
#define TYPE_COMPUTER 0
#define TYPE_CLOUD 1

#define MAXSTR__FILE MAXSTR+1
#define MAXMOD__FILE MAXMOD+1

#define TYPE_OWNER_YOU 0
#define TYPE_OWNER_EXTERNAL 1

struct owner_struct{
    unsigned short int type_user;
    char name[MAXSTR__FILE];
};

struct last_file{
    char posizione[MAXSTR__FILE];
    unsigned short int type;
    char last_modification_o[MAXMOD__FILE]; /* time */
    char last_modification_g[MAXMOD__FILE]; /* day */

    struct owner_struct owner;
};


Q_DECLARE_METATYPE(last_file);


/* file save */
void save_data(const QString &path, int type = TYPE_COMPUTER, int owner_TYPE = TYPE_OWNER_YOU, char *owner = NULL);
void save_data_f(int quanti, last_file * m_lista);
void save_quanti(int quanti);
void save_data_f(const QByteArray &);

/* file load */
last_file *load_data(int quanti);
int load_quanti();

#endif // STRUCT_LAST_FILE_H
