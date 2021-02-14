#ifndef SAVE_IMAGES_H
#define SAVE_IMAGES_H

#include "../touch/datastruct/datastruct.h"
#include "../currenttitle/currenttitle_class.h"
#include "zip.h"

#include <QList>

#define OK 0
#define ERROR 1

#define SOURCE_WRITE(x, y, z) if(zip_source_write(x, y, z) == -1) return ERROR

int save_image(QList<struct immagine_S> *, zip_source_t *);

int load_image(QList<struct immagine_S> *, zip_file_t *);

#endif // SAVE_IMAGES_H
