#ifndef INDICE_CLASS_H
#define INDICE_CLASS_H
#include <QList>
#include <QStringList>

#define MAXFILESTR 100


class indice_class
{
public:
    int versione=0;
    QStringList video = {};
    QStringList audio = {};
    QStringList titolo = {};
    QStringList compressione = {};
    //QStringList file_testo = {};

    int scritto = 0;

    //void newfile(char* /*video*/, char* /*audio*/, char* /*titolo*/, char* /*file_testo*/);
    void reset();
    void stampa();
};

#endif // INDICE_CLASS_H
