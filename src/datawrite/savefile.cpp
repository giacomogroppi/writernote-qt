#include "savefile.h"
#include "../self_class.h"
#include "../indice_class.h"
#include "../currenttitle/currenttitle_class.h"

#include <cstdlib>
#include <iostream>
#include <cstdio>

#include <zlib.h>
#include <zip.h>
#include <stdlib.h>
#include <string.h>
#include "../self_class.h"
#include "../mainwindow.h"
#include "ui_mainwindow.h"

savefile::savefile(MainWindow *parent, currenttitle_class *currenttitle, QString *namecopybook){
    //this->parent = parent;
    this->currenttitle = currenttitle;
    this->namecopybook = namecopybook;
};
