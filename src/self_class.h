#ifndef MAIN_CL_H
#define MAIN_CL_H

#define MAXSTRPATH 30
#include "currenttitle/currenttitle_class.h"
#include "indice_class.h"
#include <QStringList>
/*classe per la struttura self*/

typedef struct{
    bool scritto = false;
    char valuePath[MAXSTRPATH], username[MAXSTRPATH];
    int systemPlatform;
    
    /*
    0 -> windows32
    1 -> __apple__
    2 -> linux o unix
    */
} system_t;

class SelfClass{
    public:
        SelfClass(){};
        bool registrazione_ = false, play_ = false, controlloscrittura = false;
        int currentTime = 0;
        std::string nameFile, path, currentTitle;
        system_t system;

        indice_class indice;
        currenttitle_class currenttitle;

        void addbackslash(char *stringa);
};


#endif // MAIN_CL_H
