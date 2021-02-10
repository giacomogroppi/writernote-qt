#ifndef STRUCT_LAST_FILE_H
#define STRUCT_LAST_FILE_H

#include <QMetaType>
#define MAXSTR 100
#define MAXMOD 10

#define TYPE_COMPUTER 0
#define TYPE_CLOUD 1

#define MAXSTR_ MAXSTR+1
#define MAXMOD_ MAXMOD+1

struct last_file{
    char posizione[MAXSTR_];
    short int type;
    char last_modification[MAXMOD_];
};

Q_DECLARE_METATYPE(last_file);

#endif // STRUCT_LAST_FILE_H
