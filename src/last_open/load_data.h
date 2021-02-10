#ifndef LOAD_DATA_H
#define LOAD_DATA_H

#include <QSettings>
#include "struct_last_file.h"

#include <QList>

QList<last_file *> load_data(QSettings &setting, int quanti);

#endif // LOAD_DATA_H
