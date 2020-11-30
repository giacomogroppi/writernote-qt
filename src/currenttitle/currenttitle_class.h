#ifndef CURRENT_TITLE_H
#define CURRENT_TITLE_H

#include "../mainwindow.h"
#include "ui_mainwindow.h"
#include <QStringList>
#include <QList>

class currenttitle_class{
public:
    int versione=1;
    bool se_registato = false, se_tradotto = false;
    std::string audio_position_path = "";
    QString testi = "";
    QStringList testinohtml = {};
    QList<int> posizione_iniz = {};

    void reset();
};

#endif
