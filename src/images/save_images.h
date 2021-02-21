#ifndef SAVE_IMAGES_H
#define SAVE_IMAGES_H

#include "../touch/datastruct/datastruct.h"
#include "../currenttitle/currenttitle_class.h"
#include "zip.h"

#include <QList>

#include "../utils/common_error_definition.h"

#define SOURCE_WRITE(x, y, z) if(zip_source_write(x, y, z) == -1) return ERROR


#define SOURCE_READ_EXT(x, y, z) check+=zip_fread(x, y, z) == -1

#define ARGUMENT(x, y, z) if(zip_fread(x, y,z)==-1)

#define SOURCE_READ(x, y, z) ARGUMENT(x,y,z)goto free_;
#define FIRST_SOURCE_READ(x, y, z) ARGUMENT(x,y,z)return ERROR;


int save_image(QList<struct immagine_S> *, zip_source_t *);

int load_image(QList<struct immagine_S> *, zip_file_t *);

#endif // SAVE_IMAGES_H
