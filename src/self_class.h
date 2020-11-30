#ifndef SELF__CLASS_H
#define SELF__CLASS_H

#define MAXSTRPATH 30
#include "currenttitle/currenttitle_class.h"
#include "indice_class.h"
#include <QStringList>
/*classe per la struttura self*/


class SelfClass{
    public:
        //SelfClass(){};
        bool registrazione_ = false, play_ = false, controlloscrittura = false;
        int currentTime = 0;
        std::string nameFile, path, currentTitle;

        indice_class indice;
        currenttitle_class currenttitle;

};


#endif // SELF_CLASS_H
