#ifndef STRUCT_LAST_FILE_H
#define STRUCT_LAST_FILE_H

#include <QMetaType>
#define MAXSTR 100
#define MAXMOD 10

#define TYPE_COMPUTER 0
#define TYPE_CLOUD 1

struct last_file{
    char posizione[MAXSTR+1];
    short int type;
    char last_modification[MAXMOD+1];
};

Q_DECLARE_METATYPE(last_file);

#endif // STRUCT_LAST_FILE_H
