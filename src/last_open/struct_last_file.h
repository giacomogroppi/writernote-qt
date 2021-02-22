#ifndef STRUCT_LAST_FILE_H
#define STRUCT_LAST_FILE_H

#include <QSettings>

#include <QMetaType>
#define MAXSTR 100
#define MAXMOD 10

/* the file is in your computer or in cloud */
#define TYPE_COMPUTER 0
#define TYPE_CLOUD 1

#define MAXSTR_ MAXSTR+1
#define MAXMOD_ MAXMOD+1

#define TYPE_OWNER_YOU 0
#define TYPE_OWNER_EXTERNAL 1

struct owner_struct{
    unsigned short int type_user;
    char name[MAXSTR_];
};

struct last_file{
    char posizione[MAXSTR_];
    unsigned short int type;
    char last_modification_o[MAXMOD_];
    char last_modification_g[MAXMOD_];

    struct owner_struct owner;
};


Q_DECLARE_METATYPE(last_file);


/* file save */
void save_data(QString &path, int type = TYPE_COMPUTER, int owner_TYPE = TYPE_OWNER_YOU, char *owner = NULL);
void save_data_f(QSettings &, int quanti, last_file * m_lista);

/* file load */
last_file *load_data(QSettings &setting, int quanti);

#endif // STRUCT_LAST_FILE_H
