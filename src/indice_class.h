#ifndef INDICE_CLASS_H
#define INDICE_CLASS_H
#include <QList>
#include <QStringList>

#define MAXFILESTR 100


class indice_class
{
public:
    int versione=0;
    QStringList titolo = {};

    void reset();

    bool isEmpty(){
        return !titolo.length();
    }
};

#endif // INDICE_CLASS_H
