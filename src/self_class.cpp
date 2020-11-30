#ifndef MAIN_CL_CPP
#define MAIN_CL_CPP

#include "self_class.h"
#include "currenttitle/currenttitle_class.cpp"
#include <sys/types.h>
#include <sys/stat.h>

/* for the filesistem */
//#include <filesystem>
//#include <experimental/filesystem>

#include <bits/stdc++.h>

using namespace std;
//namespace fs = std::experimental::filesystem;


void SelfClass::addbackslash(char *stringa){
    /*funzione che cambia la stringa da /tmp/writernote a /tmp/writernote/
     -> funziona su tutte le architetture
    */
   
    int lunghezza = strlen(stringa);

    if(system.systemPlatform == 0)
        stringa[lunghezza] = '\\';
    else if(system.systemPlatform == 2)
        stringa[lunghezza] = '/';
    
    stringa[lunghezza + 1] = '\0';

}

#endif // MAIN_CL_CPP
