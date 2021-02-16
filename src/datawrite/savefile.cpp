#include "savefile.h"

#include <cstdlib>
#include <iostream>
#include <cstdio>

#include <zip.h>
#include <stdlib.h>
#include "../mainwindow.h"
#include "ui_mainwindow.h"

savefile::savefile(QString *path, currenttitle_class *currenttitle){
    this->path = path;
    this->currenttitle = currenttitle;
};
