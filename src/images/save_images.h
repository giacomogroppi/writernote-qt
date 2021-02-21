#ifndef SAVE_IMAGES_H
#define SAVE_IMAGES_H

#include "../touch/datastruct/datastruct.h"
#include "../currenttitle/currenttitle_class.h"
#include "zip.h"

#include <QList>

#include "../utils/common_error_definition.h"

int save_image(QList<struct immagine_S> *, zip_source_t *);

int load_image(QList<struct immagine_S> *, zip_file_t *);

#endif // SAVE_IMAGES_H
