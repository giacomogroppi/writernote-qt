#ifndef SAVE_IMAGES_H
#define SAVE_IMAGES_H

#include "../touch/datastruct/datastruct.h"

#include "zip.h"

int save_image(datastruct *, zip_source_t *);

int load_image(datastruct *, zip_file_t *);

#endif // SAVE_IMAGES_H
