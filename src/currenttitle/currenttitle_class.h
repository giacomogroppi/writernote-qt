#ifndef CURRENT_TITLE_CLASS_H
#define CURRENT_TITLE_CLASS_H

#include <QStringList>
#include <QList>

#include "../touch/datastruct/datastruct.h"

class currenttitle_class{
public:
    int versione = 1;
    bool se_registato = false, se_tradotto = false;
    QString audio_position_path = "";
    QString testi = "";
    QStringList testinohtml = {};
    QList<int> posizione_iniz = {};


    QString posizione_binario = "";
    datastruct *datatouch;
    void reset();

    currenttitle_class();
};

#endif
