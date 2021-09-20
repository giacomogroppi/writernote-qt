#ifndef INDICE_CLASS_H
#define INDICE_CLASS_H
#include <QList>
#include <QStringList>

#define MAXFILESTR 100

#ifdef not_def
class indice_class
{
public:
#define VER_MULTIPLE_CURRENT_TITLE 0
#define VER_SINGLE_CURRENT_TITLE 1
    int versione=0;
    QStringList titolo = {};

    void reset();

    bool isEmpty(){
        return !titolo.length();
    }
};
#endif
#endif // INDICE_CLASS_H
